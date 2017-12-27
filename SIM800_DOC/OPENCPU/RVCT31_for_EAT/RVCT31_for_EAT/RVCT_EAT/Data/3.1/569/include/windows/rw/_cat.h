/***************************************************************************
 *
 * _cat.h - Declarations for XPG4 messages
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _cat.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_CAT_H_INCLUDED
#define _RWSTD_CAT_H_INCLUDED

#include <map>
#include <string>

#include <rw/_defs.h>

#include _RWSTD_CSTDDEF
#include <rw/_locvector.h>

#if defined (_WIN32) || defined (_WIN64)
#  define _RWSTD_USE_RC 1
#  include <windows.h>
#endif

#ifndef LC_MESSAGES
#  ifdef LC_MAX
#    define LC_MESSAGES (LC_MAX + 1)
#  else
     // take a guess...
#    define LC_MESSAGES 6
#  endif
#endif

#define NL_SETD                 1    /* XPG3 Conformant Default set number. */
#define NL_CAT_LOCALE           (-1) /* XPG4 requirement */
#define NL_CAT_LOCALE           (-1) /* XPG4 requirement */

#define _RWCAT_COOKIE            0xAB34BC56


_RWSTD_NAMESPACE_BEGIN (__rw)


#ifndef _RWSTD_USE_RC
// NULL implementation (does nothing)
class __rw_cat
{
public:
    __rw_cat(const char*, int) { }

    ~__rw_cat() { }

    __rw_cat* id () {
        return this;
    }

    bool good () const {
        return false;
    }

    const char* catgets (int, int, const char *__s) {
        return __s;
    }
};

#else   // if defined (_RWSTD_USE_RC)

#define NL_TEXTMAX 8024 

// Catalog type hold relationship between cat id and file
class __rw_cat
{
public:
    typedef _STD::map<int, _STD::string, _STD::less<int>,
                      _STD::allocator<_STD::pair<const int, _STD::string> > >
    __cat_type;

    typedef __cat_type::value_type __msg_type;
    
protected:
    HINSTANCE __cat;
    __cat_type __msgs;

public:
    __rw_cat (const char *__fname, int)
        : __cat (__fname ? LoadLibrary (__fname) : GetModuleHandle (0)) { }

    ~__rw_cat () {
        if (__cat)
            FreeLibrary (__cat);
    }

    __rw_cat* id () {
        return this;
    }

    bool good () {
        return !!__cat;
    }

    HINSTANCE cat () {
        return __cat;
    }

    const char* catgets (int, int, const char*);
};


#endif   //_RWSTD_USE_RC


_RWSTD_NAMESPACE_END   // __rw


typedef _RW::__rw_cat* nl_catd;
typedef int            nl_item;

int         catclose (nl_catd);
const char* catgets (nl_catd, int, int, const char*);
nl_catd     catopen (const char*, int);


#endif  // _RWSTD_CAT_H_INCLUDED
