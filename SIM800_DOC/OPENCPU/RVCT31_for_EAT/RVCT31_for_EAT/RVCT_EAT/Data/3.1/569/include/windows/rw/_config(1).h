/***************************************************************************
 *
 * _config.h - Compiler and C library configuration definitions
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _config.h 91721 2005-08-08 18:30:07Z drodgman $
 *
 ***************************************************************************
 *
 * Copyright (c) 1994-2000 Rogue Wave Software, Inc.  All Rights Reserved.
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

#ifndef _RWSTD_CONFIG_H_INCLUDED
#define _RWSTD_CONFIG_H_INCLUDED

#if defined(_RWSTD_USE_CONFIG) || defined(__ARMCC_VERSION)
#  include <rw/config.h>
#else
#  include <rw/config/rwconfig_std.h>
#endif   // _RWSTD_USE_CONFIG

// Library version number
// 
#define _RWSTD_VER 0x02020300
//                    | | | |
//                   AABBCCDD
//                    | | | |
//                    | | | +--- DD = Point release or "patch" release. 
//                    | | |      Left D-digit is reserved for alpha releases
//                    | | |      and right D-digit for beta releases
//                    | | |      
//                    | | +----- CC = Maintenance number
//                    | +------- BB = Minor Release number
//                    +--------- AA = Major Release number

/*********************************************************************
 *                OVERRIDES FOR CONFIGURATION MACROS                 *
 *********************************************************************/

/********************** ARM armcc **************************************/

/* If implicit includes are switched off, include the file.cc explicitly */
#ifndef __IMPLICIT_INCLUDE
#  define _RWSTD_COMPILE_INSTANTIATE
#endif /* __IMPLICIT_INCLUDE */

/********************** EDG eccp *************************************/

// NOTE: the __EDG__ macro is #defined by most edg-based compilers

#if defined (__EDG__) && !defined (__DECCXX)
#  define _RWSTD_NO_DEPRECATED_C_HEADERS
#  undef _RWSTD_NO_NEW_HEADER
#  undef _RWSTD_NO_LIBC_IN_STD

#  ifdef _RWSTD_MULTI_THREAD
     // std::uncaught_exception() not thread-safe
     // in the vanilla front-end (demo)
#    define _RWSTD_NO_UNCAUGHT_EXCEPTION
#  endif   // _RWSTD_MULTI_THREAD
#endif   // __EDG__

/********************** gcc ******************************************/

/* armcc supports __attribute__ */
#define _RWSTD_GNUC_ATTRIBUTE(attr)   __attribute__ (attr)

/********************** Threads ***************************************/

#ifdef _RWSTD_DCE_THREADS
#  define _RWSTD_NO_STATIC_MUTEX_INIT
#endif   // _RWSTD_DCE_THREADS

/********************** Miscellaneous *********************************/

// g++ cannot inline functions that take a variable number of arguments
// or functions that contain static (local) variables
#if !defined (__GNUG__) || __GNUG__ > 2 || __GNUG_MINOR__ > 96
#  define _INLINE_VARARGS      inline
#  if !defined (__HP_aCC) || __HP_aCC > 012100
     // working around a known aCC 1.21 bug
#    define _INLINE_WITH_STATICS inline
#  endif   // !__HP_aCC || __HP_aCC > 012100
#endif   // !__GNUG__ || __GNUG__ > 2 || __GNUG_MINOR__ > 96

#ifdef _RWSTD_NO_STATIC_TEMPLATE_MEMBER_INIT
   // static mutex initialization depends on the compiler's (and the
   // linker's in gcc's case) ability to correctly handle explicitly
   // initialized static members of class templates
#  ifndef _RWSTD_NO_STATIC_MUTEX_INIT
#    define _RWSTD_NO_STATIC_MUTEX_INIT
#  endif
#endif   // _RWSTD_NO_STATIC_TEMPLATE_MEMBER_INIT

/********************** Environment *********************************/

/*
 * Most (but not all) non-unix systems convert new line to carriage
 * return / line feed on output:
 */
#if defined (__OS2__) || defined (_WIN32) || defined (_WIN64)
#  define _RWSTD_CRLF_CONVENTION
#endif


// define wherever atomic instructions are not provided
// affects the size of string reference (i.e., is binary incompatible)
#if    !defined (__DECCXX)                           \
    && !(defined (__i386__) && defined (__GNUG__))   \
    && !defined (_WIN32) && !defined (_WIN64)
#  ifndef _RWSTD_NO_TEST_AND_SET
#    define _RWSTD_NO_TEST_AND_SET
#  endif   // _RWSTD_NO_TEST_AND_SET
#endif   // !_WIN32 && !(__i386__ && __GNUG__)


