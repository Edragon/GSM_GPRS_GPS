/***************************************************************************
 *
 * _defs.h - Common macro definitions to be included by every library file
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _defs.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_DEFS_H_INCLUDED
#define _RWSTD_DEFS_H_INCLUDED

#include <rw/_config.h>


// provide definitions for C++ versions of standard C headers
// if a header is not available its name is #defined to the name of
// this file (it's harmless and avoids polluting code with
// tons of ugly platform-specific #ifdefs)
#ifndef _RWSTD_NO_NEW_HEADER

#  define _RWSTD_CASSERT   <cassert>
#  define _RWSTD_CCTYPE    <cctype>
#  define _RWSTD_CERRNO    <cerrno>
#  define _RWSTD_CFLOAT    <cfloat>
#  define _RWSTD_CISO646   <ciso646>
#  define _RWSTD_CLIMITS   <climits>
#  define _RWSTD_CLOCALE   <clocale>
#  define _RWSTD_CMATH     <cmath>
#  define _RWSTD_CSETJMP   <csetjmp>
#  define _RWSTD_CSIGNAL   <csignal>
#  define _RWSTD_CSTDARG   <cstdarg>
#  define _RWSTD_CSTDDEF   <cstddef>
#  define _RWSTD_CSTDIO    <cstdio>
#  define _RWSTD_CSTDLIB   <cstdlib>
#  define _RWSTD_CSTRING   <cstring>
#  define _RWSTD_CTIME     <ctime>

#  ifndef _RWSTD_NO_WCHAR_H
#    define _RWSTD_CWCHAR  <cwchar>
#  else
#    define _RWSTD_CWCHAR  <rw/_defs.h>
#  endif   // _RWSTD_NO_WCHAR_H

#  ifndef _RWSTD_NO_WCTYPE_H
#    define _RWSTD_CWCTYPE <cwctype>
#  else
#    define _RWSTD_CWCTYPE <rw/_defs.h>
#  endif   // _RWSTD_NO_WCTYPE_H

#else   // if defined (_RWSTD_NO_NEW_HEADER)

#  define _RWSTD_CASSERT   <assert.h>
#  define _RWSTD_CCTYPE    <ctype.h>
#  define _RWSTD_CERRNO    <errno.h>
#  define _RWSTD_CFLOAT    <float.h>
#  define _RWSTD_CISO646   <iso646.h>
#  define _RWSTD_CLIMITS   <limits.h>
#  define _RWSTD_CLOCALE   <locale.h>
#  define _RWSTD_CMATH     <math.h>
#  define _RWSTD_CSETJMP   <setjmp.h>
#  define _RWSTD_CSIGNAL   <signal.h>
#  define _RWSTD_CSTDARG   <stdarg.h>
#  define _RWSTD_CSTDDEF   <stddef.h>
#  define _RWSTD_CSTDIO    <stdio.h>
#  define _RWSTD_CSTDLIB   <stdlib.h>
#  define _RWSTD_CSTRING   <string.h>
#  define _RWSTD_CTIME     <time.h>

#  ifndef _RWSTD_NO_WCHAR_H
#    define _RWSTD_CWCHAR  <wchar.h>
#  else
#    define _RWSTD_CWCHAR  <rw/_defs.h>
#  endif   // _RWSTD_NO_WCHAR_H

#  ifndef _RWSTD_NO_WCTYPE_H
#    define _RWSTD_CWCTYPE <wctype.h>
#  else
#    define _RWSTD_CWCTYPE <rw/_defs.h>
#  endif   // _RWSTD_NO_WCTYPE_H

#endif   // _RWSTD_NO_NEW_HEADER


#if defined(_WIN64)
#  define _RWSTD_SYS_TYPES_H <basetsd.h>
#elif defined(_WIN32)
#  define _RWSTD_SYS_TYPES_H <rw/_defs.h>
#else
#  define _RWSTD_SYS_TYPES_H <sys/types.h>
#endif   // _WIN32


#ifdef _RWSTD_REQUIRES_IEEEFP
// non-standard file required on Some platforms (e.g., Siemens) for <limits>
#  define _RWSTD_IEEEFP <ieeefp.h>
#else
#  define _RWSTD_IEEEFP <rw/_defs.h>
#endif


// provide default #definitions of ANSI C library headers
#if !defined (_RWSTD_NO_ASSERT_H) && !defined (_RWSTD_ANSI_C_ASSERT_H)
#  define _RWSTD_ANSI_C_ASSERT_H "/usr/include/assert.h"
#endif
#if !defined (_RWSTD_NO_CTYPE_H) && !defined (_RWSTD_ANSI_C_CTYPE_H)
#  define _RWSTD_ANSI_C_CTYPE_H "/usr/include/ctype.h"
#endif
#if !defined (_RWSTD_NO_ERRNO_H) && !defined (_RWSTD_ANSI_C_ERRNO_H)
#  define _RWSTD_ANSI_C_ERRNO_H "/usr/include/errno.h"
#endif
#if !defined (_RWSTD_NO_FLOAT_H) && !defined (_RWSTD_ANSI_C_FLOAT_H)
#  define _RWSTD_ANSI_C_FLOAT_H "/usr/include/float.h"
#endif
#if !defined (_RWSTD_NO_ISO646_H) && !defined (_RWSTD_ANSI_C_ISO646_H)
#  define _RWSTD_ANSI_C_ISO646_H "/usr/include/iso646.h"
#endif
#if !defined (_RWSTD_NO_LIMITS_H) && !defined (_RWSTD_ANSI_C_LIMITS_H)
#  define _RWSTD_ANSI_C_LIMITS_H "/usr/include/limits.h"
#endif
#if !defined (_RWSTD_NO_LOCALE_H) && !defined (_RWSTD_ANSI_C_LOCALE_H)
#  define _RWSTD_ANSI_C_LOCALE_H "/usr/include/locale.h"
#endif
#if !defined (_RWSTD_NO_MATH_H) && !defined (_RWSTD_ANSI_C_MATH_H)
#  define _RWSTD_ANSI_C_MATH_H "/usr/include/math.h"
#endif
#if !defined (_RWSTD_NO_SETJMP_H) && !defined (_RWSTD_ANSI_C_SETJMP_H)
#  define _RWSTD_ANSI_C_SETJMP_H "/usr/include/setjmp.h"
#endif
#if !defined (_RWSTD_NO_SIGNAL_H) && !defined (_RWSTD_ANSI_C_SIGNAL_H)
#  define _RWSTD_ANSI_C_SIGNAL_H "/usr/include/signal.h"
#endif
#if !defined (_RWSTD_NO_STDARG_H) && !defined (_RWSTD_ANSI_C_STDARG_H)
#  define _RWSTD_ANSI_C_STDARG_H "/usr/include/stdarg.h"
#endif
#if !defined (_RWSTD_NO_STDDEF_H) && !defined (_RWSTD_ANSI_C_STDDEF_H)
#  define _RWSTD_ANSI_C_STDDEF_H "/usr/include/stddef.h"
#endif
#if !defined (_RWSTD_NO_STDIO_H) && !defined (_RWSTD_ANSI_C_STDIO_H)
#  define _RWSTD_ANSI_C_STDIO_H "/usr/include/stdio.h"
#endif
#if !defined (_RWSTD_NO_STDLIB_H) && !defined (_RWSTD_ANSI_C_STDLIB_H)
#  define _RWSTD_ANSI_C_STDLIB_H "/usr/include/stdlib.h"
#endif
#if !defined (_RWSTD_NO_STRING_H) && !defined (_RWSTD_ANSI_C_STRING_H)
#  define _RWSTD_ANSI_C_STRING_H "/usr/include/string.h"
#endif
#if !defined (_RWSTD_NO_TIME_H) && !defined (_RWSTD_ANSI_C_TIME_H)
#  define _RWSTD_ANSI_C_TIME_H "/usr/include/time.h"
#endif
#if !defined (_RWSTD_NO_WCHAR_H) && !defined (_RWSTD_ANSI_C_WCHAR_H)
#  define _RWSTD_ANSI_C_WCHAR_H "/usr/include/wchar.h"
#endif
#if !defined (_RWSTD_NO_WCTYPE_H) && !defined (_RWSTD_ANSI_C_WCTYPE_H)
#  define _RWSTD_ANSI_C_WCTYPE_H "/usr/include/wctype.h"
#endif


// Macro for determining the number of bits in a word.
// Used by vector<bool>.
//
#define _RWSTD_WORD_BIT (int(CHAR_BIT*sizeof(unsigned int)))


//
// These macros tune the speed vs. space optimization of container
// performance with regard to allocation of memory. These have been tuned
// with speed efficiency as a primary conern. Space efficiency was
// considered, but was secondary. (refer to Onyx QA Entry #4920)
//
// The ratio parameter must be above one for an amortized constant time
// algorithm.  Lowering the ratio will lower rapidity and improve space
// efficiency. This effect will be most noticable when working with
// containers of few elements (few being less than 32 for the general case)
// If your use case is with containers that typically have many elements
// lowering the ratio will have less effect on rapidity.
//
// The minimum elements parameter refers to minimum amount of additional
// capcity requested (in number of elements) when more memory is needed.
// Rapidity with containers of few elements will be affected most noticably
// as this amount this is lowered.
//
// For strings, the specializations on char and wchar_t were seen as an
// exploitable common case, and have their own more specialized template
// function overloads in the string header.  These are tweakable here
// with the STRING version of these macros.
//
#if !defined(_RWSTD_MINIMUM_NEW_CAPACITY)
#  define _RWSTD_MINIMUM_NEW_CAPACITY _RWSTD_C::size_t (32)
#endif
#if !defined(_RWSTD_NEW_CAPACITY_RATIO)
   // using long doubles to eliminate bogus warnings on g++ 2.95.2/sparc
   // (-W -O2/3 only): warning: overflow on truncation to integer
   //#  define _RWSTD_NEW_CAPACITY_RATIO 1.618L
#  define _RWSTD_INCREASE_CAPACITY(x) ((x) + ((x) >> 1) + ((x) >> 3)) // 'x * 1.625'
#endif
#if !defined(_RWSTD_MINIMUM_STRING_CAPACITY)
#  define _RWSTD_MINIMUM_STRING_CAPACITY _RWSTD_C::size_t (128)
#endif
#if !defined(_RWSTD_STRING_CAPACITY_RATIO)
   //#  define _RWSTD_STRING_CAPACITY_RATIO 1.618L
#  define _RWSTD_INCREASE_STRING_CAPACITY(x) _RWSTD_INCREASE_CAPACITY(x)
#endif


// set up Win32/64 DLL export/import directives
// _DLL - defined by the compiler when either -MD or -MDd is used
// RWDLL - defined for all Rogue Wave products to be built as shared libs
// _RWSHARED - defined for stdlib to be built/used as a shared lib
#if    (defined (_WIN32) || defined (_WIN64)) \
    && (defined (RWDLL) || defined (_RWSHARED))
#  ifdef _RWBUILD_std
     // building a shared lib, export names
#    define _RWSTD_EXPORT   __declspec (dllexport)
#  else
     // using a shared lib, import names
#    define _RWSTD_EXPORT   __declspec (dllimport)
#  endif

   // disable warnings:
   // C4251: class needs to have dll-interface to be used by cliens
   // C4275: non dll-interface class used as base for dll-interface class
#  pragma warning (disable: 4251)
#  pragma warning (disable: 4275)
#else
// disable Windows hacks
#  define _RWSTD_EXPORT
#endif


// The member variable string::npos is required to be static to allow its
// use in constant expressions.  For compilers that do not support static
// intialization an enumeration is used.  This workaround is not available
// on LLP64 architectures however, where enums are 32 bit.  In this case it
// we don't have a workaround to support the use of npos in constant
// expressions, the macro _RWSTD_NPOS must be used instead
#if defined(_RWSTD_LLP64_ARCHITECTURE) && defined(_RWSTD_NO_STATIC_CONST_MEMBER_INIT)
#  define _RWSTD_NPOS _RWSTD_C::size_t(-1)
#else
#  define _RWSTD_NPOS string::npos
#endif


/*
** Miscellaneous workarounds.
*/

