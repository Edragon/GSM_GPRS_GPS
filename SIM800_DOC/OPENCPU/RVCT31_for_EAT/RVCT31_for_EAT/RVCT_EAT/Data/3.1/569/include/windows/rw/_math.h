/***************************************************************************
 *
 * _math.h - Standard Library vs math.h exception conflict hack.
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _math.h 91721 2005-08-08 18:30:07Z drodgman $
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
 ***************************************************************************/

#ifndef _RWSTD_MATH_H_INCLUDED
#define _RWSTD_MATH_H_INCLUDED

#include <rw/_defs.h>

// avoid a conflicting exception structure on platforms where
// struct exception is defined unguarded in <math.h>
#ifndef _RWSTD_NO_MATH_EXCEPTION
#  undef exception
#  define exception math_exception
#endif   // _RWSTD_NO_MATH_EXCEPTION

#include _RWSTD_CMATH

#undef exception

// MSVC provides its own complex macro
#ifdef _MSC_VER
#  ifdef complex
#    undef complex
#  endif
#endif


#endif   // _RWSTD_MATH_H_INCLUDED