#ifdef _RWSTD_NO_WCHAR_T
#  ifndef _RWSTD_NO_NATIVE_WCHAR_T /* may be defined in config.h */
#    define _RWSTD_NO_NATIVE_WCHAR_T 1
#  endif
#endif


#ifndef _RWSTD_NO_STL_SPECIALIZATION
// #ifdef _RWSTD_NO_CLASS_PARTIAL_SPEC
// disable partial specialization for void* of STL sequences
#  define _RWSTD_NO_STL_SPECIALIZATION
// #endif   // _RWSTD_NO_CLASS_PARTIAL_SPEC
#endif   // _RWSTD_NO_STL_SPECIALIZATION

// 
// Macro for path to the ANSI 'C' headers 
// Must be set specifically for each platform when the
// C++ wrappers for 'C' headers are used.
//
#define _RWSTD_ANSIC(x) </usr/include/x>

// define unless already specified
#ifndef _RWSTD_LDBL_PRINTF_PREFIX
#  define _RWSTD_LDBL_PRINTF_PREFIX   "L"
#endif   // _RWSTD_LDBL_PRINTF_PREFIX

#ifndef _RWSTD_LDBL_SCANF_PREFIX
#  define _RWSTD_LDBL_SCANF_PREFIX   "L"
#endif   // _RWSTD_LDBL_SCANF_PREFIX


/********************** Library Option Dependencies *************************/

/*
 * The following macro sets the default size of file stream internal buffers
 */

#ifndef _RWSTD_DEFAULT_BUFSIZE         // specified on command line?
#  define _RWSTD_DEFAULT_BUFSIZE   512
#endif

#ifndef _RWSTD_NO_LONG_LONG
   // Win32/64 #defines _RWSTD_LONG_LONG to __int64
#  ifndef _RWSTD_LONG_LONG
#    define _RWSTD_LONG_LONG long long
#  endif   // _RWSTD_LONG_LONG
#endif   // _RWSTD_NO_LONG_LONG


// disable all extensions in strict ANSI mode
#ifdef _RWSTD_STRICT_ANSI
   // long long is not in ANSI C++ yet (although it is in ANSI C99)
   // but armcc supports __int64 in --strict mode
#  undef  _RWSTD_LONG_LONG
#  define _RWSTD_LONG_LONG __int64
#  define _RWSTD_NO_EXT_FILEBUF
#  define _RWSTD_NO_EXT_VECTOR_BOOL_REF_OPS
#  define _RWSTD_NO_EXT_LOCALE
#  define _RWSTD_NO_EXT_DEEP_STRING_COPY
   // no support for exceptions derived from ios_base::failure
#  define _RWSTD_NO_EXT_FAILURE
   // no support for writing out integral values in base 2
#  define _RWSTD_NO_EXT_BIN_IO
   // no support for optional mt-locking in iostreams
#  define _RWSTD_NO_EXT_REENTRANT_IO
   // allocator<const T> not possible
#  define _RWSTD_NO_EXT_CONST_ALLOCATOR
   // no definitions provided for missing operator new or delete
#  define _RWSTD_NO_EXT_OPERATOR_NEW
   // void std::count<>() not provided (except when necessary)
#  define _RWSTD_NO_EXT_VOID_COUNT
   // void std::distance<>()  not provided
#  define _RWSTD_NO_EXT_VOID_DISTANCE
#endif   // _RWSTD_STRICT_ANSI

   // macros that are enabled in library source files
#ifdef _RWSTD_LIB_SRC
   // don't instantiate templates in library source code
   //  (but see _RWSTD_INSTANTIATE_TEMPLATES macro below) 
#  define _RWSTD_NO_TEMPLATE_DEFINITIONS
   // don't provide definitions of operator new in library 
#  define _RWSTD_NO_EXT_OPERATOR_NEW
#endif  // _RWSTD_LIB_SRC

#if _MSC_VER <= 1300
   // msvc60 expects a definition to be provided for all variants
   // of operator new/delete that are declared. This means that either
   // the operators must be defined - preventing redefinition in user code -
   // or that they must be undeclared - preventing them from being called
   // directly in user code. We have chosen the former option.
#  undef _RWSTD_NO_EXT_OPERATOR_NEW
#endif  // _MSC_VER

#ifdef _RWSTD_INSTANTIATE_TEMPLATES
   // instantiate templates (this macro appears in instance.cpp)
#  undef _RWSTD_NO_TEMPLATE_DEFINITIONS
#endif


#endif   // _RWSTD_CONFIG_H_INCLUDED