#ifdef _RWSTD_NO_BOOL
#  ifdef _RWSTD_MSVC_BOOL_WARNING
#    pragma warning ( disable : 4237 )
#  endif

   typedef int     bool;

#  ifndef true
#    define true    1
#  endif

#  ifndef false
#    define false   0
#  endif
#endif // _RWSTD_NO_BOOL


#ifndef _RWSTD_NO_TYPENAME
#  define _TYPENAME typename
#else
#  define _TYPENAME
#endif

#if defined (SNI) || defined (__SUNPRO_CC) && __SUNPRO_CC <= 0x520
#  define _TYPENAME_CTOR
#else
#  define _TYPENAME_CTOR _TYPENAME
#endif

#ifndef _RWSTD_NO_EXPLICIT
#  define _EXPLICIT explicit
#else
#  define _EXPLICIT
#endif


#ifndef _RWSTD_NO_MUTABLE
#  define _MUTABLE mutable
#else
#  define _MUTABLE
#endif


//
// Macro for forming or omitting default template arguments in constructors
//

#ifndef _RWSTD_NO_DEFAULT_TEMPLATE_ARGS
#  define _RWSTD_DEFAULT_ARG(n) = n
#else
#  define _RWSTD_DEFAULT_ARG(n)
#endif


// MSVC version 12.00.xxxx (and perhaps prior) can't parse template
// re-declarations if the previous declaration contains two or more
// default template parameters (go figure)
#if defined (_MSC_VER) && _MSC_VER <= 1300
#  define _RWSTD_REDECLARED_DEFAULT(arg)   = arg
#else   // if !(!defined (_MSC_VER) || (_MSC_VER <= 1300))
#  define _RWSTD_REDECLARED_DEFAULT(ignore)
#endif   // !defined (_MSC_VER) || (_MSC_VER <= 1300)


