/***************************************************************************
 *
 * _iterator.h - Iterator declarations for the Standard Library
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _iterator.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_ITERATOR_H_INCLUDED
#define _RWSTD_ITERATOR_H_INCLUDED

#include <rw/_iterbase.h>
#include <rw/_defs.h>

#include _RWSTD_CSTDDEF


_RWSTD_NAMESPACE_BEGIN (std)


// Reverse iterator.     

//
//  Macros for reverse iterators to accomodate non-standard compilers
//
#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC 
#  define _RWSTD_ITER_TEMPLATE template <class _Iterator>
#  define _RWSTD_ITER_ID(i)    i <_Iterator>
#  define _RWSTD_ITER_DIFF_TYPE(i, ignore)                             \
          _TYPENAME iterator_traits<_Iterator>::difference_type
#else
#  define _RWSTD_ITER_TEMPLATE                                         \
          template <class _Iterator, class _Category, class _TypeT,    \
                    class _Reference, class _Pointer, class _Distance>
#  define _RWSTD_ITER_ID(i)                                            \
          i <_Iterator, _Category, _TypeT, _Reference, _Pointer, _Distance>
#  define _RWSTD_ITER_DIFF_TYPE(ignore, distance) distance          
#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC 


#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC 

template <class _Iterator>
class reverse_iterator
    : public iterator<_TYPENAME iterator_traits<_Iterator>::iterator_category, 
                      _TYPENAME iterator_traits<_Iterator>::value_type, 
                      _TYPENAME iterator_traits<_Iterator>::difference_type,
                      _TYPENAME iterator_traits<_Iterator>::pointer, 
                      _TYPENAME iterator_traits<_Iterator>::reference>
{
    typedef iterator_traits<_Iterator>  traits_type;
public:
    typedef _TYPENAME traits_type::difference_type difference_type;
    typedef _TYPENAME traits_type::value_type value_type;
    typedef _TYPENAME traits_type::pointer pointer;
    typedef _TYPENAME traits_type::reference reference;

#else   // if defined (_RWSTD_NO_CLASS_PARTIAL_SPEC)

template <class _Iterator, class _Category, class _TypeT,
          class _Reference _RWSTD_COMPLEX_DEFAULT (_TypeT&), 
          class _Pointer _RWSTD_COMPLEX_DEFAULT (_TypeT*), 
          class _Distance _RWSTD_COMPLEX_DEFAULT (ptrdiff_t)>
class reverse_iterator
    : public iterator<_Category, _TypeT, _Distance, _Pointer, _Reference>

{
public:
    typedef _Distance  difference_type;
    typedef _TypeT     value_type;
    typedef _Reference reference;
    typedef _Pointer   pointer;

#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC

    typedef _Iterator iterator_type;

    reverse_iterator () { }

    _EXPLICIT reverse_iterator (iterator_type __rhs) : current (__rhs) { }
 

#ifndef _RWSTD_NO_MEMBER_TEMPLATES
#  ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC 

      template <class _TypeU>
      reverse_iterator (const reverse_iterator<_TypeU>& __rhs)
          : current (__rhs.base ()) { }

#  else   // if defined (_RWSTD_NO_CLASS_PARTIAL_SPEC)

      template <class _Iterator2, class _Category2, class _TypeU,
                class _Reference2, class _Pointer2, class _Distance2>
      reverse_iterator (const reverse_iterator<_Iterator2, _Category2, _TypeU,
                        _Reference2, _Pointer2, _Distance2>& __rhs)
          : current (__rhs.base ()) { }

#  endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC 
#endif  // _RWSTD_NO_MEMBER_TEMPLATES


    iterator_type base () const {
        return current;
    }

    reference operator* () const {
        iterator_type __tmp = base ();
        return *--__tmp;
    }

    _RWSTD_OPERATOR_ARROW (pointer operator->() const);

    reverse_iterator& operator++ () {
        return --current, *this;
    }

    reverse_iterator operator++ (int) {
        reverse_iterator __tmp = *this;
        ++*this;
        return __tmp;
    }

    reverse_iterator& operator-- () {
        return ++current, *this;
    }

    reverse_iterator operator-- (int) {
        reverse_iterator __tmp = *this;
        --*this;
        return __tmp;
    }

    reverse_iterator& operator+= (difference_type __n) {
        return current -= __n, *this;
    }

    reverse_iterator& operator-= (difference_type __n) {
        return *this += -__n;
    }

    reverse_iterator operator+ (difference_type __n) const {
        return reverse_iterator (*this) += __n;
    }

    reverse_iterator operator- (difference_type __n) const {
        return reverse_iterator (*this) -= __n;
    }

    reference operator[] (difference_type __n) const {
        return *(*this + __n);
    }

protected:

    iterator_type current;
};


_RWSTD_ITER_TEMPLATE
inline bool operator== (const _RWSTD_ITER_ID (reverse_iterator)& __x,
                        const _RWSTD_ITER_ID (reverse_iterator)& __y)
{
    return __x.base () == __y.base ();
}


_RWSTD_ITER_TEMPLATE
inline bool operator< (const _RWSTD_ITER_ID (reverse_iterator)& __x,
                       const _RWSTD_ITER_ID (reverse_iterator)& __y)
{
    return __y.base() < __x.base();
}


_RWSTD_ITER_TEMPLATE
inline bool operator!= (const _RWSTD_ITER_ID (reverse_iterator)& __x, 
                        const _RWSTD_ITER_ID (reverse_iterator)& __y)
{
    return !(__x == __y);
}


_RWSTD_ITER_TEMPLATE
inline bool operator> (const _RWSTD_ITER_ID (reverse_iterator)& __x, 
                       const _RWSTD_ITER_ID (reverse_iterator)& __y)
{
    return __y < __x;
}


_RWSTD_ITER_TEMPLATE
inline bool operator<= (const _RWSTD_ITER_ID (reverse_iterator)& __x,
                        const _RWSTD_ITER_ID (reverse_iterator)& __y)
{
    return !(__y < __x);
}


_RWSTD_ITER_TEMPLATE
inline bool operator>= (const _RWSTD_ITER_ID (reverse_iterator)& __x, 
                        const _RWSTD_ITER_ID (reverse_iterator)& __y)
{
    return !(__x < __y);
}


_RWSTD_ITER_TEMPLATE
inline _RWSTD_ITER_DIFF_TYPE (_Iterator, _Distance)
operator- (const _RWSTD_ITER_ID (reverse_iterator)& __x, 
           const _RWSTD_ITER_ID (reverse_iterator)& __y)
{
    return __y.base () - __x.base ();
}


_RWSTD_ITER_TEMPLATE
inline _RWSTD_ITER_ID (reverse_iterator)
operator+ (_RWSTD_ITER_DIFF_TYPE (_Iterator, _Distance) __n, 
           const _RWSTD_ITER_ID (reverse_iterator)&     __x)
{
    return __x + __n;
}

#undef _RWSTD_ITER_DIFF_TYPE

#ifdef _RWSTD_NO_CLASS_PARTIAL_SPEC

_RWSTD_NAMESPACE_END   // std

_RWSTD_NAMESPACE_BEGIN (__rw)

// Reverse bidirectional iterator.       
// This is needed to get around non-standard compilers that insist
// on instantiating all members of a class whether they're used 
// or not.

template <class _Iterator, class _Category, class _TypeT, 
          class _Reference _RWSTD_COMPLEX_DEFAULT (_TypeT&), 
          class _Pointer _RWSTD_COMPLEX_DEFAULT (_TypeT*), 
          class _Distance _RWSTD_COMPLEX_DEFAULT (ptrdiff_t) >
class __reverse_bi_iterator
    : public _STD::iterator<_Category, _TypeT, _Distance, _Pointer, _Reference>
{
public:
    typedef _Distance  difference_type;
    typedef _TypeT     value_type;
    typedef _Reference reference;
    typedef _Pointer   pointer;
    typedef _Iterator  iterator_type;

    __reverse_bi_iterator () { }

    _EXPLICIT __reverse_bi_iterator (const iterator_type &__rhs) 
        : current (__rhs) { }


#ifndef _RWSTD_NO_MEMBER_TEMPLATES
#  ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC 

      template <class _TypeU>
      __reverse_bi_iterator (const __reverse_bi_iterator<_TypeU>& __rhs)
          : current (__rhs.base ()) { }

#  else   // if defined (_RWSTD_NO_CLASS_PARTIAL_SPEC )

      template <class _Iterator2, class _Category2, class _TypeU,
                class _Reference2, class _Pointer2, class _Distance2>
      __reverse_bi_iterator (const __reverse_bi_iterator<_Iterator2,
                                                         _Category2,
                                                         _TypeU,
                                                         _Reference2,
                                                         _Pointer2,
                                                         _Distance2>& __rhs)
          : current (__rhs.base ()) { }
#  endif
#endif // _RWSTD_NO_MEMBER_TEMPLATES


    iterator_type base () const {
        return current;
    }

    reference operator* () const {
        iterator_type __tmp = base ();
        return *--__tmp;
    }

    _RWSTD_OPERATOR_ARROW (pointer operator->() const);

    __reverse_bi_iterator& operator++ () {
        return --current, *this;
    }

    __reverse_bi_iterator  operator++ (int) {
        __reverse_bi_iterator __tmp (*this);
        ++*this;
        return __tmp;
    }

    __reverse_bi_iterator& operator-- () {
        return ++current, *this;
    }

    __reverse_bi_iterator operator-- (int) {
        __reverse_bi_iterator __tmp (*this);
        --*this;
        return __tmp;
    }

protected:

    iterator_type current;
};


_RWSTD_ITER_TEMPLATE
inline bool operator== (const _RWSTD_ITER_ID (__reverse_bi_iterator)& __x, 
                        const _RWSTD_ITER_ID (__reverse_bi_iterator)& __y)
{
    return __x.base () == __y.base ();
}


_RWSTD_ITER_TEMPLATE
inline bool operator!= (const _RWSTD_ITER_ID (__reverse_bi_iterator)& __x,
                        const _RWSTD_ITER_ID (__reverse_bi_iterator)& __y)
{
    return !(__x == __y);
}

#undef _RWSTD_ITER_TEMPLATE
#undef _RWSTD_ITER_ID

_RWSTD_NAMESPACE_END   // __rw

_RWSTD_NAMESPACE_BEGIN (std)

#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC


#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC
#  define _RWSTD_INSERT_ITERATOR_BASE(ignore)       \
          iterator<output_iterator_tag, void, void, void, void>
#else
   // necessary to allow __iterator_category, __value_type, etc. to work
#  define _RWSTD_INSERT_ITERATOR_BASE(cont)         \
          iterator<output_iterator_tag,             \
                   _TYPENAME cont::value_type,      \
                   _TYPENAME cont::difference_type, \
                   _TYPENAME cont::pointer,         \
                   _TYPENAME cont::reference>
#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC


template <class _Container>
class back_insert_iterator: public _RWSTD_INSERT_ITERATOR_BASE (_Container)
{
public:
    typedef _Container container_type;

    _EXPLICIT back_insert_iterator (container_type& __rhs)
        : container (&__rhs) { }

    back_insert_iterator&
    operator= (_TYPENAME container_type::const_reference __x) {
        return container->push_back (__x), *this;
    }

    back_insert_iterator& operator*  () {
        return *this;
    }

    back_insert_iterator& operator++ () {
        return *this;
    }

    back_insert_iterator operator++ (int) {
        return *this;
    }

protected:

    container_type* container;
};


template <class _Container>
inline back_insert_iterator<_Container> back_inserter (_Container& __x)
{
    return back_insert_iterator<_Container>(__x);
}


template <class _Container>
class front_insert_iterator: public _RWSTD_INSERT_ITERATOR_BASE (_Container)
{
public:
    typedef _Container container_type;

    _EXPLICIT front_insert_iterator (container_type& __rhs)
        : container (&__rhs) { }

    front_insert_iterator&
    operator= (_TYPENAME container_type::const_reference __x) { 
        return container->push_front (__x), *this;
    }

    front_insert_iterator& operator* () {
        return *this;
    }

    front_insert_iterator& operator++ () {
        return *this;
    }

    front_insert_iterator operator++ (int) {
        return *this;
    }

protected:

    container_type* container;
};


template <class _Container>
inline front_insert_iterator<_Container> front_inserter (_Container& __x)
{
    return front_insert_iterator<_Container>(__x);
}


template <class _Container>
class insert_iterator: public _RWSTD_INSERT_ITERATOR_BASE (_Container)
{
public:
    typedef _Container container_type;

    insert_iterator (container_type                    &__x,
                     _TYPENAME container_type::iterator __it)
      : iter (__it), container (&__x) { }

    insert_iterator&
    operator= (_TYPENAME container_type::const_reference __x) { 
        iter = container->insert (iter, __x);
        return ++iter, *this;
    }

    insert_iterator& operator* () {
        return *this;
    }

    insert_iterator& operator++ () {
        return *this;
    }

    insert_iterator& operator++ (int) {
        return *this;
    }

protected:

    _TYPENAME container_type::iterator iter;
    container_type*                    container;
};


template <class _Container, class _Iterator>
inline insert_iterator<_Container> inserter (_Container& __x, _Iterator __it)
{
    typedef _TYPENAME _Container::iterator _Iter;

    return insert_iterator<_Container> (__x, _Iter (__it));
}


_RWSTD_NAMESPACE_END   // std

#endif   // _RWSTD_ITERATOR_H_INCLUDED
