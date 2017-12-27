/***************************************************************************
 *
 * _algobase.h - Declarations and inline definitions of frequently used
 *               C++ Standard Library algorithms
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _algobase.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_ALGOBASE_H_INCLUDED
#define _RWSTD_ALGOBASE_H_INCLUDED

#include <utility>   // for pair

#ifndef _RWSTD_NO_DEBUG_ITER
#  include <rw/_iterbase.h>
#endif   // _RWSTD_NO_DEBUG_ITER


#undef max
#undef min


_RWSTD_NAMESPACE_BEGIN (std)


// 25.3.7, p1
template <class _TypeT>
inline const _TypeT& min (const _TypeT& __a, const _TypeT& __b)
{
    return __b < __a ? __b : __a;
}

template <class _TypeT, class _Compare>
inline const _TypeT& min (const _TypeT& __a, const _TypeT& __b, _Compare __cmp)
{
    return __cmp (__b, __a) ? __b : __a;
}


// 25.3.7, p3
template <class _TypeT>
inline const _TypeT& max (const _TypeT& __a, const _TypeT& __b)
{
    return  __a < __b ? __b : __a;
}


template <class _TypeT, class _Compare>
inline const _TypeT& max (const _TypeT& __a, const _TypeT& __b, _Compare __cmp)
{
    return __cmp (__a, __b) ? __b : __a;
}


// 25.2.1, p1
template <class _InputIter, class _OutputIter>
inline _OutputIter
copy (_InputIter __first, _InputIter __last, _OutputIter __res)
{
    _RWSTD_ASSERT_RANGE (__first, __last);

    for (; __first != __last; ++__first,++__res)
        *__res = *__first;
    return __res;
}


// 25.2.1, p5
template <class _BidirIter1, class _BidirIter2>
inline _BidirIter2
copy_backward (_BidirIter1 __first, _BidirIter1 __last, _BidirIter2 __res)
{
    _RWSTD_ASSERT_RANGE (__first, __last);

    while (__first != __last)
        *--__res = *--__last;
    return __res;
}


// 25.2.5
template <class _FwdIter, class _TypeT>
inline void fill (_FwdIter __first, _FwdIter __last, const _TypeT& __value)
{
    _RWSTD_ASSERT_RANGE (__first, __last);

    for (;__first != __last; ++__first)
        *__first = __value;
}


template <class _OutputIter, class _Size, class _TypeT>
inline void fill_n (_OutputIter __first, _Size __n, const _TypeT& __value)
{
    for (;__n > 0;--__n, ++__first)
        *__first = __value;
}


_RWSTD_NAMESPACE_END   // std

_RWSTD_NAMESPACE_BEGIN (__rw)

// `less than' qusi-function object - used by algorithms for code reuse
template <class _TypeT>
struct __rw_lt
{
    bool operator() (const _TypeT &__lhs, const _TypeT &__rhs) const {
        return __lhs < __rhs;
    }
};

#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC

#  define _RWSTD_LESS(iterT) \
     _RW::__rw_lt<_TYPENAME _STD::iterator_traits< iterT >::value_type>()

#else   // if defined (_RWSTD_NO_PARTIAL_CLASS_SPEC)

#  define _RWSTD_LESS(iterT) \
     _RW::__rw_make_lt (_RWSTD_VALUE_TYPE (iterT))

template <class _TypeT>
__rw_lt<_TypeT> __rw_make_lt (const _TypeT*)
{
    return __rw_lt<_TypeT>();
}

#endif   // _RWSTD_NO_PARTIAL_CLASS_SPEC


// swaps values of 2 (possibly distinct) types
template <class _TypeT, class _TypeU>
inline void __rw_swap (_TypeT& __a, _TypeU& __b)
{
    // _TypeT must satisfy Assignable and CopyConstructible
    _TypeT __tmp = __a;
    __a = __b;
    __b = __tmp;
}


_RWSTD_NAMESPACE_END   // __rw


_RWSTD_NAMESPACE_BEGIN (std)


// 25.2.2, p1
template <class _TypeT>
inline void swap (_TypeT& __a, _TypeT& __b)
{
    // LWG issue 227 - _TypeT must satisfy Assignable and CopyConstructible
    _TypeT __tmp = __a;
    __a = __b;
    __b = __tmp;
}


// 25.2.2, p7
template <class _FwdIter1, class _FwdIter2>
inline void iter_swap (_FwdIter1 __a, _FwdIter2 __b)
{
    // std::iterator_traits<_FwdIter[12]>::value_type must satisfy
    // both Assignable and CopyConstructible
    _RW::__rw_swap (*__a, *__b);
}


// 25.1.7 - Mismatch
template <class _InputIter1, class _InputIter2>
inline pair<_InputIter1, _InputIter2>
mismatch (_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2)
{
    _RWSTD_ASSERT_RANGE (__first1, __last1);

    while (__first1 != __last1 && *__first1 == *__first2) {
        ++__first1;
        ++__first2;
    }
    return pair<_InputIter1, _InputIter2> (__first1, __first2);
}


template <class _InputIter1, class _InputIter2, class _BinaryPredicate>
inline pair<_InputIter1, _InputIter2>
mismatch (_InputIter1 __first1, _InputIter1 __last1,
          _InputIter2 __first2, _BinaryPredicate __pred)
{
    _RWSTD_ASSERT_RANGE (__first1, __last1);

    while (__first1 != __last1 && __pred (*__first1, *__first2)) {
        ++__first1;
        ++__first2;
    }
    return pair<_InputIter1, _InputIter2> (__first1, __first2);
}


// 25.1.8 - Equal
template <class _InputIter1, class _InputIter2>
inline bool
equal (_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2)
{
    return __last1 == _STD::mismatch (__first1, __last1, __first2).first;
}


template <class _InputIter1, class _InputIter2, class _BinaryPredicate>
inline bool equal (_InputIter1 __first1, _InputIter1 __last1,
                   _InputIter2 __first2, _BinaryPredicate __pred)
{
    return __last1 == _STD::mismatch (__first1, __last1,
                                      __first2, __pred).first;
}


// 25.3.8 - Lexicographical Comparison
template <class _InputIter1, class _InputIter2>
bool lexicographical_compare (_InputIter1 __first1, _InputIter1 __last1,
                              _InputIter2 __first2, _InputIter2 __last2);


template <class _InputIter1, class _InputIter2, class _Compare>
bool lexicographical_compare(_InputIter1 __first1, _InputIter1 __last1,
                             _InputIter2 __first2, _InputIter2 __last2,
                             _Compare __comp);

_RWSTD_NAMESPACE_END   // std


#ifdef _RWSTD_COMPILE_INSTANTIATE
#  include <rw/_algobase.cc>
#endif


#endif   // _RWSTD_ALGOBASE_H_INCLUDED