//
// Macro for forming or ommitting default template parameters.
//
#ifndef _RWSTD_NO_SIMPLE_DEFAULT_TEMPLATES
#  define _RWSTD_SIMPLE_DEFAULT(a)  = a
#  ifndef _RWSTD_NO_COMPLEX_DEFAULT_TEMPLATES
#    define _RWSTD_COMPLEX_DEFAULT(a)  = a
#  else
#    define _RWSTD_COMPLEX_DEFAULT(a)
#  endif
#else   // if defined (_RWSTD_NO_SIMPLE_DEFAULT_TEMPLATES)
#  ifndef _RWSTD_NO_COMPLEX_DEFAULT_TEMPLATES
#    define _RWSTD_NO_COMPLEX_DEFAULT_TEMPLATES
#  endif
#  define _RWSTD_SIMPLE_DEFAULT(a)
#  define _RWSTD_COMPLEX_DEFAULT(a)
#  ifndef _RWSTD_NO_DEFAULT_TEMPLATES
#    define _RWSTD_NO_DEFAULT_TEMPLATES
#  endif
#endif   // _RWSTD_NO_SIMPLE_DEFAULT_TEMPLATES


// Macro for casting, using either the "old" method
// or the new C++ cast system
#ifdef _RWSTD_NO_STATIC_CAST
#  define _RWSTD_STATIC_CAST(x, y)      ((x)y)
#  define _RWSTD_REINTERPRET_CAST(x, y) ((x)y)
#  define _RWSTD_CONST_CAST(x, y)       ((x)y)
#else
#  define _RWSTD_STATIC_CAST(x, y)      static_cast< x >(y)
#  define _RWSTD_REINTERPRET_CAST(x, y) reinterpret_cast< x >(y)
#  define _RWSTD_CONST_CAST(x, y)       const_cast< x >(y)
#endif   // _RWSTD_NO_STATIC_CAST

//
// Macros for the new template specialization syntax
//

#ifdef _RWSTD_NO_NEW_FUNC_TEMPLATE_SYNTAX
#  define _RWSTD_SPECIALIZED_FUNCTION
#else
#  define _RWSTD_SPECIALIZED_FUNCTION template<>
#endif

#ifdef _RWSTD_NO_NEW_CLASS_TEMPLATE_SYNTAX
#  define _RWSTD_SPECIALIZED_CLASS
#else
#  define _RWSTD_SPECIALIZED_CLASS template<>
#endif

// for compilers or environments that do not support namespaces
#ifndef _RWSTD_NO_NAMESPACE
#  define _RWSTD_NAMESPACE_BEGIN(name)    namespace name {
#  define _RWSTD_NAMESPACE_END            }

#  define _USING(name)                    using name

#  ifdef _MSC_VER
#    define _STD                       ::std
#    define _STD_RW                    ::std
#    define _RW                        ::__rw
#  else
#    define _STD                       std
#    define _STD_RW                    std
#    define _RW                        __rw
#  endif

#else   // if defined (_RWSTD_NO_NAMESPACE)
#  define _RWSTD_NAMESPACE_BEGIN(ignore)
#  define _RWSTD_NAMESPACE_END

#  define _USING(ignore)              typedef void __rw_unused_typedef

#  define _STD
#  define _STD_RW
#  define _RW
#endif   // _RWSTD_NO_NAMESPACE

