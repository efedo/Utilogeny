/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_ENABLE_COMPARISON_09232005_1958)
#define FUSION_ENABLE_COMPARISON_09232005_1958

#include "Utilogeny/lib/boost/fusion/support/config.hpp"
#include "Utilogeny/lib/boost/mpl/or.hpp"
#include "Utilogeny/lib/boost/mpl/and.hpp"
#include "Utilogeny/lib/boost/mpl/not.hpp"
#include "Utilogeny/lib/boost/mpl/equal_to.hpp"
#include "Utilogeny/lib/boost/fusion/support/sequence_base.hpp"
#include "Utilogeny/lib/boost/fusion/support/is_sequence.hpp"
#include "Utilogeny/lib/boost/fusion/sequence/intrinsic/size.hpp"

namespace boost { namespace fusion { namespace traits
{
    template <typename Seq1, typename Seq2, typename Enable = void>
    struct enable_equality
        : mpl::and_<traits::is_sequence<Seq1>, traits::is_sequence<Seq2> >
    {};

    template <typename Seq1, typename Seq2, typename Enable = void>
    struct enable_comparison
        : mpl::and_<
            traits::is_sequence<Seq1>, traits::is_sequence<Seq2>
          , mpl::equal_to<result_of::size<Seq1>, result_of::size<Seq2> >
        >
    {};
}}}

#endif
