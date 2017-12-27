/***************************************************************************
 *
 * _dispatch.h - Functions used for method selection in containers
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _dispatch.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_DISPATCH_H_INCLUDED
#define _RWSTD_DISPATCH_H_INCLUDED

#include <rw/_defs.h>


struct _RW_is_integer {};
struct _RW_is_not_integer {};

template <class _TypeT>
struct _RWdispatch {
  typedef _RW_is_not_integer _RWtype;
};

_RWSTD_SPECIALIZED_CLASS
struct _RWdispatch<int> {
  typedef _RW_is_integer _RWtype;
};

_RWSTD_SPECIALIZED_CLASS
   struct _RWdispatch<unsigned int> {
	typedef _RW_is_integer _RWtype;
};

_RWSTD_SPECIALIZED_CLASS
   struct _RWdispatch<long> {
	typedef _RW_is_integer _RWtype;
};

_RWSTD_SPECIALIZED_CLASS
   struct _RWdispatch<unsigned long> {
	typedef _RW_is_integer _RWtype;
};

_RWSTD_SPECIALIZED_CLASS
   struct _RWdispatch<short> {
	typedef _RW_is_integer _RWtype;
};

_RWSTD_SPECIALIZED_CLASS
   struct _RWdispatch<unsigned short> {
	typedef _RW_is_integer _RWtype;
};

_RWSTD_SPECIALIZED_CLASS
   struct _RWdispatch<char> {
	typedef _RW_is_integer _RWtype;
};

_RWSTD_SPECIALIZED_CLASS
   struct _RWdispatch<unsigned char> {
	typedef _RW_is_integer _RWtype;
};

#ifndef _RWSTD_NO_BOOL
_RWSTD_SPECIALIZED_CLASS
   struct _RWdispatch<bool> {
	typedef _RW_is_integer _RWtype;
};
#endif

#ifndef _RWSTD_NO_NATIVE_WCHAR_T
_RWSTD_SPECIALIZED_CLASS
   struct _RWdispatch<wchar_t> {
	typedef _RW_is_integer _RWtype;
};
#endif

#endif   // _RWSTD_DISPATCH_H_INCLUDED