//
// for compilers that don't like specialized friends
// such as operator== in template classes
//
#ifdef _RWSTD_NO_SPECIALIZED_FRIEND
#  ifdef __TURBOC__
#    define _RWSTD_SPECIALIZED_FRIEND(name)   (_STD_RW::name)
#  else   // if !defined (__TURBOC__)
#    define _RWSTD_SPECIALIZED_FRIEND(name)   name
#  endif   // __TURBOC__
#else   // if defined (_RWSTD_NO_SPECIALIZED_FRIEND)
#  ifdef __TURBOC__
#    define _RWSTD_SPECIALIZED_FRIEND(name)   (_STD_RW::name)
#  else   // if !defined (__TURBOC__)
     // the space between `name' and `<>' is necessary
     // to prevent ..._FRIEND (operator<) from expanding into operator<<>
     // (only the GNU preprocessor inserts a space)
#    define _RWSTD_SPECIALIZED_FRIEND(name)   name <>
#  endif  // __TURBOC__
#endif   // _RWSTD_NO_SPECIALIZED_FRIEND


//
// If compiler supports member and default templates then it support
// the _RWSTD_ALLLOCATOR
//
#if    !defined(_RWSTD_NO_MEMBER_TEMPLATES) \
    && !defined(_RWSTD_NO_SIMPLE_DEFAULT_TEMPLATES) \
    && !defined(_RWSTD_NO_MEM_CLASS_TEMPLATES)
#  define _RWSTD_ALLOCATOR
#endif


#define _RWSTD_MB_CUR_MAX 16  //???  Need to find absolute maximum for this


// helper macros for token pasting
#define _RWSTD_DO_PASTE(a, b)   a ## b
#define _RWSTD_PASTE(a, b)      _RWSTD_DO_PASTE (a, b)

#ifdef _RWSTD_MULTI_THREAD
// synchronizes access by all objects holding the same mutex
#  define _RWSTD_MT_GUARD(mutex)  \
          _RW::__rw_guard _RWSTD_PASTE (__guard, __LINE__) (mutex)

// synchronizes access by all threads
#  define _RWSTD_MT_STATIC_GUARD(name)        \
          static _RW::__rw_mutex name; \
          _RWSTD_MT_GUARD (name)

// synchronizes access by all objects of the same type
#  define _RWSTD_MT_CLASS_GUARD(name, type) \
          _RWSTD_MT_GUARD (_RW::__rw_get_static_mutex ((type*)0))

#  define _RWSTD_ATOMIC_PREINCREMENT(x, mutex) \
          _RW::__rw_atomic_preincrement (x, mutex)

#  define _RWSTD_ATOMIC_PREDECREMENT(x, mutex) \
          _RW::__rw_atomic_predecrement (x, mutex)

#  define _RWSTD_ATOMIC_SWAP(x, y, mutex) \
          _RW::__rw_atomic_exchange  (x, y, mutex)

#  ifndef _RWSTD_NO_EXT_REENTRANT_IO
// conditional swap used in iostreams
#    define _RWSTD_ATOMIC_IO_SWAP(x, y, mutex)           \
            ((this->flags () & _RWSTD_IOS_NOLOCK) ?      \
                _RW::__rw_ordinary_exchange  (x, y)      \
             :  _RW::__rw_atomic_exchange  (x, y, mutex))

#  else
#    define _RWSTD_ATOMIC_IO_SWAP(x, y, mutex)           \
            _RWSTD_ATOMIC_SWAP(x, y, mutex)
#  endif    // _RWSTD_NO_EXT_REENTRANT_IO

#else   // if !defined _RWSTD_MULTI_THREAD

#  define _RWSTD_MT_GUARD(ignore)                   ((void)0)
#  define _RWSTD_MT_STATIC_GUARD(name)              _RWSTD_MT_GUARD (name)
#  define _RWSTD_MT_CLASS_GUARD(name, ignore)       _RWSTD_MT_GUARD (name)

#  define _RWSTD_ATOMIC_PREINCREMENT(x, ignore)     ++(x)
#  define _RWSTD_ATOMIC_PREDECREMENT(x, ignore)     --(x)
#  define _RWSTD_ATOMIC_SWAP(x, y, ignore)  \
          _RW::__rw_atomic_exchange ((x), (y), false)

#  define _RWSTD_ATOMIC_IO_SWAP(x, y, ignore)  \
          _RW::__rw_ordinary_exchange ((x), (y))

#endif   // _RWSTD_MULTI_THREAD


// macro for qualifying C library names in the std namespace
#ifndef _RWSTD_NO_LIBC_IN_STD
# define _RWSTD_C std
#else
# define _RWSTD_C
#endif

#define _RWSTD_INVALID_FILE_DESC -1
#define _RWSTD_INVALID_FILE_PTR   0

#ifndef _RWSTD_NO_NATIVE_IO
# define _RWSTD_INVALID_FILE     -1
# define _RWSTD_INVALID_OPENMODE -1
#else
# define _RWSTD_INVALID_FILE      (FILE*)0
# define _RWSTD_INVALID_OPENMODE  0
#endif // _RWSTD_NO_NATIVE_IO


// std::ios_base static const data members' values
#define _RWSTD_IOS_BOOLALPHA    0x00001
#define _RWSTD_IOS_DEC          0x00002
#define _RWSTD_IOS_FIXED        0x00004
#define _RWSTD_IOS_HEX          0x00008
#define _RWSTD_IOS_INTERNAL     0x00010
#define _RWSTD_IOS_LEFT         0x00020
#define _RWSTD_IOS_OCT          0x00040
#define _RWSTD_IOS_RIGHT        0x00080
#define _RWSTD_IOS_SCIENTIFIC   0x00100
#define _RWSTD_IOS_SHOWBASE     0x00200
#define _RWSTD_IOS_SHOWPOINT    0x00400
#define _RWSTD_IOS_SHOWPOS      0x00800
#define _RWSTD_IOS_SKIPWS       0x01000
#define _RWSTD_IOS_UNITBUF      0x02000
#define _RWSTD_IOS_UPPERCASE    0x04000
#define _RWSTD_IOS_BIN          0x08000
// NOLOCK[BUF] used rather than LOCK[BUF] to maintain functional compatibility
// with release 2.2 (which was always safe when these bits were cleared)
#define _RWSTD_IOS_NOLOCK       0x10000
#define _RWSTD_IOS_NOLOCKBUF    0x20000

