/***************************************************************************
 *
 * _array.h - Declarations for the Standard Library __rw_array
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _array.h 91721 2005-08-08 18:30:07Z drodgman $
 *
 ***************************************************************************
 *
 * Copyright (c) 1994-2001 Rogue Wave Software, Inc.  All Rights Reserved.
 *
 * This computer software is owned by Rogue Wave Software, Inc. and is
 * protected by U.S. copyright laws and other laws and by international
 * treaties.  This computer software is furnished by Rogue Wave Software,
 * Inc. pursuant to a written license agreement and may be used, copied,
 * transmitted, and stored only in accordance with the terms of such
 * license and with the inclusion of the above copyright notice.  This
 * computer software or any other copies thereof may not be provided or
 * otherwise made available to any other person.
 *
 * U.S. Government Restricted Rights.  This computer software is provided
 * with Restricted Rights.  Use, duplication, or disclosure by the
 * Government is subject to restrictions as set forth in subparagraph (c)
 * (1) (ii) of The Rights in Technical Data and Computer Software clause
 * at DFARS 252.227-7013 or subparagraphs (c) (1) and (2) of the
 * Commercial Computer Software--Restricted Rights at 48 CFR 52.227-19,
 * as applicable.  Manufacturer is Rogue Wave Software, Inc., 5500
 * Flatiron Parkway, Boulder, Colorado 80301 USA.
 *
 **************************************************************************/

#ifndef _RWSTD_ARRAY_H_INCLUDED
#define _RWSTD_ARRAY_H_INCLUDED

#include <memory>

#include <rw/_defs.h>

#include _RWSTD_CSTDDEF


_RWSTD_NAMESPACE_BEGIN (__rw)


template <class _TypeT>
class __rw_array
{
public:

    typedef _RWSTD_C::size_t  size_type;
    typedef _TypeT            value_type;
    typedef value_type*       pointer;
    typedef value_type&       reference;
    typedef const value_type* const_pointer;
    typedef const value_type& const_reference;

    __rw_array ()
        : _C_size (0),
          _C_data (0) { }

    // allocate but do not initialize
    __rw_array (size_type);

    // allocate and initialize from value
    __rw_array (const_reference, size_type);

    // allocate and initialize from an array
    __rw_array (const_pointer, size_type);

    __rw_array (const __rw_array &__rhs);

    ~__rw_array () {
        resize (0);
    }

    __rw_array& operator= (const __rw_array &__rhs);

    size_type size () const {
        return _C_size;
    }

    value_type operator[] (size_type __inx) const {
        _RWSTD_ASSERT (__inx < size ());
        return begin () [__inx];
    }

    reference operator[] (size_type __inx) {
        _RWSTD_ASSERT (__inx < size ());
        return begin () [__inx];
    }

    void swap (__rw_array &__rhs);

    pointer begin () {
        return _C_data;
    }

    const_pointer begin () const {
        return _C_data;
    }

    pointer end () {
        return begin () + size ();
    }

    const_pointer end () const {
        return begin () + size ();
    }

    void resize (size_type, const_reference = value_type ());

private:
    size_type _C_size;   // number of elements
    pointer   _C_data;   // array of elements
};


template <class _TypeT>
inline __rw_array<_TypeT>::__rw_array (size_type __n)
    : _C_size (__n)
{
    if (__n) {
        _C_data = _RWSTD_STATIC_CAST (pointer,
                                      ::operator new (__n * sizeof *_C_data));
    }
    else
        _C_data = 0;
}


template <class _TypeT>
inline __rw_array<_TypeT>::__rw_array (const_reference __val, size_type __n)
    : _C_size (__n)
{
    if (__n) {
        _C_data = _RWSTD_STATIC_CAST (pointer,
                                      ::operator new (__n * sizeof *_C_data));
        _STD::uninitialized_fill_n (begin (), size (), __val);
    }
    else
        _C_data = 0;
}


template <class _TypeT>
inline __rw_array<_TypeT>::__rw_array (const_pointer __data, size_type __n)
    : _C_size (__n)
{ 
    if (__n) {
        _C_data = _RWSTD_STATIC_CAST (pointer,
                                      ::operator new (__n * sizeof *_C_data));
        _STD::uninitialized_copy (__data, __data + __n, begin ());
    }
    else
        _C_data = 0;
}


template <class _TypeT>
inline __rw_array<_TypeT>::__rw_array (const __rw_array<_TypeT> &__rhs)
    : _C_size (__rhs.size ())
{ 
    if (__rhs.size ()) {
        _C_data = _RWSTD_STATIC_CAST (pointer,
                                    ::operator new (_C_size * sizeof *_C_data));
        _STD::uninitialized_copy (__rhs.begin (), __rhs.end (), begin ());
    }
    else
        _C_data = 0;
}


template <class _TypeT>
inline __rw_array<_TypeT>&
__rw_array<_TypeT>::operator= (const __rw_array<_TypeT> &__rhs)
{
    if (__rhs.size () == size ())
        // do not allocate, just copy for efficiency
        _STD::copy (__rhs.begin (), __rhs.end (), begin ());
    else
        // allocate and copy into a temporary, then swap
        __rw_array <_TypeT> (__rhs).swap (*this);

    return *this;
}


template <class _TypeT>
inline void __rw_array<_TypeT>::swap (__rw_array<_TypeT> &__rhs)
{
    pointer __tmp_data   = begin ();
    size_type __tmp_size = size ();

    _C_data       = __rhs.begin ();
    _C_size       = __rhs.size ();
    __rhs._C_data = __tmp_data;
    __rhs._C_size = __tmp_size;
}


template <class _TypeT>
inline void __rw_array<_TypeT>::
resize (size_type __size, const_reference __val /* = value_type () */)
{
    if (begin ()) {
        __rw_destroy (begin (), end ());
        ::operator delete (begin ());
        _C_data = 0;
    }

    if (0 != (_C_size = __size)) {
        _C_data = _RWSTD_STATIC_CAST (pointer,
                              ::operator new (_C_size * sizeof *_C_data));
        _STD::uninitialized_fill_n (_C_data, _C_size, __val);
    }
}


_RWSTD_NAMESPACE_END   // __rw


#endif   // _RWSTD_ARRAY_H_INCLUDED
