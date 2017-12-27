/***************************************************************************
 *
 * _random.h - Header for the Standard Library random generator
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _random.h 91721 2005-08-08 18:30:07Z drodgman $
 *
 ***************************************************************************
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
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

#ifndef _RWSTD_RANDOM_H_INCLUDED
#define _RWSTD_RANDOM_H_INCLUDED


#include <rw/_defs.h>
#include _RWSTD_CSTDDEF

#ifdef _RWSTD_MULTI_THREAD
#  include <rw/_mutex.h>
#endif   // _RWSTD_MULTI_THREAD


// prevent complaints by compilers such as g++ 2.95.2 that
// cannot inline functions containing static local) variables
#ifdef _INLINE_WITH_STATICS
#  define _INLINE _INLINE_WITH_STATICS
#else
#  define _INLINE
#endif


_RWSTD_NAMESPACE_BEGIN (__rw)


template <unsigned _SIZE>
class _RWSTD_EXPORT __rw_random_generator
{
    enum { _C_SIZE = _SIZE };

    unsigned long    _C_table [_C_SIZE];
    _RWSTD_C::size_t _C_index1;
    _RWSTD_C::size_t _C_index2;

public:

    __rw_random_generator (unsigned long __seed = 161803398) {
        seed (__seed);
    }

    void seed (unsigned long __seed);

    unsigned long operator() (unsigned long __limit);

    static unsigned long generate (unsigned long __limit);
};


template <unsigned _SIZE>
inline unsigned long
__rw_random_generator<_SIZE>::operator() (unsigned long __limit)
{
    _RWSTD_ASSERT (0 != __limit);

    _C_table [++_C_index1 %= _C_SIZE] -= _C_table [++_C_index2 %= _C_SIZE];

    return _C_table [_C_index1] % __limit;
}


template <unsigned _SIZE>
_INLINE unsigned long
__rw_random_generator<_SIZE>::generate (unsigned long __limit)
{
    _RWSTD_MT_STATIC_GUARD (__guard);

    static __rw_random_generator __gen;

    return __gen (__limit);
}


template <class _Distance>
inline _Distance __rw_random (_Distance __limit)
{
    return __rw_random_generator<55>::generate (__limit);
}


_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_random_generator<55>);


_RWSTD_NAMESPACE_END   // __rw


#undef _INLINE

#ifdef _RWSTD_COMPILE_INSTANTIATE
#  include <rw/_random.cc>
#endif


#endif   // _RWSTD_RANDOM_H_INCLUDED
