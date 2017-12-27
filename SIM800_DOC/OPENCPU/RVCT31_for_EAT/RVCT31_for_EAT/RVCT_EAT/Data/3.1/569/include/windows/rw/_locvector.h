/***************************************************************************
 *
 * _locvector.h - Declarations for locales local vector class
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _locvector.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_LOCVECTOR_H_INCLUDED
#define _RWSTD_LOCVECTOR_H_INCLUDED

#include <rw/_defs.h>
#include _RWSTD_CSTDDEF


_RWSTD_NAMESPACE_BEGIN (__rw)


// Minute vector class
template <class _TypeT>
class _RWSTD_EXPORT __rw_locale_vector
{
public:
    typedef _RWSTD_C::size_t    size_type;
    typedef _RWSTD_C::ptrdiff_t difference_type;
    typedef _TypeT              value_type;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef pointer             iterator;
    typedef const_pointer       const_iterator;
    
public:

    __rw_locale_vector (): _C_first(0), _C_size(0) { }
    
    __rw_locale_vector (size_type __n)
        : _C_first (__n ? new value_type [__n] : 0), _C_size (__n) { }
    
    __rw_locale_vector (size_type __n, const_reference __x)
        : _C_first (__n ? new value_type [__n] : 0), _C_size(__n) {
        while (__n)
            _C_first [--__n] = __x;
    }
    
    __rw_locale_vector (const __rw_locale_vector &__rhs)
        : _C_first (__rhs._C_size  ? new value_type [__rhs._C_size] : 0),
          _C_size (__rhs._C_size) {
        for (size_type __n = _C_size; __n--; )
            _C_first [__n] = __rhs._C_first [__n];
    }
    
    ~__rw_locale_vector () {
        delete [] _C_first;
    }

    void swap (__rw_locale_vector &__rhs) {
        pointer   __p = _C_first;
        size_type __n = _C_size;
        _C_first       = __rhs._C_first;
        _C_size        = __rhs._C_size;
        __rhs._C_first = __p;
        __rhs._C_size  = __n;
    }

    const __rw_locale_vector& operator= (const __rw_locale_vector &__rhs) {
        __rw_locale_vector (__rhs).swap (*this);
        return *this;
    }
    
    iterator begin () const {
        return _C_first;
    }
    
    iterator end () const {
        return _C_first + _C_size;
    }    
    
    size_type size () const {
        return _C_size;
    }
    
    reference operator[] (size_type __i) {
        return _C_first [__i];
    }
    
    const_reference operator[] (size_type __i) const {
        return _C_first [__i];
    }
    
    iterator resize (size_type __n, const_reference __x) {
        pointer __new_buf = new value_type [__n];
        iterator __j, __k;
        size_type __d = _C_size < __n ? _C_size : __n;
        
        for (__j = _C_first, __k = __new_buf; __j != _C_first+__d; ++__j,++__k)
            *__k =  *__j;
        
        while (__d < __n) 
            __new_buf [__d++] = __x;
        
        delete [] _C_first;
        _C_size  = __n;
        return _C_first = __new_buf;
    }

    iterator resize (size_type __n) { 
        return resize (__n, value_type ()); 
    }

private:

    iterator  _C_first;
    size_type _C_size;
};


_RWSTD_NAMESPACE_END   // __rw


#endif // _RWSTD_LOCVECTOR_H_INCLUDED
