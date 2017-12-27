/***************************************************************************
 *
 * _iterbase.h - Definitions of iterator primitives
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _iterbase.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_ITERBASE_H_INCLUDED
#define _RWSTD_ITERBASE_H_INCLUDED

#include <rw/_defs.h>
#include _RWSTD_CSTDDEF


_RWSTD_NAMESPACE_BEGIN (std)


// 24.3.1 - Iterator traits
template <class _Iterator>
struct iterator_traits
{
    typedef _TYPENAME _Iterator::value_type        value_type;
    typedef _TYPENAME _Iterator::difference_type   difference_type;
    typedef _TYPENAME _Iterator::pointer           pointer;
    typedef _TYPENAME _Iterator::reference         reference;
    typedef _TYPENAME _Iterator::iterator_category iterator_category;
};


// 24.3.3 - Standard iterator tags
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };


#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC

template <class _TypeT>
struct iterator_traits<_TypeT*>
{
    typedef _TypeT                     value_type;
    typedef ptrdiff_t                  difference_type;
    typedef value_type*                pointer;
    typedef value_type&                reference;
    typedef random_access_iterator_tag iterator_category;
};


template <class _TypeT>
struct iterator_traits<const _TypeT*>
{
    typedef _TypeT                     value_type;
    typedef ptrdiff_t                  difference_type;
    typedef const value_type*          pointer;
    typedef const value_type&          reference;
    typedef random_access_iterator_tag iterator_category;
};

#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC


// 24.3.2 - Basic iterator
template <class _Category, class _TypeT,  
          class _Distance _RWSTD_SIMPLE_DEFAULT (ptrdiff_t), 
          class _Pointer _RWSTD_SIMPLE_DEFAULT (_TypeT*), 
          class _Reference _RWSTD_SIMPLE_DEFAULT (_TypeT&)>
struct iterator
{
    typedef _TypeT     value_type;
    typedef _Distance  difference_type;
    typedef _Pointer   pointer;
    typedef _Reference reference;
    typedef _Category  iterator_category;
};


// returns the category of an iterator
template <class _TypeT>
inline random_access_iterator_tag __iterator_category (const _TypeT*)
{
    return random_access_iterator_tag ();
}


template <class _Category, class _TypeT, class _Distance, 
          class _Pointer, class _Reference> 
inline _Category
__iterator_category (const iterator<_Category, _TypeT,
                                    _Distance, _Pointer, _Reference>&)
{
    typedef _TYPENAME iterator<_Category, _TypeT, _Distance, _TypeT*,
                               _TypeT&>::iterator_category _IterCategory;

    return _IterCategory ();
}


template <class _Tag>
inline bool __is_input_iterator (_Tag)
{
    return false;
}


template <class _Tag>
inline bool __is_bidirectional_iterator (_Tag)
{
    return false;
}


template <class _Tag>
inline bool __is_random_access_iterator (_Tag)
{
    return false;
}


_RWSTD_SPECIALIZED_FUNCTION 
inline bool __is_input_iterator (input_iterator_tag)
{
    return true;
}


_RWSTD_SPECIALIZED_FUNCTION 
inline bool __is_bidirectional_iterator (bidirectional_iterator_tag)
{
    return true;
}


_RWSTD_SPECIALIZED_FUNCTION 
inline bool __is_bidirectional_iterator (random_access_iterator_tag)
{
    return true;
}


_RWSTD_SPECIALIZED_FUNCTION 
inline bool __is_random_access_iterator (random_access_iterator_tag)
{
    return true;
}


#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC

template <class _Iterator>
inline _TYPENAME iterator_traits<_Iterator>::value_type*
__value_type (const _Iterator*)
{ 
    return 0;
}

#else   // if defined (_RWSTD_NO_CLASS_PARTIAL_SPEC)

template <class _Category, class _TypeT, class _Distance, 
          class _Pointer, class _Reference>
inline _TypeT*
__value_type (const iterator<_Category, _TypeT, _Distance,
                             _Pointer, _Reference>*)
{
    return 0;
}

template <class _TypeT>
inline _TypeT* __value_type (const _TypeT* const*)
{
    return 0;
}

#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC


#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC

template <class _Iterator>
inline _TYPENAME iterator_traits<_Iterator>::difference_type*
__distance_type (_Iterator)
{ 
    return 0;
}

#else   // if defined (_RWSTD_NO_CLASS_PARTIAL_SPEC)

template <class _Category, class _TypeT, class _Distance, 
          class _Pointer, class _Reference>
inline _Distance* 
__distance_type (iterator<_Category, _TypeT, _Distance, _Pointer, _Reference>)
{
    return 0;
}

template <class _TypeT>
inline ptrdiff_t* __distance_type (const _TypeT*)
{ 
    return 0;
}

#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC


// 24.3.4 - Iterator operations

template <class _InputIterator, class _Distance>
inline void __advance (_InputIterator &__it, _Distance __n, input_iterator_tag)
{
    _RWSTD_ASSERT (__n == 0 || __n > 0);

    while (__n > 0) {
        --__n;
        ++__it;
    }
}


template <class _ForwardIterator, class _Distance>
inline void __advance (_ForwardIterator &__it, _Distance __n,
                       forward_iterator_tag)
{
    __advance (__it, __n, input_iterator_tag ());
}


template <class _BidirectionalIterator, class _Distance>
inline void __advance (_BidirectionalIterator &__it, _Distance __n, 
                       bidirectional_iterator_tag)
{
    if (__n > 0)
        __advance (__it, __n, input_iterator_tag ());
    else
        while (__n) {
            ++__n;
            --__it;
        }
}


template <class _RandomAccessIterator, class _Distance>
inline void __advance (_RandomAccessIterator& __it, _Distance __n, 
                       random_access_iterator_tag)
{
    __it += __n;
}


// 24.3.4, p2
template <class _InputIterator, class _Distance>
inline void advance (_InputIterator& __it, _Distance __n)
{
    __advance (__it, __n, _RWSTD_ITERATOR_CATEGORY (_InputIterator, __it));
}


template <class _InputIterator, class _Distance>
inline void __distance (const _InputIterator &__first,
                        const _InputIterator &__last,
                        _Distance            &__n,
                        input_iterator_tag)
{
    for (_InputIterator __it = __first; __it != __last; ++__it)
        ++__n;
}


template <class _ForwardIterator, class _Distance>
inline void __distance (const _ForwardIterator &__first,
                        const _ForwardIterator &__last,
                        _Distance              &__n,
                        forward_iterator_tag)
{
    __distance (__first, __last, __n, input_iterator_tag ());
}

template <class _BidirectionalIterator, class _Distance>
inline void __distance (const _BidirectionalIterator &__first,
                        const _BidirectionalIterator &__last, 
                        _Distance                    &__n,
                        bidirectional_iterator_tag)
{
    __distance (__first, __last, __n, input_iterator_tag ());
}


template <class _RandomAccessIterator, class _Distance>
inline void __distance (const _RandomAccessIterator &__first,
                        const _RandomAccessIterator &__last, 
                        _Distance                   &__n,
                        random_access_iterator_tag)
{
    __n = __last - __first;
}


#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC

// 24.3.4, p4
template <class _ForwardIterator>
inline _TYPENAME iterator_traits<_ForwardIterator>::difference_type
distance (const _ForwardIterator &__first, const _ForwardIterator &__last)
{
    _TYPENAME iterator_traits<_ForwardIterator>::difference_type __n = 0;

    __distance (__first, __last, __n,
                _RWSTD_ITERATOR_CATEGORY (_ForwardIterator, __first));

    return __n;
}

#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC


#ifndef _RWSTD_NO_EXT_VOID_DISTANCE

template <class _ForwardIterator, class _Distance>
inline void distance (const _ForwardIterator &__first,
                      const _ForwardIterator &__last,
                      _Distance              &__n)
{
    __distance (__first, __last, __n,
                _RWSTD_ITERATOR_CATEGORY (_ForwardIterator, __first));
}

#endif   // _RWSTD_NO_EXT_VOID_DISTANCE


_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)

// __rw_distance: Same purpose as 3-parameter distance function, but
// with return value.

template <class _ForwardIterator, class _Distance>
inline _Distance
__rw_distance (const _ForwardIterator &__first,
               const _ForwardIterator &__last,
               _Distance               __n)
{
    _STD::__distance (__first, __last, __n,
                      _RWSTD_ITERATOR_CATEGORY (_ForwardIterator, __first));
    return __n;
}


_RWSTD_NAMESPACE_END // __rw


#ifndef _RWSTD_NO_DEBUG_ITER

_RWSTD_NAMESPACE_BEGIN (__rw)

// __rw_debug_iter - iterator adapter with debugging support
// _Iterator is either iterator or const_iterator; if the latter,
// _MutableIterator should be iterator to allow for implicit
// conversions from non-const (mutable) to const_iterator objects


template <class _Container, class _Iterator, class _MutableIterator>
class __rw_debug_iter
{
    typedef _Container                                container_type;
    typedef _Iterator                                 iterator_type;
    typedef _STD::iterator_traits<iterator_type>      traits_type;

public:

    typedef _TYPENAME traits_type::value_type         value_type;
    typedef _TYPENAME traits_type::difference_type    difference_type;
    typedef _TYPENAME traits_type::reference          reference;
    typedef _TYPENAME traits_type::pointer            pointer;
    typedef _TYPENAME traits_type::iterator_category  iterator_category;

    typedef __rw_debug_iter <container_type, _MutableIterator,
                             _MutableIterator>        _C_mutable_iterator;

    __rw_debug_iter (): _C_cont (0) { }

    __rw_debug_iter (const container_type &__cont, const iterator_type &__it)
        : _C_iter (__it), _C_cont (&__cont) { }

    // no copy ctor other than the one below is defined
    // will use a compiler generated one if _Iterator != _MutableIterator
    __rw_debug_iter (const _C_mutable_iterator &__rhs)
        : _C_iter (__rhs._C_iter), _C_cont (__rhs._C_cont) { }

    __rw_debug_iter& operator= (const __rw_debug_iter &__rhs) {
        if (this != &__rhs) {
            _C_iter = __rhs._C_iter;
            _C_cont = __rhs._C_cont;
        }
        return *this;
    }

    reference operator* () const {
        _RWSTD_ASSERT (_C_is_dereferenceable ());
        return *_C_iter;
    }

    reference operator[] (difference_type __n) const {
        _RWSTD_ASSERT ((*this + __n)._C_is_dereferenceable ());
        return _C_iter [__n];
    }

    _RWSTD_OPERATOR_ARROW (pointer operator-> () const);

    __rw_debug_iter& operator++ () {
        _RWSTD_ASSERT (!_C_is_end ());
        return ++_C_iter, *this;
    }

    __rw_debug_iter& operator-- () {
        _RWSTD_ASSERT (!_C_is_begin ());
        return --_C_iter, *this;
    }

    __rw_debug_iter operator++ (int) {
        __rw_debug_iter __tmp = *this;
        return ++*this, __tmp;
    }

    __rw_debug_iter operator-- (int) {
        __rw_debug_iter __tmp = *this;
        return --*this, __tmp;
    }

    __rw_debug_iter& operator+= (difference_type __n) {
        _C_iter += __n;
        _RWSTD_ASSERT (   _C_iter >= _C_cont->begin ()._C_iter
                       && _C_iter <= _C_cont->end ()._C_iter);
        return *this;
    }

    __rw_debug_iter& operator-= (difference_type __n) {
        _C_iter -= __n;
        _RWSTD_ASSERT (   _C_iter >= _C_cont->begin ()._C_iter
                       && _C_iter <= _C_cont->end ()._C_iter);
        return *this;
    }

    __rw_debug_iter operator+ (difference_type __n) const {
        return __rw_debug_iter (*this) += __n;
    }

    __rw_debug_iter operator- (difference_type __n) const {
        return __rw_debug_iter (*this) -= __n;
    }

    bool _C_is_begin () const {
        return _C_cont && _C_cont->begin () == *this;
    }

    bool _C_is_end () const {
        return _C_cont && _C_cont->end () == *this;
    }

    bool _C_is_dereferenceable () const {
        return !_C_is_end ();
    }

    bool _C_valid_range (const __rw_debug_iter &__it) const {
        return _C_cont && _C_cont == __it._C_cont;
    }

    const iterator_type& base () const {
        return _C_iter;
    }

    iterator_type& base () {
        return _C_iter;
    }

#ifndef _RWSTD_NO_MEMBER_TEMPLATES

    // operators are templatized to assure const/non-const symmetry

    template <class _Iter>
    difference_type
    operator- (const __rw_debug_iter<container_type, _Iter,
                                     _MutableIterator> &__rhs) const {
        _RWSTD_ASSERT (_C_cont && _C_cont == __rhs._C_cont);
        return _C_iter - __rhs._C_iter;
    }

    template <class _Iter>
    bool
    operator== (const __rw_debug_iter<container_type, _Iter,
                                      _MutableIterator> &__rhs) const {
        return _C_iter == __rhs._C_iter;
    }
    
    template <class _Iter>
    bool
    operator< (const __rw_debug_iter<container_type, _Iter,
                                     _MutableIterator> &__rhs) const {
        return _C_iter < __rhs._C_iter;
    }

    template <class _Iter>
    bool
    operator!= (const __rw_debug_iter<container_type, _Iter,
                                      _MutableIterator> &__rhs) const {
        return _C_iter != __rhs._C_iter;
    }

    template <class _Iter>
    bool
    operator<= (const __rw_debug_iter<container_type, _Iter,
                                      _MutableIterator> &__rhs) const {
        return _C_iter <= __rhs._C_iter;
    }

    template <class _Iter>
    bool
    operator> (const __rw_debug_iter<container_type, _Iter,
                                     _MutableIterator> &__rhs) const {
        return _C_iter > __rhs._C_iter;
    }

    template <class _Iter>
    bool
    operator>= (const __rw_debug_iter<container_type, _Iter,
                                      _MutableIterator> &__rhs) const {
        return _C_iter >= __rhs._C_iter;
    }

#endif   // _RWSTD_NO_MEMBER_TEMPLATES

    iterator_type         _C_iter;   // wrapped iterator
    const container_type *_C_cont;   // associated container
};


_RWSTD_NAMESPACE_END   // __rw


_RWSTD_NAMESPACE_BEGIN (std)


#ifndef _RWSTD_NO_NONDEDUCED_CONTEXT
# define _RWSTD_CONT_DIFF_TYPE _TYPENAME _Cont::difference_type 
#else
# define _RWSTD_CONT_DIFF_TYPE ptrdiff_t 
#endif

template <class _Cont, class _Iter, class _MutIter>
inline _RW::__rw_debug_iter<_Cont, _Iter, _MutIter> 
operator+ (_RWSTD_CONT_DIFF_TYPE                               __n,
           const _RW::__rw_debug_iter<_Cont, _Iter, _MutIter> &__x)
{
    return __x + __n;
}

#undef _RWSTD_CONT_DIFF_TYPE 


#ifdef _RWSTD_NO_MEMBER_TEMPLATES

// with no support for member templates namespace-scope (non-member)
// operators must be used - these will cause ambiguities with those
// in std::rel_ops if the latter are found during lookup



// _Iter1 may differ from _Iter2 if the function operands are const
// and non-const iterators, respectively (allows symmetry)

template <class _Cont, class _Iter1, class _Iter2, class _MutIter>
inline _TYPENAME _Cont::difference_type
operator- (const _RW::__rw_debug_iter<_Cont, _Iter1, _MutIter> &__x,
           const _RW::__rw_debug_iter<_Cont, _Iter2, _MutIter> &__y)
{
    _RWSTD_ASSERT (__x._C_cont && __x._C_cont == __y._C_cont);
    return __x._C_iter - __y._C_iter;
}
    
template <class _Cont, class _Iter1, class _Iter2, class _MutIter>
inline bool
operator== (const _RW::__rw_debug_iter<_Cont, _Iter1, _MutIter> &__x,
            const _RW::__rw_debug_iter<_Cont, _Iter2, _MutIter> &__y)
{
    return __x._C_iter == __y._C_iter;
}

template <class _Cont, class _Iter1, class _Iter2, class _MutIter>
inline bool
operator< (const _RW::__rw_debug_iter<_Cont, _Iter1, _MutIter> &__x,
           const _RW::__rw_debug_iter<_Cont, _Iter2, _MutIter> &__y)
{
    _RWSTD_ASSERT (__x._C_cont && __x._C_cont == __y._C_cont);
    return __x._C_iter < __y._C_iter;
}

template <class _Cont, class _Iter1, class _Iter2, class _MutIter>
inline bool
operator!= (const _RW::__rw_debug_iter<_Cont, _Iter1, _MutIter> &__x,
            const _RW::__rw_debug_iter<_Cont, _Iter2, _MutIter> &__y)
{
    return !(__x == __y);
}

template <class _Cont, class _Iter1, class _Iter2, class _MutIter>
inline bool
operator<= (const _RW::__rw_debug_iter<_Cont, _Iter1, _MutIter> &__x,
            const _RW::__rw_debug_iter<_Cont, _Iter2, _MutIter> &__y)
{
    return !(__y < __x);
}

template <class _Cont, class _Iter1, class _Iter2, class _MutIter>
inline bool
operator>= (const _RW::__rw_debug_iter<_Cont, _Iter1, _MutIter> &__x,
            const _RW::__rw_debug_iter<_Cont, _Iter2, _MutIter> &__y)
{
    return !(__x < __y);
}

template <class _Cont, class _Iter1, class _Iter2, class _MutIter>
inline bool
operator> (const _RW::__rw_debug_iter<_Cont, _Iter1, _MutIter> &__x,
           const _RW::__rw_debug_iter<_Cont, _Iter2, _MutIter> &__y)
{
    return __y < __x;
}

#endif   // _RWSTD_NO_MEMBER_TEMPLATES

_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)


#define _RWSTD_DEBUG_ITER(cont, it, mutit) __rw_debug_iter< cont, it, mutit >


template <class _Cont, class _Iter, class _MutIter>
inline bool
__rw_valid_range (const _RWSTD_DEBUG_ITER(_Cont, _Iter, _MutIter) &__first,
                  const _RWSTD_DEBUG_ITER(_Cont, _Iter, _MutIter) &__last)
{
    return __first._C_cont && __first._C_cont == __last._C_cont;
}


template <class _Iterator>
inline bool
__rw_valid_range (const _Iterator &, const _Iterator &)
{
    return true;
}


template <class _Cont, class _Iter, class _MutIter>
inline bool
__rw_in_range (const _RWSTD_DEBUG_ITER(_Cont, _Iter, _MutIter) &__it,
               const _RWSTD_DEBUG_ITER(_Cont, _Iter, _MutIter) &__first,
               const _RWSTD_DEBUG_ITER(_Cont, _Iter, _MutIter) &__last)
{
    return    __rw_valid_range (__first, __it)
           && __rw_valid_range (__it, __last);
}


template <class _Iterator>
inline bool
__rw_in_range (const _Iterator&, const _Iterator&, const _Iterator&)
{
    return true;
}


template <class _Cont, class _Iter, class _MutIter>
inline bool
__rw_dereferenceable (const _RWSTD_DEBUG_ITER(_Cont, _Iter, _MutIter) &__it)
{
    return __it._C_is_dereferenceable ();
}


template <class _Iterator>
inline bool
__rw_dereferenceable (const _Iterator&)
{
    return true;
}


template <class _TypeT>
inline bool
__rw_dereferenceable (const _TypeT *__ptr)
{
    return 0 != __ptr;
}

_RWSTD_NAMESPACE_END   // __rw

#undef _RWSTD_DEBUG_ITER

#endif   // _RWSTD_NO_DEBUG_ITER


#endif   // _RWSTD_ITERBASE_H_INCLUDED