#define _RWSTD_IOS_FLOATFIELD   _RWSTD_IOS_FIXED | _RWSTD_IOS_SCIENTIFIC

#define _RWSTD_IOS_ADJUSTFIELD    \
        (_RWSTD_IOS_LEFT | _RWSTD_IOS_RIGHT | _RWSTD_IOS_INTERNAL)

#ifndef _RWSTD_NO_EXT_BIN_IO
#  define _RWSTD_IOS_BASEFIELD    \
          (_RWSTD_IOS_DEC | _RWSTD_IOS_HEX | _RWSTD_IOS_OCT | _RWSTD_IOS_BIN)
#else
#  define _RWSTD_IOS_BASEFIELD    \
          (_RWSTD_IOS_DEC | _RWSTD_IOS_HEX | _RWSTD_IOS_OCT)
#endif   // _RWSTD_NO_EXT_BIN_IO


#define _RWSTD_IOS_GOODBIT       0x00
#define _RWSTD_IOS_BADBIT        0x01
#define _RWSTD_IOS_EOFBIT        0x02
#define _RWSTD_IOS_FAILBIT       0x04

#define _RWSTD_IOS_APP           0x01
#define _RWSTD_IOS_BINARY        0x02
#define _RWSTD_IOS_IN            0x04
#define _RWSTD_IOS_OUT           0x08
#define _RWSTD_IOS_TRUNC         0x10
#define _RWSTD_IOS_ATE           0x20

#ifndef _RWSTD_STRICT_ANSI
#  define _RWSTD_IOS_NOCREATE    0x40
#  define _RWSTD_IOS_NOREPLACE   0x80
#else
#  define _RWSTD_IOS_NOCREATE    0
#  define _RWSTD_IOS_NOREPLACE   0
#endif

#define _RWSTD_IOS_MASK                         \
        (  _RWSTD_IOS_APP | _RWSTD_IOS_BINARY   \
         | _RWSTD_IOS_IN | _RWSTD_IOS_OUT       \
         | _RWSTD_IOS_TRUNC | _RWSTD_IOS_ATE    \
         | _RWSTD_IOS_NOCREATE | _RWSTD_IOS_NOREPLACE)


#if    !defined (_RWSTD_NO_STATIC_IOSTREAM_INIT)  \
    && !defined (_RWSTD_NO_IOSTREAM_OBJECT_REFS)
   // standard iostream objects are references
#  define _RWSTD_IOSTREAM_OBJECT(name)   &name
#else
   // standard iostream objects are declared to be real objects
   // they may be implemented as real objects of the same type
   // that get destroyed during program lifetime or as objects
   // of some POD type that do not get destroyed at all
#  define _RWSTD_IOSTREAM_OBJECT(name)   name
#endif   //  _RWSTD_NO_STATIC_IOSTREAM_INIT && !_RWSTD_NO_IOSTREAM_OBJECT_REFS


// exceptions can be completely disabled (bodies of catch blocks are still
// compiled but will be removed by the optimizing stage of the compiler)
// this approach reveals any errors even if _RWSTD_NO_EXCEPTIONS is #defined
#ifndef _RWSTD_NO_EXCEPTIONS
#  define _TRY           try
#  define _CATCH(ex)     catch (ex)
#  define _CATCH_T(ex)   catch (ex)
#  define _THROW(ex)     throw ex
#  define _RETHROW       throw
#else   // if defined (_RWSTD_NO_EXCEPTIONS)
#  define _TRY
#  define _CATCH(ignore)   while (0)
#  define _CATCH_T(type)   for (type; 0;) 
#  define _THROW(ignore)   ((void)0)
#  define _RETHROW         ((void)0)
#endif   // _RWSTD_NO_EXCEPTIONS


// for compilers that can't handle standard allocators we provide four
// workarounds: _RWSTD_REBIND, used in place of _Allocator::rebind, allows
// containers to use the same allocator for different data types
// _RWSTD_ALLOC_TYPE is required to "rebind", or rather wrap,  the user
// supplied non-conforming allocator in allocator_interface so that the
// container need not be aware of the allocator's special properties (namely
// the fact that the allocator allocates in terms of bytes rather than
// elemements)
//
// _RWSTD_VALUE_ALLOC creates a temporary allocator_interface wrapper
// from the given parameter (usually *this) when allocating values in
// the sequence containers with non-conforming allocators.
// _RWSTD_VALUE_ALLOC_CAST casts the given container to a reference to
// a value allocator, allowing access to inherited allocator members
// outside of the class.  For non-conforming compilers, this macro is
// equivalent to _RWSTD_VALUE_ALLOC.
//
// NOTE: the spaces around type below are necessary to prevent errors
//       if `to' is a template-id and the preprocessor doesn't put
//       the spaces there automatically (GNU does, Sun does not)
#ifdef _RWSTD_ALLOCATOR
#  define _RWSTD_REBIND(from, to) \
          _TYPENAME from::template rebind < to >::other
#  define _RWSTD_ALLOC_TYPE(_Allocator, _ValueType)   _Allocator
#  define _RWSTD_VALUE_ALLOC(ignore, call)            allocator_type::call
#  define _RWSTD_VALUE_ALLOC_CAST(rvalue) \
          _RWSTD_STATIC_CAST (allocator_type&, rvalue)
#else   // if !defined (_RWSTD_ALLOCATOR)
#  define _RWSTD_REBIND(from, to) \
          _STD_RW::allocator_interface < from, to >
#  define _RWSTD_ALLOC_TYPE(_Allocator, _ValueType)   \
          _RWSTD_REBIND (_Allocator, _ValueType)
#  define _RWSTD_VALUE_ALLOC(type, call)      type(*this).call
#  define _RWSTD_VALUE_ALLOC_CAST(rvalue)     _C_value_alloc_type(rvalue)
#endif   // _RWSTD_ALLOCATOR


