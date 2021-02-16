/*
    Copyright (c) 2005-2020 Intel Corporation

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _TBB_mailbox_H
#define _TBB_mailbox_H

#include "oneapi/tbb/cache_aligned_allocator.h"
#include "oneapi/tbb/detail/_small_object_pool.h"

#include "arena_slot.h"
#include "scheduler_common.h"

#include <atomic>

namespace tbb {
namespace detail {
namespace r1 {

struct task_proxy : public d1::task {
    static const intptr_t      pool_bit = 1<<0;
    static const intptr_t   mailbox_bit = 1<<1;
    static const intptr_t location_mask = pool_bit | mailbox_bit;
    /* All but two low-order bits represent a (task*).
       Two low-order bits mean:
       1 = proxy is/was/will be in task pool
       2 = proxy is/was/will be in mailbox */
    std::atomic<intptr_t> task_and_tag;

    //! Pointer to next task_proxy in a mailbox
    std::atomic<task_proxy*> next_in_mailbox;

    //! Mailbox to which this was mailed.
    mail_outbox* outbox;

    //! Task affinity id which is referenced
    d1::slot_id slot;

    d1::small_object_pool* allocator;

    //! True if the proxy is stored both in its sender's pool and in the destination mailbox.
    static bool is_shared ( intptr_t tat ) {
        return (tat & location_mask) == location_mask;
    }

    //! Returns a pointer to the encapsulated task or NULL.
    static task* task_ptr ( intptr_t tat ) {
        return (task*)(tat & ~location_mask);
    }

    //! Returns a pointer to the encapsulated task or NULL, and frees proxy if necessary.
    template<intptr_t from_bit>
    inline task* extract_task () {
        // __TBB_ASSERT( prefix().extra_state == es_task_proxy, "Normal task misinterpreted as a proxy?" );
        intptr_t tat = task_and_tag.load(std::memory_order_acquire);
        __TBB_ASSERT( tat == from_bit || (is_shared(tat) && task_ptr(tat)),
            "Proxy's tag cannot specify both locations if the proxy "
            "was retrieved from one of its original locations" );
        if ( tat != from_bit ) {
            const intptr_t cleaner_bit = location_mask & ~from_bit;
            // Attempt to transition the proxy to the "empty" state with
            // cleaner_bit specifying entity responsible for its eventual freeing.
            // Explicit cast to void* is to work around a seeming ICC 11.1 bug.
            if ( task_and_tag.compare_exchange_strong(tat, cleaner_bit) ) {
                // Successfully grabbed the task, and left new owner with the job of freeing the proxy
                return task_ptr(tat);
            }
        }
        // Proxied task has already been claimed from another proxy location.
        __TBB_ASSERT( task_and_tag.load(std::memory_order_relaxed) == from_bit, "Empty proxy cannot contain non-zero task pointer" );
        return NULL;
    }
}; // struct task_proxy

//! Internal representation of mail_outbox, without padding.
class unpadded_mail_outbox {
protected:
    typedef std::atomic<task_proxy*> atomic_proxy_ptr;

    //! Pointer to first task_proxy in mailbox, or NULL if box is empty.
    atomic_proxy_ptr my_first;

    //! Pointer to pointer that will point to next item in the queue.  Never NULL.
    std::atomic<atomic_proxy_ptr*> my_last;

    //! Owner of mailbox is not executing a task, and has drained its own task pool.
    bool my_is_idle;
};

// TODO: - consider moving to arena slot
//! Class representing where mail is put.
/** Padded to occupy a cache line. */
class mail_outbox : padded<unpadded_mail_outbox> {

