/***************************************************************************
 *
 * _messages.h - Declarations for the Standard Library messaging facet
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _messages.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_MESSAGES_H_INCLUDED
#define _RWSTD_MESSAGES_H_INCLUDED

#include <rw/_locale.h>
#include <rw/_defs.h>


_RWSTD_NAMESPACE_BEGIN (std)

struct _RWSTD_EXPORT messages_base
{
    typedef int catalog;
};


class _RWSTD_EXPORT __rw_messages_base: public messages_base
{
    string _C_loc_name;

protected:

    // We require a LC_MESSAGES name to be fixed at construction time
    // so that subsequent operations can be independent of changes to the
    // C library locale.

    __rw_messages_base (const string &__name)
        : _C_loc_name (__name)
        { }

    catalog _C_cat_open (const string&, const locale&) const;

    const char* _C_get_message (catalog, int, int) const;

    const locale& _C_get_locale (catalog) const;

    void _C_cat_close (catalog) const;

    static string _C_get_locale_name (const char*);
};


// 22.2.7.1
template <class _CharT>
class messages: public locale::facet,
                public __rw_messages_base
{
public:
    typedef _CharT char_type;

    typedef basic_string<char_type, char_traits<char_type>,
                         allocator<char_type> > string_type;

    _EXPLICIT messages (size_t __refs = 0)
        : locale::facet (__refs, locale::messages),
          __rw_messages_base ("C")
        { }


    catalog open (const string& __fun, const locale& __loc) const {
        return do_open (__fun, __loc);
    }

    string_type get (catalog __c, int __set, int __msgid,
                     const string_type& __df) const {
        return do_get (__c, __set, __msgid, __df);
    }

    void close (catalog __c) const {
        do_close (__c);
    }

    static locale::id id;

    // Implementation:
    enum { _C_facet_cat = locale::messages, _C_ok_implicit = 1 };

protected:

    virtual catalog do_open (const string &__name,
                             const locale &__loc) const {
        return _C_cat_open (__name, __loc);
    }

    virtual string_type do_get (catalog, int, int, const string_type&) const;

    virtual void do_close (catalog __c) const {
        _C_cat_close (__c);
    }

    messages (size_t __refs, const string &__name)
        :  locale::facet (__refs, locale::messages),
           __rw_messages_base (__name)
        { }


private:

#ifdef _RWSTD_NO_MEMBER_TEMPLATES

    virtual locale::id &_C_get_id () const {
        return id;
    }

#endif   // _RWSTD_NO_MEMBER_TEMPLATES
};

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT messages<char>);

#ifndef _RWSTD_NO_WCHAR_TYPE

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT messages<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_TYPE


// 22.2.7.2
template <class _CharT>
class messages_byname: public messages<_CharT>
{
public:
    typedef messages_base::catalog                   catalog;
    typedef _TYPENAME messages <_CharT>::string_type string_type;

    _EXPLICIT messages_byname (const char *__name, size_t __refs = 0)
        :  messages<_CharT> (__refs,
                            __rw_messages_base::_C_get_locale_name (__name))
        { }
};


_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)

_USING (_RWSTD_C::size_t);

#ifndef _RWSTD_NO_FUNC_PARTIAL_SPEC

template <class _CharT>
inline _STD::messages<_CharT>*
__rw_create_named_facet (_STD::messages<_CharT>*,
                         const char *__name, size_t __refs)
{
    return new _STD::messages_byname<_CharT> (__name, __refs);
}


#else   // if defined (_RWSTD_NO_FUNC_PARTIAL_SPEC)

_RWSTD_SPECIALIZED_FUNCTION
inline _STD::messages<char>*
__rw_create_named_facet (_STD::messages<char>*,
                         const char *__name, size_t __refs)
{
    return new _STD::messages_byname<char> (__name, __refs);
}


#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_SPECIALIZED_FUNCTION
inline _STD::messages<wchar_t>*
__rw_create_named_facet (_STD::messages<wchar_t>*,
                         const char *__name, size_t __refs)
{
    return new _STD::messages_byname<wchar_t> (__name, __refs);
}

#endif   // _RWSTD_NO_WCHAR_T

#endif   // _RWSTD_NO_FUNC_PARTIAL_SPEC


_RWSTD_NAMESPACE_END   // __rw


#if _RWSTD_DEFINE_TEMPLATE (MESSAGES)
#  include <rw/_messages.cc>
#endif


#endif   // _RWSTD_MESSAGES_H_INCLUDED