// for compilers that can't deal with template functions parametrized
// on the function's return type (as opposed to function arguments)
//
// NOTE: the spaces around type below are necessary to prevent errors
//       if `type' is a template-id and the preprocessor doesn't put
//       the spaces there automatically (GNU does, Sun does not)
#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
#  define _STD_USE_FACET(type, arg)    _STD::use_facet < type >(arg)
#  define _USE_FACET(type, arg)        use_facet < type >(arg)
#  define _GET_TEMP_BUFFER(type, size) get_temporary_buffer < type >(size)
#else   // if defined (_RWSTD_NO_TEMPLATE_ON_RETURN_TYPE)
#  define _STD_USE_FACET(type, arg)    _STD::use_facet (arg, (type*)(0))
#  define _USE_FACET(type, arg)        use_facet (arg, (type*)(0))
#  define _GET_TEMP_BUFFER(type, size) get_temporary_buffer (size, (type*)0)
#endif   // _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE


#ifndef _RWSTD_NO_NONCLASS_ARROW_RETURN
   // expected signature: "_TypeT* operator->()"
   // and/or:             "const _TypeT* operator->() const"
#  define _RWSTD_OPERATOR_ARROW(signature) signature { return &**this; }
#else
#  define _RWSTD_OPERATOR_ARROW(ignore)
#endif   // _RWSTD_NO_NONCLASS_ARROW_RETURN


// to prevent warnings about unused arguments
#define _RWSTD_UNUSED(arg)   ((void)&arg)


// allows all externs to be declared/defined only in headers
// and defined just by first #defining _RWSTD_DEFINE_EXTERNS
// and then #including the header in a .cpp file
#ifndef _RWSTD_DEFINE_EXTERNS
#  define _RWSTD_EXTERN(decl, ignore)   extern decl
#else
#  define _RWSTD_EXTERN(decl, value)    extern decl = value
#endif   // _RWSTD_DEFINE_EXTERNS


// allows static const integral or enum class members to be initialized
// inside the class and defined (w/o being initialized) outside
// of the class (as required by 9.4.2, p4)
#if !defined(_RWSTD_NO_STATIC_CONST_MEMBER_INIT)
#  define _RWSTD_STATIC_CONST(type, init)    static const type init
#  define _RWSTD_DEFINE_STATIC_CONST(decl)   decl
#else
#  define _RWSTD_STATIC_CONST(ignore, init)  enum { init }
#  define _RWSTD_DEFINE_STATIC_CONST(ignore)
#endif   // _RWSTD_NO_STATIC_CONST_MEMBER_INIT


// Selection of vendor dependent "format" string argument to wcsftime()
#ifndef _RWSTD_NO_WCSFTIME
   // 7.24.5.1 of C99 - wcsftime()'s 3rd arg is const wchar* [restrict]
   //                   HP-UX 11 (and perhaps others) still uses const char*
#  ifndef _RWSTD_NO_WCSFTIME_WCHAR_T_FMAT
#     define _RWSTD_FMT_TYPE(arg)  const wchar_t* arg
#     define _RWSTD_FMT_ASSGN(str) L##str
#  else
#     define _RWSTD_FMT_TYPE(arg)  const char* arg
#     define _RWSTD_FMT_ASSGN(str) str
#  endif
#else   // if defined (_RWSTD_NO_WCSFTIME)
#  define _RWSTD_FMT_TYPE(arg)     const char* arg
#  define _RWSTD_FMT_ASSGN(str)    str
#endif // _RWSTD_NO_WCSFTIME


// _Iterator typedefs
// (note that you must use a semi-colon at the end of this macro)
#define _RWSTD_ITERATOR_TYPES(_Iterator)                               \
     typedef _TYPENAME _Iterator::difference_type difference_type;     \
     typedef _TYPENAME _Iterator::value_type value_type;               \
     typedef _TYPENAME _Iterator::pointer pointer;                     \
     typedef _TYPENAME _Iterator::reference reference;                 \
     typedef _TYPENAME _Iterator::iterator_category iterator_category

// helpers making working w/o iterator_traits transparent
#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC

#  define _RWSTD_VALUE_TYPE(iterT) \
       (_TYPENAME _STD::iterator_traits< iterT >::value_type*)0
#  define _RWSTD_DIFFERENCE_TYPE(iterT)   \
       (_TYPENAME _STD::iterator_traits< iterT >::difference_type*)0
#  define _RWSTD_ITERATOR_CATEGORY(iterT, ignore) \
       _TYPENAME_CTOR _STD::iterator_traits< iterT >::iterator_category ()

#else   // if defined (_RWSTD_NO_CLASS_PARTIAL_SPEC)

#  define _RWSTD_VALUE_TYPE(iterT) __value_type ((iterT*)0)
   // assume default ctor is defined and accessible
#  define _RWSTD_DIFFERENCE_TYPE(iterT)   \
       _STD::__distance_type (iterT ())
   // macro cannot use the default ctor to construct a temporary
   // with the type of the first argument since the default ctor
   // may not exist or be accessible (e.g., istream_iterator<>)
#  define _RWSTD_ITERATOR_CATEGORY(ignore, iter) \
       _STD::__iterator_category (iter)

#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC

// Use -D_RWSTDDEBUG to compile a version of the libraries to debug
// the user's code.  This will perform pre- and post-condition checks
// upon entering routines, but will be larger and run more slowly.
// This option affects the binary compatibility of generated code.


#if defined(RWDEBUG) && !defined(_RWSTDDEBUG)
#  define _RWSTDDEBUG 1
#endif


#if defined(_RWSTDDEBUG) && _RWSTDDEBUG
#  define _RWSTD_ASSERT(expr)                                                \
     ((expr) ? (void)0 : _RW::__rw_assert_fail (#expr, __MODULE__, __LINE__, \
                                                __PRETTY_FUNCTION__))
#else   //  if !defined (_RWSTDDEBUG)
#  define _RWSTD_ASSERT(ignore)         ((void)0)
#endif   // _RWSTDDEBUG