    task_proxy* internal_pop( isolation_type isolation ) {
        task_proxy* curr = my_first.load(std::memory_order_acquire);
        if ( !curr )
            return NULL;
        atomic_proxy_ptr* prev_ptr = &my_first;
        if ( isolation != no_isolation ) {
            while ( task_accessor::isolation(*curr) != isolation ) {
                prev_ptr = &curr->next_in_mailbox;
                curr = curr->next_in_mailbox.load(std::memory_order_relaxed);
                if ( !curr )
                    return NULL;
            }
        }
        // There is a first item in the mailbox.  See if there is a second.
        if ( task_proxy* second = curr->next_in_mailbox.load(std::memory_order_relaxed) ) {
            // There are at least two items, so first item can be popped easily.
            prev_ptr->store(second, std::memory_order_relaxed);
        } else {
            // There is only one item.  Some care is required to pop it.
            *prev_ptr = NULL;
            atomic_proxy_ptr* expected = &curr->next_in_mailbox;
            if ( my_last.compare_exchange_strong( expected, prev_ptr ) ) {
                // Successfully transitioned mailbox from having one item to having none.
                __TBB_ASSERT( !curr->next_in_mailbox.load(std::memory_order_relaxed), NULL );
            } else {
                // Some other thread updated my_last but has not filled in first->next_in_mailbox
                // Wait until first item points to second item.
                atomic_backoff backoff;
                while ( !(second = curr->next_in_mailbox.load(std::memory_order_relaxed)) ) backoff.pause();
                prev_ptr->store( second, std::memory_order_relaxed);
            }
        }
        __TBB_ASSERT( curr, NULL );
        return curr;
    }
public:
    friend class mail_inbox;

    //! Push task_proxy onto the mailbox queue of another thread.
    /** Implementation is wait-free. */
    void push( task_proxy* t ) {
        __TBB_ASSERT(t, NULL);
        t->next_in_mailbox = NULL;
        atomic_proxy_ptr* const link = my_last.exchange(&t->next_in_mailbox);
        // No release fence required for the next store, because there are no memory operations
        // between the previous fully fenced atomic operation and the store.
        link->store(t, std::memory_order_relaxed);
    }

    //! Return true if mailbox is empty
    bool empty() {
        return my_first.load(std::memory_order_relaxed) == NULL;
    }

    //! Construct *this as a mailbox from zeroed memory.
    /** Raise assertion if *this is not previously zeroed, or sizeof(this) is wrong.
        This method is provided instead of a full constructor since we know the object
        will be constructed in zeroed memory. */
    void construct() {
        __TBB_ASSERT( sizeof(*this)==max_nfs_size, NULL );
        __TBB_ASSERT( !my_first.load(std::memory_order_relaxed), NULL );
        __TBB_ASSERT( !my_last.load(std::memory_order_relaxed), NULL );
        __TBB_ASSERT( !my_is_idle, NULL );
        my_last = &my_first;
        suppress_unused_warning(pad);
    }

    //! Drain the mailbox
    intptr_t drain() {
        intptr_t k = 0;
        // No fences here because other threads have already quit.
        for( ; task_proxy* t = my_first; ++k ) {
            my_first.store(t->next_in_mailbox, std::memory_order_relaxed);
            // cache_aligned_deallocate((char*)t - task_prefix_reservation_size);
        }
        return k;
    }

    //! True if thread that owns this mailbox is looking for work.
    bool recipient_is_idle() {
        return my_is_idle;
    }
}; // class mail_outbox

//! Class representing source of mail.
class mail_inbox {
    //! Corresponding sink where mail that we receive will be put.
    mail_outbox* my_putter;
public:
    //! Construct unattached inbox
    mail_inbox() : my_putter(NULL) {}

    //! Attach inbox to a corresponding outbox.
    void attach( mail_outbox& putter ) {
        my_putter = &putter;
    }
    //! Detach inbox from its outbox
    void detach() {
        __TBB_ASSERT(my_putter,"not attached");
        my_putter = NULL;
    }
    //! Get next piece of mail, or NULL if mailbox is empty.
    task_proxy* pop( isolation_type isolation ) {
        return my_putter->internal_pop( isolation );
    }
    //! Return true if mailbox is empty
    bool empty() {
        return my_putter->empty();
    }
    //! Indicate whether thread that reads this mailbox is idle.
    /** Raises assertion failure if mailbox is redundantly marked as not idle. */
    void set_is_idle( bool value ) {
        if( my_putter ) {
            __TBB_ASSERT( my_putter->my_is_idle || value, "attempt to redundantly mark mailbox as not idle" );
            my_putter->my_is_idle = value;
        }
    }
    //! Indicate whether thread that reads this mailbox is idle.
    bool is_idle_state ( bool value ) const {
        return !my_putter || my_putter->my_is_idle == value;
    }
}; // class mail_inbox

} // namespace r1
} // namespace detail
} // namespace tbb

#endif /* _TBB_mailbox_H */
