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
#ifndef NDARRAY_ArrayTraits_h_INCLUDED
#define NDARRAY_ArrayTraits_h_INCLUDED

/** 
 *  @file ndarray/ArrayTraits.h
 *
 *  @brief Traits for Array.
 */

#include "ndarray_fwd.h"
#include "ndarray/ExpressionTraits.h"
#include "ndarray/detail/Core.h"
#include <boost/mpl/int.hpp>
#include <boost/mpl/bool.hpp>

namespace ndarray {
namespace detail {

template <int N, typename T2, int C2, typename T1, int C1>
struct Convertible : public boost::mpl::bool_<
    (((C2>=C1 && C1>=0) || (C2<=C1 && C1<=0) || (N == 1 && C2 == -C1)) 
     && boost::is_convertible<T2*,T1*>::value)
> {};

} // namespace detail


/**
 *  @brief Dimension-specialized traits shared by Array and ArrayRef.
 *
 *  @ingroup MainGroup
 */
template <typename T, int N, int C>
struct ArrayTraits {
    typedef T Element;
    typedef boost::mpl::int_<N> ND;
    typedef boost::mpl::int_<C> RMC;
    typedef detail::NestedIterator<T,N,C> Iterator;
    typedef ArrayRef<T,N-1,(N==C)?(N-1):((C>0)?C:0)> Reference;
    typedef Array<T,N-1,(N==C)?(N-1):((C>0)?C:0)> Value;
    typedef detail::Core<N> Core;
    typedef typename Core::ConstPtr CorePtr;

    static Reference makeReference(Element * data, CorePtr const & core) {
        return Reference(data, core);
    }
    static Iterator makeIterator(Element * data, CorePtr const & core, int stride) {
        return Iterator(Reference(data, core), stride);
    }
};

template <typename T>
struct ArrayTraits<T,1,0> {
    typedef T Element;
    typedef boost::mpl::int_<1> ND;
    typedef boost::mpl::int_<0> RMC;
    typedef detail::StridedIterator<Element> Iterator;
    typedef Element & Reference;
    typedef Element Value;
    typedef detail::Core<1> Core;
    typedef typename Core::ConstPtr CorePtr;

    static Reference makeReference(Element * data, CorePtr const & core) {
        return *data;
    }
    static Iterator makeIterator(Element * data, CorePtr const & core, int stride) {
        return Iterator(data, stride);
    }
};

template <typename T>
struct ArrayTraits<T,1,1> {
    typedef T Element;
    typedef boost::mpl::int_<1> ND;
    typedef boost::mpl::int_<1> RMC;
    typedef Element * Iterator;
    typedef Element & Reference;
    typedef Element Value;
    typedef detail::Core<1> Core;
    typedef typename Core::ConstPtr CorePtr;

    static Reference makeReference(Element * data, CorePtr const & core) {
        return *data;
    }
    static Iterator makeIterator(Element * data, CorePtr const & core, int stride) {
        return data;
    }
};

template <typename T>
struct ArrayTraits<T,1,-1> {
    typedef T Element;
    typedef boost::mpl::int_<1> ND;
    typedef boost::mpl::int_<-1> RMC;
    typedef Element * Iterator;
    typedef Element & Reference;
    typedef Element Value;
    typedef detail::Core<1> Core;
    typedef typename Core::ConstPtr CorePtr;

    static Reference makeReference(Element * data, CorePtr const & core) {
        return *data;
    }
    static Iterator makeIterator(Element * data, CorePtr const & core, int stride) {
        return data;
    }
};

template <typename T, int N, int C>
struct ExpressionTraits< Array<T,N,C> > : public ArrayTraits<T,N,C> {
    typedef Array<T,N,C> Self;
    typedef boost::mpl::false_ IsScalar;
};

template <typename T, int N, int C>
struct ExpressionTraits< ArrayRef<T,N,C> > : public ArrayTraits<T,N,C> {
    typedef ArrayRef<T,N,C> Self;
    typedef boost::mpl::false_ IsScalar;
};

} // namespace ndarray

#endif // !NDARRAY_ArrayTraits_h_INCLUDED