// compile-time assertion - asserts constant expressions during
// compilation with no runtime overhead; failed assertions are reported
// as compilation errors

_RWSTD_NAMESPACE_BEGIN (__rw)

template <bool __b>
struct __rw_compile_assert;

_RWSTD_SPECIALIZED_CLASS
struct __rw_compile_assert<true> { enum { _C_ok }; };

#define _RWSTD_COMPILE_ASSERT(const_expr) \
        ((void)_RW::__rw_compile_assert<(const_expr)>::_C_ok)

// called for failed assertions
void _RWSTD_EXPORT
__rw_assert_fail (const char *__expr,
                       const char *__file, int __line, const char *__func);

_RWSTD_NAMESPACE_END   // __rw

#  define _RWSTD_STR(x)       #x
#  define _RWSTD_STRSTR(x)    _RWSTD_STR(x)

#if defined(_RWSTDDEBUG) && _RWSTDDEBUG
#  define _RWSTD_FUNC(ignore) _RWSTD_FILE_LINE, __PRETTY_FUNCTION__
#  define _RWSTD_FILE_LINE   __MODULE__ ":" _RWSTD_STRSTR (__LINE__)
#else
#  define _RWSTD_FUNC(ignore) _RWSTD_FILE_LINE, ""
#  define _RWSTD_FILE_LINE   ""
#endif

/* at least some of the _RWSTD_REQUIRES are needed for conformance */
#  define _RWSTD_REQUIRES(pred, args)   (pred) ? (void)0 : _RW::__rw_throw args

// function exception specification
#if    !defined (_RWSTD_NO_EXCEPTIONS) \
    && !defined (_RWSTD_NO_EXCEPTION_SPECIFICATION)
   // type_id_list is a possibly empty parenthesized list
   //of comma-separated type-id's
#  define _THROWS(type_id_list)   throw type_id_list
#else   // if _RWSTD_NO_EXCEPTIONS || _RWSTD_NO_EXCEPTION_SPECIFICATION
#  define _THROWS(ignore)
#endif   // !_RWSTD_NO_EXCEPTIONS && !_RWSTD_NO_EXCEPTION_SPECIFICATION


// for convenience
#ifndef _RWSTD_NO_CLASS_PARTIAL_SPEC
#  define _DISTANCE(first, last, ignore) _STD_RW::distance (first, last)
#else
#  define _DISTANCE(first, last, type) _RW::__rw_distance (first, last, type())
#endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC


_RWSTD_NAMESPACE_BEGIN (__rw)

// pointers to the incomplete types declared below are used
// to disambiguate calls to template member functions
// bodies provided only to work around an HP aCC 3.14.10 bug
template <bool __b>
struct __rw_select { };

_RWSTD_SPECIALIZED_CLASS
struct __rw_select<true> { };

#define _RWSTD_DISPATCH_INT(yes) const _RW::__rw_select < yes >*
#define _RWSTD_DISPATCH(iter) \
        ((_RW::__rw_select<numeric_limits< iter >::is_integer>*)0)

_RWSTD_NAMESPACE_END   // __rw


#if defined (__IBMCPP__) && __IBMCPP <= 500

// working around an xlC 5.0 bug
_USING (__rw::__rw_select);

#endif


// select appropriate C multibyte conversion function based on
// whether "restartable" functions are available.


#ifndef _RWSTD_NO_MBRTOWC
   // reentrant versions return size_t - see 7.24.6.3.2 of C99
#  define _RWSTD_MBTOWC(to, from, size, state) mbrtowc (to, from, size, state)
#elif !defined (_RWSTD_NO_MBTOWC)
   // non-reentrant versions return int - see 7.20.7.2 of C99
#  define _RWSTD_MBTOWC(to, from, size, ignore_state) \
          _RWSTD_C::size_t (mbtowc(to, from, size))
#else
   // mbtowc not supported
#  define _RWSTD_MBTOWC(ign1, ign2, ign3, ign4) _RWSTD_C::size_t (-1)
#endif   // _RWSTD_NO_MBRTOWC

#ifndef _RWSTD_NO_WCRTOMB
#  define _RWSTD_WCTOMB(to, from, state)       wcrtomb (to, from, state)
#else   // if defined (_RWSTD_NO_WCRTOMB)
#  define _RWSTD_WCTOMB(to, from, ignore_state) \
          _RWSTD_C::size_t (wctomb (to, from))
#endif   // _RWSTD_NO_WCRTOMB


// unsafe when underlying libc doesn't support mbsinit
// as described in 7.24.6.3.1 of C99
#ifndef _RWSTD_NO_MBSINIT
# if defined(SNI) && defined(_RWSTD_STRICT_ANSI)
#  define _MBSINIT(ps)   __SNI::mbsinit (ps)
# else
#  define _MBSINIT(ps)   mbsinit (ps)
# endif
#else   // if defined (_RWSTD_NO_MBSINIT)
#  define _MBSINIT(ps)   (0 == (ps))
#endif   // _RWSTD_NO_MBSINIT


// enable only in debug mode and only id partial class specialization
// is supported; prevent multiple definition of _RWSTD_NO_DEBUG_ITER
#if    defined(_RWSTDDEBUG) && !defined (_RWSTD_NO_CLASS_PARTIAL_SPEC) \
    && !defined (_RWSTD_NO_DEBUG_ITER) && !defined (SNI)
#  define _RWSTD_ASSERT_RANGE(first, last) \
      _RWSTD_ASSERT (_RW::__rw_valid_range (first, last))
#  define _RWSTD_ASSERT_IN_RANGE(it, first, last) \
      _RWSTD_ASSERT (_RW::__rw_in_range (it, first, last))
#  define _RWSTD_ASSERT_DEREF(it) \
      _RWSTD_ASSERT (_RW::__rw_dereferenceable (it))
#else
#  ifndef _RWSTD_NO_DEBUG_ITER
#    define  _RWSTD_NO_DEBUG_ITER
#  endif   // _RWSTD_NO_DEBUG_ITER

