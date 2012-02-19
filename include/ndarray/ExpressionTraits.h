// -*- c++ -*-
/*
 * Copyright 2012, Jim Bosch and the LSST Corporation
 * 
 * ndarray is available under two licenses, both of which are described
 * more fully in other files that should be distributed along with
 * the code:
 * 
 *  - A simple BSD-style license (ndarray-bsd-license.txt); under this
 *    license ndarray is broadly compatible with essentially any other
 *    code.
 * 
 *  - As a part of the LSST data management software system, ndarray is
 *    licensed with under the GPL v3 (LsstLicenseStatement.txt).
 * 
 * These files can also be found in the source distribution at:
 * 
 * https://github.com/ndarray/ndarray
 */
#ifndef NDARRAY_ExpressionTraits_h_INCLUDED
#define NDARRAY_ExpressionTraits_h_INCLUDED

/** 
 *  @file ndarray/ExpressionTraits.h
 *
 *  @brief Traits for Expression.
 */

#include "ndarray_fwd.h"
#include <boost/static_assert.hpp>

namespace ndarray {

/**
 *  @brief Traits for expressions.
 *
 *  @ingroup MainGroup
 */
template <typename Expression_> struct ExpressionTraits {
    typedef boost::mpl::true_ IsScalar;
};

#ifndef GCC_45

/**
 *  @internal @brief ExpressionTraits specialization for UnaryOpExpression.
 *
 *  @ingroup ndarrayInternalGroup
 */
template <typename Operand, typename UnaryFunction, int N>
struct ExpressionTraits< detail::UnaryOpExpression<Operand,UnaryFunction,N> > {
    typedef typename UnaryFunction::result_type Element;
    typedef typename ExpressionTraits<Operand>::ND ND;
    typedef detail::UnaryOpIterator<Operand,UnaryFunction> Iterator;
    typedef detail::UnaryOpExpression<
        typename ExpressionTraits<Operand>::Reference,UnaryFunction,N-1
        > Value;
    typedef Value Reference;
    typedef boost::mpl::false_ IsScalar;
};

/**
 *  @internal @brief ExpressionTraits specialization for 1D UnaryOpExpression.
 *
 *  @ingroup ndarrayInternalGroup
 */
template <typename Operand, typename UnaryFunction>
struct ExpressionTraits< detail::UnaryOpExpression<Operand,UnaryFunction,1> > {
    typedef typename UnaryFunction::result_type Element;
    typedef typename ExpressionTraits<Operand>::ND ND;
    typedef detail::UnaryOpIterator<Operand,UnaryFunction> Iterator;
    typedef typename boost::remove_const<Element>::type Value;
    typedef Value const Reference;
    typedef boost::mpl::false_ IsScalar;
};

/**
 *  @internal @brief ExpressionTraits specialization for BinaryOpExpression.
 *
 *  @ingroup ndarrayInternalGroup
 */
template <typename Operand1, typename Operand2, typename BinaryFunction, int N>
struct ExpressionTraits< detail::BinaryOpExpression<Operand1,Operand2,BinaryFunction,N> > {
    typedef typename BinaryFunction::result_type Element;
    typedef typename ExpressionTraits<Operand1>::ND ND;
    typedef detail::BinaryOpIterator<Operand1,Operand2,BinaryFunction> Iterator;
    typedef detail::BinaryOpExpression<
        typename ExpressionTraits<Operand1>::Reference,
        typename ExpressionTraits<Operand2>::Reference,
        BinaryFunction, N-1 > Reference;
    typedef Reference Value;
    typedef boost::mpl::false_ IsScalar;
    BOOST_STATIC_ASSERT((ND::value == ExpressionTraits<Operand2>::ND::value));
};

/**
 *  @internal @brief ExpressionTraits specialization for 1D BinaryOpExpression.
 *
 *  @ingroup ndarrayInternalGroup
 */
template <typename Operand1, typename Operand2, typename BinaryFunction>
struct ExpressionTraits< detail::BinaryOpExpression<Operand1,Operand2,BinaryFunction,1> > {
    typedef typename BinaryFunction::result_type Element;
    typedef typename ExpressionTraits<Operand1>::ND ND;
    typedef detail::BinaryOpIterator<Operand1,Operand2,BinaryFunction> Iterator;
    typedef typename boost::remove_const<Element>::type Value;
    typedef Value const Reference;
    typedef boost::mpl::false_ IsScalar;
    BOOST_STATIC_ASSERT((ND::value == ExpressionTraits<Operand2>::ND::value));
};

#endif // GCC_45

} // namespace ndarray

#endif // !NDARRAY_ExpressionTraits_h_INCLUDED
