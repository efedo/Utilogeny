// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/precomp.h"

template <class T>
struct atomic_spawnable : public std::atomic<T>
{
    atomic_spawnable()
        : std::atomic<T>() {
    }

    atomic_spawnable(const atomic_spawnable &)
        : std::atomic<T>() {
    }

    atomic_spawnable(const T & val)
        : std::atomic<T>(val) {
    }

    atomic_spawnable<T>& operator=(const atomic_spawnable&) {
        return *this;
    }

    atomic_spawnable<T>& operator=(const T & val) {
        std::atomic<T>::operator=(val);
        return *this;
    }
};

// Wrapper around shared mutex to make it recursive AND spawnable
// When copied, spawns new mutex
class shared_recursive_mutex_spawnable : public std::shared_mutex
{
public:

    shared_recursive_mutex_spawnable()
        : shared_mutex() {
    }

    shared_recursive_mutex_spawnable(const shared_recursive_mutex_spawnable&) 
        : shared_mutex() {
    }

    shared_recursive_mutex_spawnable& operator=(const shared_recursive_mutex_spawnable&) {
        return *this; 
    }


    void lock(void) {
        std::thread::id this_id = std::this_thread::get_id();
        if (owner == this_id) {
            // recursive locking
            count++;
        }
        else {
            // normal locking
            shared_mutex::lock();
            owner = this_id;
            count = 1;
        }
    }

    bool try_lock() {
        std::thread::id this_id = std::this_thread::get_id();
        if (owner == this_id) {
            // recursive locking
            count++;
            return true;
        }
        else {
            // normal locking
            if (shared_mutex::try_lock()) { // lock succeeded
                owner = this_id;
                count = 1;
                return true;
            }
            else { // lock failed
                return false;
            }
        }
    }

    void unlock(void) {
        if (count > 1) {
            // recursive unlocking
            count--;
        }
        else {
            // normal unlocking
            owner = std::thread::id();
            count = 0;
            shared_mutex::unlock();
        }
    }

    void lock_shared()
    {
        std::thread::id this_id = std::this_thread::get_id();
        auto it = shared_locks.find(this_id);
        if (it != shared_locks.end()) {
            // recursive locking
            (it->second)++;
        }
        else {
            shared_mutex::lock_shared();
            shared_locks.emplace(this_id, 1);
        }
    }

    bool try_lock_shared()
    {
        std::thread::id this_id = std::this_thread::get_id();
        auto it = shared_locks.find(this_id);
        if (it != shared_locks.end()) {
            // recursive locking
            (it->second)++;
        }
        else {
            // normal locking
            if (shared_mutex::try_lock_shared()) { // lock succeeded
                shared_locks.emplace(this_id, 1);
                return true;
            }
            else { // lock failed
                return false;
            }
        }
    }

    void unlock_shared()
    {
        std::thread::id this_id = std::this_thread::get_id();
        int & shared_count = shared_locks.at(this_id);
        if (shared_count > 1) {
            // recursive unlocking
            shared_count--;
        }
        else {
            // normal unlocking
            shared_locks.erase(this_id);
            shared_mutex::unlock_shared();
        }
    }

private:
    std::atomic<std::thread::id> owner;
    int count;
    std::map<std::thread::id, int> shared_locks;
};