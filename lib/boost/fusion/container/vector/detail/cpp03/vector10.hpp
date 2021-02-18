/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_VECTOR10_05042005_0257)
#define FUSION_VECTOR10_05042005_0257

#include "Utilogeny/lib/boost/fusion/support/config.hpp"
#include "Utilogeny/lib/boost/fusion/container/vector/detail/cpp03/vector10_fwd.hpp"
#include "Utilogeny/lib/boost/fusion/support/sequence_base.hpp"
#include "Utilogeny/lib/boost/fusion/support/is_sequence.hpp"
#include "Utilogeny/lib/boost/fusion/support/detail/access.hpp"
#include "Utilogeny/lib/boost/fusion/iterator/next.hpp"
#include "Utilogeny/lib/boost/fusion/iterator/deref.hpp"
#include "Utilogeny/lib/boost/fusion/sequence/intrinsic/begin.hpp"
#include "Utilogeny/lib/boost/fusion/container/vector/detail/at_impl.hpp"
#include "Utilogeny/lib/boost/fusion/container/vector/detail/value_at_impl.hpp"
#include "Utilogeny/lib/boost/fusion/container/vector/detail/begin_impl.hpp"
#include "Utilogeny/lib/boost/fusion/container/vector/detail/end_impl.hpp"

#include "Utilogeny/lib/boost/mpl/void.hpp"
#include "Utilogeny/lib/boost/mpl/int.hpp"
#include "Utilogeny/lib/boost/mpl/bool.hpp"
#include "Utilogeny/lib/boost/mpl/at.hpp"
#include "Utilogeny/lib/boost/mpl/vector/vector10.hpp"
#include "Utilogeny/lib/boost/type_traits/is_convertible.hpp"
#include "Utilogeny/lib/boost/utility/enable_if.hpp"

#include "Utilogeny/lib/boost/preprocessor/dec.hpp"
#include "Utilogeny/lib/boost/preprocessor/iteration/iterate.hpp"
#include "Utilogeny/lib/boost/preprocessor/repetition/enum.hpp"
#include "Utilogeny/lib/boost/preprocessor/repetition/enum_shifted.hpp"
#include "Utilogeny/lib/boost/preprocessor/repetition/enum_params.hpp"
#include "Utilogeny/lib/boost/preprocessor/repetition/enum_binary_params.hpp"
#include "Utilogeny/lib/boost/preprocessor/repetition/repeat_from_to.hpp"

namespace boost { namespace fusion
{
    struct vector_tag;
    struct fusion_sequence_tag;
    struct random_access_traversal_tag;

    template <typename Dummy>
    struct vector0 : sequence_base<vector0<Dummy> >
    {
        typedef mpl::vector0<> types;
        typedef vector_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::false_ is_view;
        typedef random_access_traversal_tag category;
        typedef mpl::int_<0> size;

        BOOST_CONSTEXPR BOOST_FUSION_GPU_ENABLED
        vector0() BOOST_NOEXCEPT {}

        template<typename Sequence>
        BOOST_CONSTEXPR BOOST_FUSION_GPU_ENABLED
        vector0(Sequence const& /*seq*/) BOOST_NOEXCEPT
        {}
    };
}}

#if !defined(BOOST_FUSION_DONT_USE_PREPROCESSED_FILES)
#include "Utilogeny/lib/boost/fusion/container/vector/detail/cpp03/preprocessed/vector10.hpp"
#else
#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 2, line: 0, output: "preprocessed/vector10.hpp")
#endif

/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    This is an auto-generated file. Do not edit!
==============================================================================*/

#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(preserve: 1)
#endif

namespace boost { namespace fusion
{
    struct vector_tag;
    struct fusion_sequence_tag;
    struct random_access_traversal_tag;

#define FUSION_HASH #
// expand vector1 to vector10
#define BOOST_PP_FILENAME_1 "Utilogeny/lib/boost/fusion/container/vector/detail/cpp03/vector_n.hpp"
#define BOOST_PP_ITERATION_LIMITS (1, 10)
#include BOOST_PP_ITERATE()
#undef FUSION_HASH
}}

#if defined(__WAVE__) && defined(BOOST_FUSION_CREATE_PREPROCESSED_FILES)
#pragma wave option(output: null)
#endif

#endif // BOOST_FUSION_DONT_USE_PREPROCESSED_FILES

#endif
