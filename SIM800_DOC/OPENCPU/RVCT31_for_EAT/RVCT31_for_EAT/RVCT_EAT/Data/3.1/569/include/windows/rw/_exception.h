/***************************************************************************
 *
 * _exception.h - Declarations for the Standard Library exception class
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _exception.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_EXCEPTION_H_INCLUDED
#define _RWSTD_EXCEPTION_H_INCLUDED

#include <exception>

#include <rw/_defs.h>
#include _RWSTD_CSTDARG   // for va_list, va_start, ...
#include _RWSTD_CSTDDEF   // for size_t


_RWSTD_NAMESPACE_BEGIN (std)


// forward declarations avoid circular dependencies
template <class _CharT, class _Traits, class _Allocator>
class basic_string;

template <class _CharT>
struct char_traits;

template <class _TypeT>
class allocator;


typedef basic_string<char, char_traits<char>, allocator<char> > string;


// base exception class inherited by all other exceptions
class _RWSTD_EXPORT __rw_exception: public exception
{
    char *_C_what;   // description string

public:

    __rw_exception& _C_assign (const char*,
                               size_t __len = ~0UL) _THROWS (());

    __rw_exception& _C_vformat (const char*,
                                va_list) _THROWS (());

    __rw_exception& _C_vformat (int, va_list) _THROWS (());

    __rw_exception () _THROWS (())
        : exception (), _C_what (0) { }

    __rw_exception (const __rw_exception &__rhs) _THROWS (())
        : exception (__rhs), _C_what (0) {
        _C_assign (__rhs.what ());
    }

    _EXPLICIT __rw_exception (const string&) _THROWS (());
    
    _EXPLICIT __rw_exception (const char*) _THROWS (());

    _EXPLICIT __rw_exception (int) _THROWS (());

    virtual ~__rw_exception () _THROWS (()) {
        delete[] _C_what;
    }

    __rw_exception& operator= (const __rw_exception &__rhs) _THROWS (()) {
        return _C_assign (__rhs.what ());
    }

    virtual const char* what () const _THROWS (()) {
        return _C_what;
    }

    // format sprintf-style (appends to an existing string)
    __rw_exception& _C_format (const char*, ...) _THROWS (());

    // same as above, but format is obtained from a catalog via catgets()
    __rw_exception& _C_format (int, ...) _THROWS (());
};


#ifdef _INLINE_VARARGS

_INLINE_VARARGS __rw_exception&
__rw_exception::_C_format (const char *__fmat, ...) _THROWS (())
{
    va_list __arg;
    va_start (__arg, __fmat);

    _C_vformat (__fmat, __arg);

    va_end (__arg);

    return *this;
}


_INLINE_VARARGS __rw_exception&
__rw_exception::_C_format (int __fmat_id, ...) _THROWS (())
{
    va_list __arg;
    va_start (__arg, __fmat_id);

    _C_vformat (__fmat_id, __arg);

    va_end (__arg);

    return *this;
}

#endif   // _INLINE_VARARGS


inline
__rw_exception::__rw_exception (const char* __what) _THROWS (())
: exception (), _C_what (0)
{
    _C_assign (__what);
}


inline
__rw_exception::__rw_exception (int __id)  _THROWS (())
: _C_what (0)
{
    _C_format (__id);
}


_RWSTD_NAMESPACE_END   // std


#endif   // _RWSTD_EXCEPTION_H_INCLUDED
