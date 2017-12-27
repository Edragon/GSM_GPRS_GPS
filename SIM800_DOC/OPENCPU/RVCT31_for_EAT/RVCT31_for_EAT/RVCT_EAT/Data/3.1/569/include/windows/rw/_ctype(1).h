/***************************************************************************
 *
 * _ctype.h - Definitions of the character classification facets
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _ctype.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_CTYPE_H_INCLUDED
#define _RWSTD_CTYPE_H_INCLUDED

#include <rw/_locale.h>


_RWSTD_NAMESPACE_BEGIN (std)

// 22.2.1
struct _RWSTD_EXPORT ctype_base
{
    enum mask {
        space  = 0x0001,
        print  = 0x0002,
        cntrl  = 0x0004,
        upper  = 0x0008,
        lower  = 0x0010,
        alpha  = 0x0020,
        digit  = 0x0040,
        punct  = 0x0080,
        xdigit = 0x0100,
        alnum  = alpha | digit,
        graph  = alnum | punct
    };
};


_RWSTD_NAMESPACE_END   // std

_RWSTD_NAMESPACE_BEGIN (__rw)

template <class _CharT>
class __rw_ctype_helper
{
#ifndef _MSC_VER
    friend class __rw_digit_map<_CharT>;
#else
public:
#endif
    __rw_digit_map<_CharT> _C_digit_map;
};

_RWSTD_NAMESPACE_END   // __rw


_RWSTD_NAMESPACE_BEGIN (std)

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT ctype_byname<char>;


_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT ctype<char>
    : public locale::facet, public ctype_base,
      public _RW::__rw_ctype_helper<char>
{
    friend class _RWSTD_EXPORT locale;
    friend class _RWSTD_EXPORT ctype_byname<char>;

#ifndef _RWSTD_NO_WCHAR_T
    friend class _RWSTD_EXPORT ctype<wchar_t>;
#endif

public:

    typedef char char_type;

    bool is (mask __m, char_type __c) const {
#if UCHAR_MAX <= 255
        return (_C_table [(unsigned char)__c] & __m) != 0;
#else
        return __c < table_size && (_C_table [(unsigned char)__c] & __m) != 0;
#endif
    }

protected:

    // inline virtual functions defined first to allow compilers to
    // inline their definitions when being called from nonvirtuals

    const mask* table () const _THROWS (()) {
        return _C_table;
    }

    static const mask* classic_table () _THROWS (()) {
        return _C_classic_table;
    }

    virtual ~ctype () {
        if (_C_delete_it)
            delete[] _RWSTD_CONST_CAST (mask*, _C_table);
        delete[] _C_to_upper_tab;
        delete[] _C_to_lower_tab;
    }

    inline virtual const char_type*
    do_toupper (char_type*, const char_type*) const;

    inline virtual const char_type*
    do_tolower (char_type*, const char_type*) const;

    virtual char_type do_toupper (char_type __c) const {
        return is (lower, __c) ?
            _C_to_upper_tab [(unsigned char)__c - _C_lower_min] : __c;
    }

    virtual char_type do_tolower (char_type __c) const {
        return is (upper, __c) ?
            _C_to_lower_tab [(unsigned char)__c - _C_upper_min] : __c;
    }

    virtual char_type do_widen (char_type __c) const {
        return __c;
    }

    virtual const char_type*
    do_widen (const char_type* __low, const char_type* __high,
              char_type* __to) const {
        memcpy (__to, __low, __high - __low);
        return __high;
    }

    virtual char_type do_narrow (char_type __c, char_type) const {
        return __c;
    }

    virtual const char_type*
    do_narrow (const char_type* __low, const char_type* __high,
               char_type, char_type* __to) const {
        memcpy (__to, __low, __high - __low);
        return __high;
    }

    virtual void _C_initfacet (const locale&);

public:
    _RWSTD_STATIC_CONST (size_t, table_size = 256);

    _EXPLICIT ctype (const mask* = 0, bool = false, size_t = 0);

    const char_type* is (const char_type*, const char_type*, mask*) const;
    const char_type* scan_is (mask, const char_type*, const char_type*) const;
    const char_type* scan_not (mask, const char_type*, const char_type*) const;

    char_type (toupper)(char_type __c) const {
        return do_toupper (__c);
    }

    char_type (tolower)(char_type __c) const {
        return do_tolower (__c);
    }

    char_type widen (char_type __c) const {
        return do_widen (__c);
    }

    char_type narrow (char_type __c, char_type __dfault) const {
        return do_narrow (__c, __dfault);
    }

    const char_type* (toupper)(char_type* __low, const char_type* __high) const{
        return do_toupper (__low, __high);
    }

    const char_type* (tolower)(char_type* __low, const char_type* __high) const{
        return do_tolower (__low, __high);
    }

    const char_type*
    widen (const char_type* __lo, const char_type* __hi,
           char_type* __to) const {
        return do_widen (__lo, __hi, __to);
    }

    const char_type*
    narrow (const char_type* __lo, const char_type* __hi,
            char_type __dfault, char_type* __to) const {
        return do_narrow (__lo, __hi, __dfault, __to);
    }

    static locale::id id;

#ifndef _RWSTD_NO_EXT_LOCALE

    typedef string string_type;

    // extensions:
    string_type widen (const string &__s) const {
        return __s;
    }
    string narrow (const string_type &__s, char_type) const {
        return __s;
    }
#endif   // _RWSTD_NO_EXT_LOCALE

    // Implementation.
    enum { _C_facet_cat = locale::ctype, _C_ok_implicit = 1 };

private:

    static const mask _C_classic_table[table_size];
    const mask*       _C_table;
    bool              _C_delete_it;
    int               _C_lower_min,
                      _C_lower_max;
    int               _C_upper_min,
                      _C_upper_max;
    char_type*        _C_to_upper_tab;
    char_type*        _C_to_lower_tab;

#ifdef _RWSTD_NO_MEMBER_TEMPLATES

    locale::id& _C_get_id () const {
        return id;
    }

#endif
};


inline const ctype<char>::char_type*
ctype<char>::
is (const char_type* __lo, const char_type* __hi, mask* __vec) const
{
    while (__lo != __hi)
        *__vec++ = _C_table[(unsigned char)*__lo++];
    return __hi;
}


inline const ctype<char>::char_type*
ctype<char>::scan_is (ctype_base::mask __m,
                      const char_type* __lo, const char_type* __hi) const
{
    while (__lo != __hi && !(_C_table[(unsigned char)*__lo] & __m))
        ++__lo;
    return __lo;
}


inline const ctype<char>::char_type*
ctype<char>::scan_not (ctype_base::mask __m,
                       const char_type* __lo, const char_type* __hi) const
{
    while (__lo != __hi && (_C_table[(unsigned char)*__lo] & __m))
        ++__lo;
    return __lo;
}


inline const ctype<char>::char_type*
ctype<char>::do_toupper (char_type* __low, const char_type* __high) const
{
    for ( ; __low<__high; ++__low)
        if (is (lower, *__low))
            *__low = _C_to_upper_tab[(unsigned char)*__low-_C_lower_min];
    return __low;
}


inline const ctype<char>::char_type*
ctype<char>::do_tolower (char_type* __low, const char_type* __high) const
{
    for (; __low < __high; ++__low)
        if (is (upper, *__low))
            *__low = _C_to_lower_tab[(unsigned char)*__low-_C_upper_min];
    return __low;
}



#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT ctype<wchar_t>
    : public locale::facet, public ctype_base,
      public _RW::__rw_ctype_helper<wchar_t>
{
    static bool _C_fits (wchar_t __c) {
        return ((~(wchar_t)UCHAR_MAX) & __c) == 0;
    }

    static mask mask_of (wchar_t __c) {
        return _C_fits (__c) ? ctype<char>::classic_table ()[(unsigned char)__c]
            : (ctype_base::mask) 0;
    }

public:

    typedef wchar_t char_type;

protected:

    // inline virtual functions defined first to allow compilers to
    // inline their definitions when being called from nonvirtuals

    virtual bool do_is (mask __m, char_type __c) const {
        return (mask_of (__c) & __m) != 0;
    }

    inline virtual const char_type*
    do_is (const char_type*, const char_type*, mask*) const;

    inline virtual const char_type*
    do_scan_is (mask, const char_type*, const char_type*) const;

    inline virtual const char_type*
    do_scan_not (mask, const char_type*, const char_type*) const;

    inline virtual const char_type*
    do_toupper (char_type*, const char_type*) const;

    virtual char_type do_toupper (char_type __c) const {
        return (mask_of (__c) & lower) ? (__c &~ 0x20) : __c;
    }

    inline virtual const char_type*
    do_tolower (char_type*, const char_type*) const;

    virtual char_type do_tolower (char_type __c) const {
        return (mask_of (__c) & upper) ? (__c | 0x20) : __c;
    }

    inline virtual const char*
    do_widen (const char*, const char*, char_type*) const;

    virtual char_type do_widen (char __c) const {
        return (char_type)(unsigned char)__c;
    }

    inline virtual const char_type*
    do_narrow (const char_type*, const char_type*, char, char*) const;

    virtual char do_narrow (char_type __c, char ____dfault) const {
        return (_C_fits (__c) ? (char)(unsigned char)__c : ____dfault);
    }

public:

    _EXPLICIT ctype (size_t __ref = 0)
        : locale::facet (__ref, locale::ctype) { }

    bool is (mask __m, char_type __c) const {
        return do_is ( __m, __c);
    }

    const char_type*
    is (const char_type* __lo, const char_type* __hi, mask* __vec) const {
        return do_is (__lo, __hi, __vec);
    }

    const char_type*
    scan_is (mask __m, const char_type* __lo, const char_type* __hi) const {
        return do_scan_is (__m, __lo, __hi);
    }

    const char_type*
    scan_not (mask __m, const char_type* __lo, const char_type* __hi) const {
        return do_scan_not (__m, __lo, __hi);
    }

    char_type (toupper)(char_type __c) const {
        return do_toupper (__c);
    }

    const char_type* (toupper)(char_type* __lo, const char_type* __hi) const {
        return do_toupper (__lo, __hi);
    }

    char_type (tolower)(char_type __c) const {
        return do_tolower (__c);
    }

    const char_type* (tolower)(char_type* __lo, const char_type* __hi) const {
        return do_tolower (__lo, __hi);
    }

    char_type widen (char __c) const {
        return do_widen (__c);
    }

    const char*
    widen (const char* __lo, const char* __hi, char_type* __to) const {
        return do_widen (__lo, __hi, __to);
    }

    char narrow (char_type __c, char __dfault) const {
        return do_narrow (__c, __dfault);
    }

    const char_type*
    narrow (const char_type* __lo, const char_type* __hi, char __dfault,
            char* __to) const {
        return do_narrow (__lo, __hi, __dfault, __to);
    }

    static locale::id id;

#ifndef _RWSTD_NO_EXT_LOCALE

    // extensions:
    typedef wstring string_type;

    string_type widen (const string&) const;

    string narrow (const string_type&, char) const;

#endif   // _RWSTD_NO_EXT_LOCALE

    // Implementation:
    enum { _C_facet_cat = locale::ctype, _C_ok_implicit = 1 };

private:

#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    locale::id &_C_get_id (void) const { return id; }
#endif
};


inline const ctype<wchar_t>::char_type*
ctype<wchar_t>::
do_is (const char_type* __lo, const char_type* __hi, mask* __vec) const
{
    while (__lo != __hi)
        *__vec++ = mask_of (*__lo++);
    return __hi;
}


inline const ctype<wchar_t>::char_type*
ctype<wchar_t>::
do_scan_is (ctype_base::mask __m,
            const char_type* __low, const char_type* __high) const
{
    while (__low != __high && !(mask_of (*__low) & __m))
        ++__low;
    return __low;
}


inline const ctype<wchar_t>::char_type*
ctype<wchar_t>::
do_scan_not ( mask __m, const char_type* __low, const char_type* __high) const
{
    while (__low != __high && (mask_of (*__low) & __m))
        ++__low;
    return __low;
}


inline const ctype<wchar_t>::char_type*
ctype<wchar_t>::do_toupper (char_type* __low, const char_type* __high) const
{
    for (; __low < __high; ++__low)
        if (mask_of (*__low) & lower)
            *__low &=~ 0x20;
    return __low;
}


inline const ctype<wchar_t>::char_type*
ctype<wchar_t>::do_tolower (char_type* __low, const char_type* __high) const
{
    for (; __low < __high; ++__low)
        if ((mask_of (*__low) & upper) != 0) *__low |= 0x20;
    return __low;
}



inline const char*
ctype<wchar_t>::
do_widen (const char* __lo, const char* __hi, char_type* __dest) const
{
    while (__lo != __hi) {
        *__dest = (char_type)(unsigned char)*__lo;
        ++__lo; ++__dest;
    }
    return __hi;
}


inline const ctype<wchar_t>::char_type*
ctype<wchar_t>::do_narrow (const char_type* __lo, const char_type* __hi,
                           char __dfault, char* __dest) const {
    for ( ; __lo != __hi; __lo++)
        *__dest++ = _C_fits (*__lo) ? (char)(unsigned char)*__lo : __dfault;
    return __hi;
}

#ifndef _RWSTD_NO_EXT_LOCALE

inline ctype<wchar_t>::string_type
ctype<wchar_t>::widen (const string &__s) const
{
    string_type __res (__s.size (), 0);
    do_widen (__s.c_str (), __s.c_str () + __s.length (), &*__res.begin ());
    return __res;
}


inline string
ctype<wchar_t>::narrow (const string_type &__s, char __dfault) const
{
    string __res (__s.size (), 0);
    do_narrow (__s.c_str (), __s.c_str () + __s.length (), __dfault,
               &*__res.begin ());
    return __res;
}

#endif //_RWSTD_NO_EXT_LOCALE

#endif // _RWSTD_NO_WCHAR_T


// 22.2.1.1
template <class _CharT>
class ctype : public locale::facet, public ctype_base,
              public _RW::__rw_ctype_helper<_CharT>
{
public:

    typedef _CharT char_type;

protected:

    // inline virtual functions defined first to allow compilers to
    // inline their definitions when being called from nonvirtuals

    // 22.2.1.1.2, p1
    virtual bool do_is (mask, char_type) const {
        return false;
    }

    virtual const char_type*
    do_is (const char_type*, const char_type* __high, mask*) const {
        return __high;
    }

    // 22.2.1.1.2, p3
    virtual const char_type*
    do_scan_is (mask, const char_type*, const char_type* __high) const {
        return __high;
    }

    // 22.2.1.1.2, p5
    virtual const char_type*
    do_scan_not (mask, const char_type*, const char_type* __high) const {
        return __high;
    }

    // 22.2.1.1.2, p7
    virtual char_type do_toupper (char_type __c) const {
        return __c;
    }

    virtual const char_type*
    do_toupper (char_type*, const char_type* __high) const {
        return __high;
    }

    // 22.2.1.1.2, p9
    virtual char_type do_tolower (char_type __c) const {
        return __c;
    }

    virtual const char_type*
    do_tolower (char_type*, const char_type* __high) const {
        return __high;
    }

    // 22.2.1.1.2, p11
    virtual char_type do_widen (char __c) const {
        return char_type (__c);
    }

    virtual const char*
    do_widen (const char* __lo, const char* __hi, char_type* __dest) const {
        while (__lo < __hi)
            *__dest++ = char_type (*__lo++);
        return __hi;
    }

    // 22.2.1.1.2, p13
    virtual char do_narrow (char_type, char __dfault) const {
        return __dfault;
    }

    virtual const char_type*
    do_narrow (const char_type*, const char_type* __hi, char, char*) const {
        return __hi;
    }

public:

    _EXPLICIT ctype (size_t __ref = 0)
        : locale::facet (__ref, locale::ctype) { }


    // 22.2.1.1.1, p1
    bool is (mask __m, char_type __c) const {
        return do_is ( __m, __c);
    }

    const char_type*
    is (const char_type* __low, const char_type* __high, mask* __vec) const {
        return do_is (__low, __high, __vec);
    }

    // 22.2.1.1.1, p2
    const char_type*
    scan_is (mask __m, const char_type* __low, const char_type* __high) const {
        return do_scan_is ( __m, __low, __high);
    }

    // 22.2.1.1.1, p3
    const char_type*
    scan_not (mask __m, const char_type* __low, const char_type* __high) const {
        return do_scan_not ( __m, __low, __high);
    }

    // 22.2.1.1.1, p4
    char_type (toupper) (char_type __c) const {
        return do_toupper (__c);
    }

    const char_type* (toupper)(char_type* __low, const char_type* __high) const{
        return do_toupper (__low, __high);
    }

    // 22.2.1.1.1, p5
    char_type (tolower)(char_type __c) const {
        return do_tolower (__c);
    }

    const char_type* (tolower)(char_type* __low, const char_type* __high) const{
        return do_tolower (__low, __high);
    }

    // 22.2.1.1.1, p6
    char_type  widen (char __c) const {
        return do_widen (__c);
    }

    const char*
    widen (const char* __lo, const char* __hi, char_type* __to) const {
        return do_widen (__lo, __hi, __to);
    }

    // 22.2.1.1.1, p7
    char narrow (char_type __c, char __dfault) const {
        return do_narrow (__c, __dfault);
    }

    const char_type* narrow (const char_type* __lo, const char_type* __hi,
                             char __dfault, char* __to) const {
        return do_narrow (__lo, __hi, __dfault, __to);
    }

    static locale::id id;

#ifndef _RWSTD_NO_EXT_LOCALE

    // extensions:
    typedef
    basic_string<char_type, char_traits<char_type>, allocator<char_type> >
    string_type;

    string_type widen (const string&) const;

    string narrow (const string_type&, char) const;

#endif   // _RWSTD_NO_EXT_LOCALE

    // Implementation:
    enum { _C_facet_cat = locale::ctype, _C_ok_implicit = 1 };

private:

#ifdef _RWSTD_NO_MEMBER_TEMPLATES

    locale::id &_C_get_id () const {
        return id;
    }

#endif   // _RWSTD_NO_MEMBER_TEMPLATES

};


_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT ctype_byname<char>: public ctype<char>
{
public:

    _EXPLICIT ctype_byname (const char*, size_t __ref = 0);

private:

    static const mask *_C_get_mask_table (const char*);
};


#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT ctype_byname<wchar_t>: public ctype<wchar_t>
{
public:

    _EXPLICIT ctype_byname (const char*, size_t __ref = 0)
	: ctype<wchar_t> (__ref) { }
};

#endif   // _RWSTD_NO_WCHAR_T


// 22.2.1.2
template <class _CharT>
class ctype_byname: public ctype<_CharT>
{
public:

    _EXPLICIT ctype_byname (const char*, size_t __ref = 0)
	: ctype<_CharT>(__ref) { }
};

// convenience interfaces: is*(char)
// names parenthesized to avoid clashing with masking macros

template <class _CharT>
inline bool (isspace)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).is (ctype_base::space, __c);
}


template <class _CharT>
inline bool (isprint)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).is (ctype_base::print, __c);
}

template <class _CharT>
inline bool (iscntrl)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).is(ctype_base::cntrl, __c);
}


template <class _CharT>
inline bool (isupper)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).is (ctype_base::upper, __c);
}


template <class _CharT>
inline bool (islower)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).is (ctype_base::lower, __c);
}


template <class _CharT>
inline bool (isalpha)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).is (ctype_base::alpha, __c);
}


template <class _CharT>
inline bool (isdigit)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).is (ctype_base::digit, __c);
}


template <class _CharT>
inline bool (ispunct)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).is(ctype_base::punct, __c);
}


template <class _CharT>
inline bool (isxdigit)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).is (ctype_base::xdigit, __c);
}


template <class _CharT>
inline bool (isalnum)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).is (ctype_base::alnum, __c);
}


template <class _CharT>
inline bool (isgraph)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).is (ctype_base::graph, __c);
}


template <class _CharT>
inline _CharT (toupper)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).toupper (__c);
}


template <class _CharT>
inline _CharT (tolower)(_CharT __c, const locale& __loc)
{
    return _USE_FACET (ctype<_CharT>, __loc).tolower (__c);
}


_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)


inline const __rw_digit_map<char>&
__rw_digit_map<char>::_C_get_digit_map (const _STD::ctype<char>& __ctp)
{
    return __ctp._C_digit_map;
}


_RWSTD_SPECIALIZED_FUNCTION
inline _STD::ctype<char>*
__rw_create_named_facet (_STD::ctype<char>*, const char *__name, size_t __ref)
{
    return new _STD::ctype_byname<char>(__name, __ref);
}


_RWSTD_SPECIALIZED_FUNCTION
inline _STD::ctype<char>*
__rw_create_native_facet (_STD::ctype<char>*)
{
    return new _STD::ctype<char>(0, false, 1);
}


_RWSTD_SPECIALIZED_FUNCTION
inline _STD::ctype<char>*
__rw_create_classic_facet (_STD::ctype<char>*)
{
    return new _STD::ctype<char>(0, false, 0);
}


_RWSTD_NAMESPACE_END   // __rw


#if _RWSTD_DEFINE_TEMPLATE (CTYPE)
#  include <rw/_ctype.cc>
#endif

#endif   // _RWSTD_CTYPE_H_INCLUDED
