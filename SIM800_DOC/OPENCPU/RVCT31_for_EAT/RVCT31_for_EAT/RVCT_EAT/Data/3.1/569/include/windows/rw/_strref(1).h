/***************************************************************************
 *
 * _strref.h - Declarations for the Standard Library string_ref classes
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _strref.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_STRREF_H_INCLUDED
#define _RWSTD_STRREF_H_INCLUDED

#include <memory>
#include <rw/_mutex.h>
#include <rw/_traits.h>
#include <rw/_defs.h>


_RWSTD_NAMESPACE_BEGIN (std)


// chooses either a single global mutex or a mutex per string object
// or no mutex at all when atomic test-and-set instruction is available
#if !defined (_RWSTD_MULTI_THREAD) || !defined (_RWSTD_NO_TEST_AND_SET)
#  define _RWSTD_STRING_MUTEX(ignore)   false
#elif defined (_RWSTD_ONE_STRING_MUTEX)
#  define _RWSTD_STRING_MUTEX(ignore)   __rw_string_mutex
#else
#  define _RWSTD_STRING_MUTEX(pstr)     pstr->_C_mutex
#endif


template< class _CharT,
          class _Traits _RWSTD_COMPLEX_DEFAULT (char_traits<_CharT>), 
          class _Allocator _RWSTD_COMPLEX_DEFAULT (allocator<_CharT>) >
class basic_string;


_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)


#ifdef _RWSTD_NO_COLLAPSE_TEMPLATE_STATICS

extern unsigned long _RWSTD_EXPORT __nullref [];
#define _NULLREF _RW::__nullref

#else

#define _NULLREF string_type::__nullref

#endif   // _RWSTD_NO_COLLAPSE_TEMPLATE_STATICS


#if    defined (_RWSTD_MULTI_THREAD)    \
    && defined (_RWSTD_ONE_STRING_MUTEX) \
    && defined (_RWSTD_NO_TEST_AND_SET)

extern  __rw_mutex _RWSTD_EXPORT __rw_string_mutex;

#endif   // _MULTI_THREAD && _ONE_STRING_MUTEX && _NO_TEST_AND_SET


template <class _CharT, class _Traits , class _Allocator>
struct __string_ref
{
    typedef _CharT                                            char_type;
    typedef _Allocator                                        allocator_type;
    typedef _TYPENAME allocator_type::size_type               size_type;
    typedef _STD::basic_string<_CharT, _Traits, _Allocator>   string_type;

#if     defined (_RWSTD_MULTI_THREAD)          \
    && !defined (_RWSTD_ONE_STRING_MUTEX)      \
    &&  defined (_RWSTD_NO_TEST_AND_SET)       \
    && !defined (_RWSTD_NO_STATIC_MUTEX_INIT)

    void _C_init (size_type __ref, size_type __cap, size_type __size) {

        _C_cap  = __cap;
        _C_size = __size;

#  ifndef _RWSTD_NO_STRING_REF_COUNT

        _C_refs = __ref - 1;

        if (0 != _RWSTD_MUTEX_INIT (this->_C_mutex._C_mutex))
            _RW::__rw_throw (_RWSTD_ERROR_RUNTIME_ERROR,
                             "synchronization error");
#  else   // if defined (_RWSTD_NO_STRING_REF_COUNT)
        _RWSTD_UNUSED (__ref);
#  endif   // _RWSTD_NO_STRING_REF_COUNT

    }

    void _C_destroy () {
#  ifndef _RWSTD_NO_STRING_REF_COUNT

        _RWSTD_MUTEX_DESTROY (this->_C_mutex._C_mutex);

#  endif   // _RWSTD_NO_STRING_REF_COUNT
    }

#else

    void _C_init (size_type __ref, size_type __cap, size_type __size) {

#  ifndef _RWSTD_NO_STRING_REF_COUNT
        _C_refs = __ref - 1;
#  else
        _RWSTD_UNUSED (__ref);
#  endif   // _RWSTD_NO_STRING_REF_COUNT

        _C_cap  = __cap;
        _C_size = __size;
    }

    void _C_destroy () { }

#endif   // _MULTI_THREAD && !_ONE_STRING_MUTEX && _NO_TEST_AND_SET && ...

    size_type _C_ref_count () const {

#ifndef _RWSTD_NO_STRING_REF_COUNT
        return this->_C_refs + 1;
#else   // if defined (_RWSTD_NO_STRING_REF_COUNT)
        return 0;
#endif   // _RWSTD_NO_STRING_REF_COUNT
    }

    void _C_set_ref_count (size_type __ref) { 

#ifndef _RWSTD_NO_STRING_REF_COUNT

        if (this != (void*)&_NULLREF)
            _RWSTD_ATOMIC_SWAP (this->_C_refs, __ref - 1,
                                _RWSTD_STRING_MUTEX (this));
#else   // if defined (_RWSTD_NO_STRING_REF_COUNT)
        _RWSTD_UNUSED (__ref);
#endif   // _RWSTD_NO_STRING_REF_COUNT

    }

    void _C_inc_ref () { 

#ifndef _RWSTD_NO_STRING_REF_COUNT

        if (this != (void*)&_NULLREF)
            _RWSTD_ATOMIC_PREINCREMENT (this->_C_refs,
                                        _RWSTD_STRING_MUTEX (this));
#endif   // _RWSTD_NO_STRING_REF_COUNT

    }

    size_type _C_dec_ref () {

#ifndef _RWSTD_NO_STRING_REF_COUNT

        if (this != (void*)&_NULLREF)
            return 1 + _RWSTD_ATOMIC_PREDECREMENT (this->_C_refs,
                                                   _RWSTD_STRING_MUTEX (this));
        return 1;
#else
        return 0;
#endif   // _RWSTD_NO_STRING_REF_COUNT

    }

    size_type size () const {
        return this->_C_size;
    }

    size_type capacity () const {
        return this->_C_cap;
    }

    char_type* data () {
        return _RWSTD_REINTERPRET_CAST (char_type*, this + 1);
    }

    const char_type* data () const {
        return _RWSTD_REINTERPRET_CAST (const char_type*, this + 1);
    }

#ifndef _RWSTD_NO_STRING_REF_COUNT

#  if     defined (_RWSTD_MULTI_THREAD)     \
      && !defined (_RWSTD_ONE_STRING_MUTEX) \
      &&  defined (_RWSTD_NO_TEST_AND_SET)

    __rw_mutex_base _C_mutex;

#  endif   // _MULTI_THREAD && !_ONE_STRING_MUTEX && _NO_TEST_AND_SET

    long      _C_refs;      // (1 less than) number of references
                            // (-1 if reference counting is disabled)

#endif   // _RWSTD_NO_STRING_REF_COUNT

    size_type _C_cap;       // Size of allocated memory

    union {
        size_type _C_size;  // number of actual data values stored
        _CharT    _C_dummy; // force the alignment of the first char
    };
};


// representation of the null string; will be a POD wherever possible
// (will not be POD if the reference contains a mutex with a UD-ctor)
template <class _CharT, class _Traits , class _Allocator>
struct __null_ref
    : __string_ref<_CharT, _Traits , _Allocator>
{
    // string reference immediately followed by a single terminating null
    _CharT _C_eos;
};


_RWSTD_NAMESPACE_END   // __rw


#endif   // _RWSTD_STRREF_H_INCLUDED