#  define _RWSTD_ASSERT_RANGE(ign1, ign2)          ((void)0)
#  define _RWSTD_ASSERT_IN_RANGE(ign1, ign2, ign3) ((void)0)
#  define _RWSTD_ASSERT_DEREF(ignore)              ((void)0)
#endif   // _RWSTDDEBUG && !_RWSTD_NO_CLASS_PARTIAL_SPEC


// convenience macro to get the "real" iterator embedded in __rw_debug_iter<>
#ifndef _RWSTD_NO_DEBUG_ITER
#  define _ITER_BASE(it)   ((it).base ())
#else   // if defined (_RWSTD_NO_DEBUG_ITER)
#  define _ITER_BASE(it)   (it)
#endif   // _RWSTD_NO_DEBUG_ITER


// will use the deduced native type rather than the typedef to prevent
// conflicts caused by inconsistent definition of wint_t (gcc 2.95.2/AIX)
#ifndef _RWSTD_WINT_T
   // hardcode if wint_t is not available in <wchar.h>
   // used by std::traits and std::basic_[i|o]stream
#  define _RWSTD_WINT_T int
#endif   // _RWSTD_WINT_T


// used by std::traits and std::basic_[i|o]stream,
// required to be int by 21.1.3.1, p2
#define _RWSTD_INT_T int


// ssize_t is defined by POSIX.1
#ifndef _RWSTD_SSIZE_T
#  ifdef _WIN64
#    define _RWSTD_SSIZE_T SSIZE_T
#  else
#    define _RWSTD_SSIZE_T long
#  endif
#endif


#if defined(SNI)
#  define _RWSTD_USING_SNI(name) _USING(__SNI::name)
#  if defined(_RWSTD_STRICT_ANSI)
#    define fileno(p)       (int) ((p)->_file)
#  endif
#else
#  define _RWSTD_USING_SNI(ignore)    typedef void __rw_unused_typedef
#endif


#ifndef _RWSTD_TMPBUF_SIZE
#  define _RWSTD_TMPBUF_SIZE 4096
#endif

#ifndef _RWSTD_NO_STRTOLL
#  define _STRTOLL           strtoll
#else
   // libc has no strtoll, use strtol instead
#  define _STRTOLL           (_RWSTD_LONG_LONG)strtol
#endif   // _RWSTD_NO_STRTOLL


#ifndef _RWSTD_NO_STRTOULL
#  define _STRTOULL          strtoull
#else
   // libc has no strtoull, use strtoul instead
#  define _STRTOULL          (unsigned _RWSTD_LONG_LONG)strtoul
#endif   // _RWSTD_NO_STRTOULL


#if     defined (_RWSTD_INSTANTIATE_TEMPLATES)       \
    && !defined (_RWSTD_NO_EXPLICIT_INSTANTIATION)   \
    && !defined (_RWSTD_NO_INSTANTIATE)
#  define _RWSTD_INSTANTIATE_1(arg)          template arg
#  define _RWSTD_INSTANTIATE_2(a1, a2)       template a1, a2
#  define _RWSTD_INSTANTIATE_3(a1, a2, a3)   template a1, a2, a3
#else
#  if    defined (_MSC_VER)                          \
      && !defined (_RWSTD_NO_EXPLICIT_INSTANTIATION) \
      && !defined (_RWSTD_NO_INSTANTIATE)
     // disable warning C4231: nonstandard extension used :
     //         'extern' before template explicit instantiation
#    pragma warning (disable: 4231)
#    define _RWSTD_INSTANTIATE_1(arg)        extern template arg
#    define _RWSTD_INSTANTIATE_2(a1, a2)     extern template a1, a2
#    define _RWSTD_INSTANTIATE_3(a1, a2, a3) extern template a1, a2, a3
#  else
#    define _RWSTD_INSTANTIATE_1(ignore)           \
            typedef void __rw_unused_typedef
#    define _RWSTD_INSTANTIATE_2(ign1, ign2)       \
            typedef void __rw_unused_typedef
#    define _RWSTD_INSTANTIATE_3(ign1, ign2, ign3) \
            typedef void __rw_unused_typedef
#  endif
#endif   // _RWSTD_INSTANTIATE_TEMPLATES


#ifndef _RWSTD_NO_UNCAUGHT_EXCEPTION
#  define _UNCAUGHT_EXCEPTION()   uncaught_exception()
#else   // if defined (_RWSTD_NO_UNCAUGHT_EXCEPTION)
#  define _UNCAUGHT_EXCEPTION()   true
#endif   // _RWSTD_NO_UNCAUGHT_EXCEPTION

// allows for efficient compilation without implicit inclusion; only
// specializations explicitly instantiated in the library are available
#if     defined _RWSTD_COMPILE_INSTANTIATE               \
    && (   !defined (_RWSTD_NO_TEMPLATE_DEFINITIONS)     \
        ||  defined (_RWSTD_NO_EXPLICIT_INSTANTIATION)   \
        ||  defined (_RWSTD_NO_IMPLICIT_INSTANTIATION)   \
        ||  defined (_RWSTD_NO_INSTANTIATE))
#  define _RWSTD_DEFINE_TEMPLATE(name)     !(_RWSTD_NO_ ## name ## _DEFINITION)
#else
#  define _RWSTD_DEFINE_TEMPLATE(ignore)   0
#endif


// introduce namespace std so that "using namespace std;" always works
_RWSTD_NAMESPACE_BEGIN (std)
_RWSTD_NAMESPACE_END   // std

#ifndef _RWSTD_NO_NAMESPACE
#ifdef __EDG_IMPLICIT_USING_STD 
/* Implicitly include a using directive for the STD namespace when this */ 
/* preprocessing flag is TRUE. */ 
using namespace ::std; 
#endif /* ifdef __EDG_IMPLICIT_USING_STD */ 
#endif /* _RWSTD_NO_NAMESPACE */

#endif   // _RWSTD_DEFS_H_INCLUDED
