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
#ifndef NDARRAY_DETAIL_StridedIterator_h_INCLUDED
#define NDARRAY_DETAIL_StridedIterator_h_INCLUDED

/** 
 *  @file ndarray/detail/StridedIterator.h
 *
 *  @brief Definition of StridedIterator.
 */

#include "ndarray_fwd.h"
#include <boost/iterator/iterator_facade.hpp>

namespace ndarray {
namespace detail {

/**
 *  @internal @brief Strided iterator for noncontiguous 1D arrays.
 *
 *  @ingroup ndarrayInternalGroup
 */
template <typename T>
class StridedIterator : public boost::iterator_facade<
    StridedIterator<T>, 
    T, boost::random_access_traversal_tag
    >
{
public:
    typedef T Value;
    typedef T & Reference;
    
    StridedIterator() : _data(0), _stride(0) {}

    StridedIterator(T * data, int stride) : _data(data), _stride(stride) {}

    StridedIterator(StridedIterator const & other) : _data(other._data), _stride(other._stride) {}

    template <typename U>
    StridedIterator(StridedIterator<U> const & other) : _data(other._data), _stride(other._stride) {
        BOOST_STATIC_ASSERT((boost::is_convertible<U*,T*>::value));
    }

    StridedIterator & operator=(StridedIterator const & other) {
        if (&other != this) {
            _data = other._data;
            _stride = other._stride;
        }
        return *this;
    }

    template <typename U>
    StridedIterator & operator=(StridedIterator<U> const & other) {
        BOOST_STATIC_ASSERT((boost::is_convertible<U*,T*>::value));
        _data = other._data;
        _stride = other._stride;
        return *this;
    }

private:

    friend class boost::iterator_core_access;

    template <typename OtherT> friend class StridedIterator;

    Reference dereference() const { return *_data; }

    void increment() { _data += _stride; }
    void decrement() { _data -= _stride; }
    void advance(int n) { _data += _stride * n; }

    template <typename U>
    int distance_to(StridedIterator<U> const & other) const {
        return std::distance(_data, other._data) / _stride; 
    }

    template <typename U>
    bool equal(StridedIterator<U> const & other) const {
        return _data == other._data;
    }

    T * _data;
    int _stride;

};

} // namespace detail
} // namespace ndarray

#endif // !NDARRAY_DETAIL_StridedIterator_h_INCLUDED
