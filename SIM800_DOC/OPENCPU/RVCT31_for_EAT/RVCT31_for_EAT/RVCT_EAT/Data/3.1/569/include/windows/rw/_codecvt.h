/***************************************************************************
 *
 * _codecvt.h - Definitions of the code conversion facets
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _codecvt.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_CODECVT_H_INCLUDED
#define _RWSTD_CODECVT_H_INCLUDED

#include <limits>
#include <rw/_locale.h>
#include <rw/_defs.h>

#include _RWSTD_CSTDLIB


_RWSTD_NAMESPACE_BEGIN (std)

// 22.2.1.5 - Template class codecvt

class codecvt_base
{
public:
    enum result { ok, partial, error, noconv };
};


// primary template - a stub implementation that doesn't perform any conversion
// specializations on user-defined character types or state type can be
// implemented to do something useful

template <class _InternT, class _ExternT, class _StateT>
class codecvt: public locale::facet,
               public codecvt_base
{
public:
    typedef _InternT intern_type;
    typedef _ExternT extern_type;
    typedef _StateT  state_type;

protected:

    // 22.2.1.5.2 - virtual functions

    // inline virtual functions defined first to allow compilers to
    // inline their definitions when being called from nonvirtuals

    // 22.2.1.5.2, p1
    virtual result do_out (state_type&,
                           const intern_type*, const intern_type*,
                           const intern_type*&,
                           extern_type*, extern_type*, extern_type*&) const {
        return noconv;
    }

    virtual result do_in (state_type&, const extern_type*,
                          const extern_type*, const extern_type*&,
                          intern_type*, intern_type*, intern_type*&) const {
        return noconv;
    }

    // 22.2.1.5.2, p5 - stores no characters
    virtual result do_unshift (state_type&, extern_type *__to,
                               extern_type*, extern_type *&__to_next) const {
        __to_next = __to;
        return noconv;
    }

    // 22.2.1.5.2, p7
    virtual int do_encoding () const _THROWS (()) {
        return 1;   // 1 external char converts to a single internal char
    }

    // 22.2.1.5.2, p8
    virtual bool do_always_noconv () const _THROWS (()) {
        return true;   // conversion never necessary
    }

    // 22.2.1.5.2, p9
    virtual int do_length (state_type&, const extern_type* __from,
                           const extern_type *__end, size_t __maxlen) const {
        // will not hold for shift sencodings
        size_t __n = __end - __from;
        return __n < __maxlen ? __n : __maxlen;
    }

    // 22.2.1.5.2, p11
    virtual int do_max_length () const _THROWS (()) {
        // returns the max value do_length (s, from, from_end, 1) can return
        // for any valid range [from, from_end) - see LWG issue 74 (a DR)
        return 1;
    }
    
public:

    _EXPLICIT codecvt (size_t __ref = 0): locale::facet (__ref) { }

    // 22,2,1,5,1, p1
    result out (state_type& __state,
                const intern_type* __from, const intern_type* __from_end,
                const intern_type*& __from_next,
                extern_type* __to, extern_type* __to_limit,
                extern_type*& __to_next) const {
        return do_out(__state,__from,__from_end,__from_next,
                      __to,__to_limit,__to_next);
    }

    // 22,2,1,5,1, p2
    result unshift (state_type& __state, extern_type* __to,
                    extern_type* __to_limit, extern_type*& __to_next) const {
        return do_unshift (__state,__to,__to_limit,__to_next);
    }

    // 22,2,1,5,1, p3
    result in (state_type& __state, const extern_type* __from,
               const extern_type* __from_end, const extern_type*& __from_next,
               intern_type* __to, intern_type* __to_limit,
               intern_type*& __to_next) const {
        return do_in (__state,__from,__from_end,__from_next,
                      __to,__to_limit,__to_next);
    }

    // 22,2,1,5,1, p5
    bool always_noconv () const _THROWS (()) {
        return do_always_noconv();
    }

    // 22,2,1,5,1, p4
    int encoding () const _THROWS (()) {
        return do_encoding();
    }

    // 22,2,1,5,1, p6
    int length (state_type& __state, const extern_type* __from,
                const extern_type* end, size_t __maxlen) const {
        return do_length (__state, __from, end, __maxlen);
    }

    // 22,2,1,5,1, p7
    int max_length() const _THROWS (())  {
        return do_max_length();
    }

    static locale::id id;

    // Rogue Wave extension:
    typedef basic_string<extern_type,char_traits<extern_type>,
                         allocator<extern_type> >
    external_string_type;

    typedef basic_string<intern_type,char_traits<intern_type>,
                         allocator<intern_type> >
    internal_string_type;

    internal_string_type in (const external_string_type &) const;

    external_string_type out (const internal_string_type &) const;

    // Implementation:
    enum { _C_facet_cat = locale::ctype, _C_ok_implicit = 1 };

private:

#ifdef _RWSTD_NO_MEMBER_TEMPLATES

    locale::id &_C_get_id (void) const {
        return id;
    }

#endif

};


// 22.2.1.5, p3 - performs no conversion

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT codecvt<char, char, mbstate_t>
    : public locale::facet,
      public codecvt_base
{
public:

    typedef char      extern_type;   
    typedef char      intern_type;
    typedef mbstate_t state_type;

protected:

    virtual result do_out (state_type&,
                           const intern_type *__from, const intern_type*,
                           const intern_type *&__from_next,
                           extern_type *__to, extern_type*,
                           extern_type *&__to_next) const {
        __from_next = __from;
        __to_next   = __to;
        return noconv; 
    }

    virtual result do_in (state_type&,
                          const extern_type *__from, const extern_type*,
                          const extern_type *&__from_next,
                         intern_type *__to, intern_type*,
                          intern_type*& __to_next) const {
        __from_next = __from;
        __to_next   = __to;
        return noconv; 
    }

    // 22.2.1.5.2, p5 - stores no characters
    virtual result do_unshift (state_type&, extern_type *__to,
                               extern_type*, extern_type *&__to_next) const {
        __to_next = __to;
        return noconv;
    }

    virtual int do_encoding () const _THROWS (()) {
        return 1;   // 1 external char converts to a single internal char
    }

    virtual bool do_always_noconv () const _THROWS (()){
        return true;   // conversion never necessary
    }

    virtual int do_length (const state_type&,
                           const extern_type *__from,
                           const extern_type *__from_end,
                           size_t __maxlen) const {
        // 22.2.1.5.2, p9 - preconditions
        _RWSTD_ASSERT (__from <= __from_end);

        // 22.2.1.5.2, p10
        size_t __len = __from_end - __from;
        return __len < __maxlen ? __len : __maxlen; 
    }

    virtual int do_max_length () const _THROWS (()) {
        return 1;   // 22.2.1.5.2, p11
    }

public:

    _EXPLICIT codecvt (size_t __refs = 0)
        : locale::facet (__refs, locale::ctype) { }

    result out (state_type &__state,
                const intern_type *__from, const intern_type* __from_end,
                const intern_type *&__from_next,
                extern_type* __to, extern_type* __to_limit,
                extern_type *&__to_next) const {
        return do_out (__state, __from, __from_end, __from_next,
                       __to, __to_limit, __to_next);
    }

    result unshift (state_type &__state,
                    extern_type *__to, extern_type *__to_limit,
                    extern_type*& __to_next) const {
        return do_unshift (__state, __to, __to_limit, __to_next);
    }

    result in (state_type &__state,
               const extern_type *__from, const extern_type *__from_end,
               const extern_type *&__from_next,
               intern_type *__to, intern_type *__to_limit,
               intern_type *&__to_next) const {
        return do_in (__state, __from, __from_end, __from_next,
                      __to, __to_limit, __to_next);
    }

    int encoding () const _THROWS (()) {
        return do_encoding ();
    }

    bool always_noconv () const _THROWS (()) {
        return do_always_noconv ();
    }

    int length (const state_type &__state, const extern_type *__from,
                const extern_type *__end,  size_t __maxlen) const {
        return do_length (__state, __from, __end, __maxlen);
    }

    int max_length () const _THROWS (()) {
        return do_max_length ();
    }

    static locale::id id;

#ifndef _RWSTD_NO_EXT_LOCALE

    // Rogue Wave extension:
    typedef string internal_string_type;
    typedef string external_string_type;

    internal_string_type in (const external_string_type & __s) const {
        return __s;
    }

    external_string_type out (const internal_string_type &__s) const {
        return __s;
    }

#endif   // _RWSTD_NO_EXT_LOCALE

    // Implementation:
    enum { _C_facet_cat = locale::ctype, _C_ok_implicit = 1 };

#ifdef _RWSTD_NO_MEMBER_TEMPLATES

private:

    locale::id& _C_get_id () const {
        return id;
    }

#endif   // _RWSTD_NO_MEMBER_TEMPLATES
};


#ifndef _RWSTD_NO_WCHAR_T

// 22.2.1.5, p3 - converts between wide and narrow characters
// of the native character set (i.e., widens and narrows)

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT codecvt<wchar_t, char, mbstate_t>
    : public locale::facet,
      public codecvt_base
{
public:

    typedef wchar_t   intern_type;
    typedef char      extern_type;
    typedef mbstate_t state_type;

protected:

    inline virtual result
    do_out (state_type&, const intern_type*, const intern_type*,
            const intern_type*&,
            extern_type*, extern_type*, extern_type*&) const;
    
    inline virtual result
    do_in (state_type&,
           const extern_type*, const extern_type*, const extern_type*&,
           intern_type*, intern_type*, intern_type*&) const;

    // 22.2.1.5.2, p5 - stores no characters
    virtual result do_unshift (state_type&, extern_type *__to,
                               extern_type*, extern_type *&__to_next) const {
        __to_next = __to;
        return noconv;
    }

    virtual bool do_always_noconv () const _THROWS (()) {
        return false;   // conversion always necessary
    }

    virtual int do_encoding () const _THROWS (()) {
        return 1;   // 1 external char converts to a single internal char
    }

    virtual int do_length (const state_type&, const extern_type* __from,
                           const extern_type* __from_end,
                           size_t __maxlen) const {
        // 22.2.1.5.2, p9 - preconditions
        _RWSTD_ASSERT (__from <= __from_end);

        // 22.2.1.5.2, p10
        size_t __len = size_t (__from_end - __from);
        return int (__len < __maxlen ? __len : __maxlen);
    }


    virtual int do_max_length () const _THROWS (()) {
        return 1;
    }

public:

    _EXPLICIT codecvt (size_t __refs = 0)
        : locale::facet(__refs,locale::ctype) { }

    result out (state_type& __state,
                const intern_type *__from, const intern_type *__from_end,
                const intern_type *&__from_next,
                extern_type *__to, extern_type* __to_limit,
                extern_type *& __to_next) const {
        return do_out (__state, __from, __from_end, __from_next, __to,
                       __to_limit, __to_next);
    }

    result unshift (state_type &__state,
                    extern_type *__to, extern_type *__to_limit,
                    extern_type *&__to_next) const {
        return do_unshift (__state, __to, __to_limit, __to_next);
    }

    result in (state_type& __state,
               const extern_type *__from, const extern_type *__from_end,
               const extern_type *&__from_next,
               intern_type *__to, intern_type *__to_limit,
               intern_type *&__to_next) const {
        return do_in (__state, __from, __from_end, __from_next,
                      __to, __to_limit, __to_next);
  }

    int encoding () const _THROWS (()) {
        return do_encoding();
    }

    bool always_noconv () const _THROWS (()) {
        return do_always_noconv ();
    }

    int length (const state_type &__state,
                const extern_type *__from, const extern_type *__end,
                size_t __maxlen) const {
        return do_length (__state, __from, __end, __maxlen);
    }

    int max_length () const _THROWS (()) {
        return do_max_length ();
    }

    static locale::id id;

    // Rogue Wave extension:
    typedef string external_string_type;
    typedef wstring internal_string_type;

    inline
    internal_string_type in (const external_string_type&) const;

    external_string_type out (const internal_string_type&) const;

    // Implementation:
    enum { _C_facet_cat = locale::ctype, _C_ok_implicit = 1 };

#ifdef _RWSTD_NO_MEMBER_TEMPLATES

private:

    locale::id& _C_get_id () const {
        return id;
    }

#endif   // _RWSTD_NO_MEMBER_TEMPLATES
};


inline codecvt_base::result
codecvt<wchar_t, char, mbstate_t>::
do_out (state_type&,
        const intern_type *__from, const intern_type *__from_end,
        const intern_type *&__from_next,
        extern_type *__to, extern_type *__to_end, extern_type *&__to_next) const
{
    // 22.2.1.5.2, p1 - preconditions
    _RWSTD_ASSERT (__from <= __from_end);
    _RWSTD_ASSERT (__to   <= __to_end);

    // copy internal sequence to external
    for (__from_next = __from, __to_next = __to;
         __from_next != __from_end && __to_next != __to_end;
         ++__from_next, ++__to_next)
        // prevent conversion problems due to char signedness
        *__to_next = _RWSTD_STATIC_CAST (unsigned char, *__from_next);

    return ok;
}


inline codecvt_base::result
codecvt<wchar_t,char,mbstate_t>::
do_in (state_type&,
       const extern_type *__from, const extern_type *__from_end,
       const extern_type *&__from_next,
       intern_type *__to, intern_type *__to_end, intern_type *&__to_next) const
{
    // 22.2.1.5.2, p1 - preconditions
    _RWSTD_ASSERT (__from <= __from_end);
    _RWSTD_ASSERT (__to   <= __to_end);

    // copy external sequence to internal
    for (__from_next = __from, __to_next = __to;
         __from_next != __from_end && __to_next != __to_end;
         ++__from_next, ++__to_next) {
        // prevent conversion problems due to char signedness
        *__to_next = _RWSTD_STATIC_CAST (unsigned char, *__from_next);
    }

    return ok;
}


// Inlined wstring codecvt<wchar_t,char,mbstate_t> members.
inline wstring
codecvt<wchar_t,char,mbstate_t>::in (const string &__s) const {

    wchar_t  __res_buf[1000];
    size_t   __maxlen = sizeof (__res_buf) / sizeof (wchar_t);
    size_t   __n      = mbstowcs (__res_buf, __s.c_str (), __maxlen);
    
    
    if (__n == __maxlen) {
        // FIXME:
        // 1000 was too small, allocate something larger and try again
        // ... but for now we just return the truncated result.
    }
    return wstring(__res_buf,(wstring::size_type)__n);
}



inline string
codecvt<wchar_t,char,mbstate_t>::out (const wstring &__s) const {
    char __res_buf[1000];
    size_t __n=wcstombs(__res_buf,__s.c_str(),sizeof __res_buf);
    
    if (__n==sizeof __res_buf) {
        // FIXME:
        // 1000 was too small, allocate something larger and try again ... but
        // for now we just return the truncated result.
    }

    return string(__res_buf,(string::size_type)__n);
}


#endif // _RWSTD_NO_WCHAR_T


// 22.2.1.6
template <class _InternT, class _ExternT, class _StateT>
class codecvt_byname: public codecvt<_InternT, _ExternT, _StateT>
{
public:
  
    typedef _InternT intern_type;
    typedef _ExternT extern_type;
    typedef _StateT  state_type; 

    _EXPLICIT codecvt_byname (const char*, size_t __ref = 0)
        : codecvt <_InternT, _ExternT, _StateT> (__ref) { }

protected:

    virtual codecvt_base::result
    do_out (state_type&,
            const intern_type*, const intern_type*, const intern_type*&,
            extern_type*, extern_type*, extern_type*&) const {
        return codecvt_base::error;
    }

    virtual codecvt_base::result
    do_in (state_type&,
           const extern_type*, const extern_type*, const extern_type*&,
           intern_type*, intern_type*, intern_type*&) const {
        return codecvt_base::error;
    }

    virtual codecvt_base::result
    do_unshift (state_type&,
                extern_type*, extern_type*, extern_type*&) const {
        return codecvt_base::error;
    }

    virtual int do_encoding () const _THROWS (()) {
        return -1;
    }

    virtual bool do_always_noconv () const _THROWS (()) {
        return false;
    }
};


template <class _InternT,class _ExternT,class _StateT>
_TYPENAME codecvt<_InternT,_ExternT,_StateT>::internal_string_type
codecvt<_InternT,_ExternT,_StateT>::in (const external_string_type &__s) const
{
    // Calculate the number of intern_type's that will be produced.
    // I'm not sure what to use for max in the do_length call --
    // too bad char_traits can't tell me.  This needs to be revised
    // to use separate in and out versions of do_length once someone
    // comes up with the right syntax.

    // FIXME: get rid of this hardcoded size and dynamically grow
    //        (use the result string to avoid copying)

    int __n = 1000; // do_length(state_type(0),__s.c_str(),
                    //  __s.c_str()+__s.length(),
                    // numeric_limits<size_t>::max() / do_max_length());
    
    const extern_type* __unused_from = 0;
    intern_type*       __unused_to   = 0;
    
    intern_type *__conversion_buffer = new intern_type[__n];

    state_type __stt (0);

    __n = do_in (__stt,
                 __s.c_str(), __s.c_str() + __s.length(), __unused_from,
                 __conversion_buffer, __conversion_buffer + __n, __unused_to);

    internal_string_type __res (__conversion_buffer,
                                __conversion_buffer + __n);

    delete[] __conversion_buffer;

    return __res;
}


_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT codecvt_byname<wchar_t, char, mbstate_t>
    : public codecvt<wchar_t, char, mbstate_t>
{
public:

    _EXPLICIT codecvt_byname (const char* = 0, size_t __ref = 0)
        : codecvt<wchar_t, char, mbstate_t> (__ref) { }

protected:

    virtual codecvt_base::result
    do_out (state_type&,
            const intern_type*, const intern_type*, const intern_type*&,
            extern_type*, extern_type*, extern_type*&) const;

    virtual codecvt_base::result
    do_in (state_type&,
           const extern_type*, const extern_type*, const extern_type*&,
           intern_type*, intern_type*, intern_type*&) const;

    virtual codecvt_base::result
    do_unshift (state_type&,
                extern_type*, extern_type*, extern_type*&) const;

    virtual int do_encoding () const _THROWS (()) {
        // 7.20.7.2 of C99: if first arg is 0, mbtowc returns non-zero or 0
        // if multibyte character encodings, respectively, do or do not have
        // state-dependent encodings
        return _RWSTD_MBTOWC (0, 0, 0, 0) ? -1 : 1;
    }

    virtual bool do_always_noconv () const _THROWS (()) {
        return false;   // conversion always necessary
    }

    virtual int do_max_length () const _THROWS (()) {
        // returns the max value do_length (s, from, from_end, 1) can return
        // for any valid range [from, from_end) - see LWG issue 74 (a DR)
        return MB_CUR_MAX;
    }
};


inline codecvt_base::result
codecvt_byname <wchar_t, char, mbstate_t>::
do_unshift (mbstate_t& __state,
            extern_type* __to, extern_type* __to_end,
            extern_type*& __to_next) const
{
    intern_type __c            = intern_type ();
    const intern_type *__dummy = 0;

    // converting a null character to an external representation
    // will write out '\0' preceded by an "unshift" sequence and
    // switch to initial conversion state
    return do_out (__state, &__c, &__c + 1, __dummy,
                   __to, __to_end, __to_next);
}


_RWSTD_NAMESPACE_END   // std

#if _RWSTD_DEFINE_TEMPLATE (CODECVT)
#  include <rw/_codecvt.cc>
#endif


#endif   // _RWSTD_CODECVT_H_INCLUDED
