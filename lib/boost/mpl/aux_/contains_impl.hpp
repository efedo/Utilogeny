
#ifndef BOOST_MPL_AUX_CONTAINS_IMPL_HPP_INCLUDED
#define BOOST_MPL_AUX_CONTAINS_IMPL_HPP_INCLUDED

// Copyright Eric Friedman 2002
// Copyright Aleksey Gurtovoy 2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include "Utilogeny/lib/boost/mpl/contains_fwd.hpp"
#include "Utilogeny/lib/boost/mpl/begin_end.hpp"
#include "Utilogeny/lib/boost/mpl/find.hpp"
#include "Utilogeny/lib/boost/mpl/not.hpp"
#include "Utilogeny/lib/boost/mpl/aux_/traits_lambda_spec.hpp"
#include "Utilogeny/lib/boost/mpl/aux_/config/forwarding.hpp"
#include "Utilogeny/lib/boost/mpl/aux_/config/static_constant.hpp"

#include "Utilogeny/lib/boost/type_traits/is_same.hpp"

namespace boost { namespace mpl {

template< typename Tag >
struct contains_impl
{
    template< typename Sequence, typename T > struct apply
#if !defined(BOOST_MPL_CFG_NO_NESTED_FORWARDING)
        : not_< is_same<
              typename find<Sequence,T>::type
            , typename end<Sequence>::type
            > >
    {
#else
    {
        typedef not_< is_same<
              typename find<Sequence,T>::type
            , typename end<Sequence>::type
            > > type;

        BOOST_STATIC_CONSTANT(bool, value = 
              (not_< is_same<
                  typename find<Sequence,T>::type
                , typename end<Sequence>::type
                > >::value)
            );
#endif
    };
};

BOOST_MPL_ALGORITM_TRAITS_LAMBDA_SPEC(2,contains_impl)

}}

#endif // BOOST_MPL_AUX_CONTAINS_IMPL_HPP_INCLUDED
