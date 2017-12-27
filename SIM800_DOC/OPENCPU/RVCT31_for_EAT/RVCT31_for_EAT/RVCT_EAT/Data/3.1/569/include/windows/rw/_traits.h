/***************************************************************************
 *
 * _traits.h - Declarations for char_traits 
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _traits.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_TRAITS_H_INCLUDED
#define _RWSTD_TRAITS_H_INCLUDED

#include <rw/_defs.h>

#include _RWSTD_CSTDDEF
#include _RWSTD_CSTDIO
#include _RWSTD_CSTRING
#include _RWSTD_CWCHAR

#ifdef  _WIN64
#   include _RWSTD_SYS_TYPES_H
#endif

#if defined (_RWSTD_STRICT_ANSI)
_RWSTD_USING_SNI (uid_t);
#endif


_RWSTD_NAMESPACE_BEGIN (std)



// 27.4.1, p2
typedef _RWSTD_SSIZE_T streamsize;
typedef _RWSTD_SSIZE_T streamoff;


/* fpos<> maintains all the information necessary to restore an arbitrary
 * sequence, controlled by the Standard C++ library, to a previous stream
 * position and conversion state.
 */

template <class _StateT>
class fpos
{
public:

    typedef _StateT  state_type;

    fpos (streamoff __off = 0) // allowing a default is an extension to the standard
        : _C_pos (__off),
          _C_state ()
        {
    }

    fpos (streamoff __off, state_type __state) // allowing two arguments is an extension to the standard
        : _C_pos (__off),
          _C_state (__state)
        { }

    operator streamoff () const {
        return _C_pos;
    }

    fpos (const fpos&);

    fpos& operator= (const fpos&);

    state_type state () const {
        return _C_state;
    }

    void state (state_type __state) {
        _C_state = __state;
    }

    // arithmetic operators
    fpos operator- (const fpos &__off) const {
        return fpos (*this) -= __off;
    }

    fpos operator+ (const fpos &__off) const {
        return fpos (*this) += __off;
    }

    fpos& operator-= (const fpos &__off) {
        return _C_pos -= __off._C_pos, *this;
    }

    fpos& operator+= (const fpos &__off) {
        return _C_pos += __off._C_pos, *this;
    }

    fpos operator- (streamoff __off) const {
        return fpos (*this) -= __off;
    }

    fpos operator+ (streamoff __off) const {
        return fpos (*this) += __off;
    }

    fpos& operator-= (streamoff __off) {
        return _C_pos -= __off, *this;
    }

    fpos& operator+= (streamoff __off) {
        return _C_pos += __off, *this;
    }

    // relational operators
    bool operator== (const fpos& __rhs) const; 

    bool operator< (const fpos& __rhs) const {
        return _C_pos < __rhs._C_pos;
    }

    bool operator<= (const fpos& __rhs) const {
        return !(__rhs < *this);
    }

    bool operator>= (const fpos& __rhs) const {
        return !(*this < __rhs);
    }

    bool operator!= (const fpos& __rhs) const {
        return !(*this == __rhs);
    }

    bool operator> (const fpos& __rhs) const {
        return __rhs < *this;
    }

    bool operator== (streamoff __rhs) const {
        return _C_pos == __rhs;
    }

    bool operator< (streamoff __rhs) const {
        return _C_pos < __rhs;
    }

    bool operator<= (streamoff __rhs) const {
        return _C_pos <= __rhs;
    }

    bool operator>= (streamoff __rhs) const {
        return _C_pos >= __rhs;
    }

    bool operator> (streamoff __rhs) const {
        return _C_pos > __rhs;
    }

    bool operator!= (streamoff __rhs) const {
        return !(_C_pos == __rhs);
    }

private:

    streamoff   _C_pos;     // signed displacement
    state_type  _C_state;   // conversion state
};


template <class _StateT> 
inline
fpos<_StateT>::fpos(const fpos<_StateT>& __rhs)
{
    _C_pos    = __rhs._C_pos;
    _C_state  = __rhs._C_state;
}


template <class _StateT>
inline fpos<_StateT>& 
fpos<_StateT>::operator= (const fpos<_StateT>& __rhs)
{
    if (&__rhs != this) {
        _C_pos    = __rhs._C_pos;
        _C_state  = __rhs._C_state;
    }
    return *this;
}


template <class _StateT>
inline bool 
fpos<_StateT>::operator== (const fpos<_StateT>& __rhs) const
{ 
    return    _C_pos == __rhs._C_pos
#ifndef  _RWSTD_NO_MBSTATE_COMPARE
           && _C_state == __rhs._C_state
#endif
    ;
}


template <class  _CharT> 
struct char_traits 
{
    typedef _CharT                   char_type;
    typedef int                      int_type;
    typedef streamoff                off_type;
    typedef mbstate_t                state_type;
    typedef fpos<state_type>         pos_type;

    static int_type eof () {
        return EOF;
    }

    static void assign (char_type& __c1, const char_type& __c2) {
        __c1 = __c2;
    }

    static bool eq (const char_type& __c1, const char_type& __c2) {
        return __c1 == __c2;
    }

    static bool lt (const char_type& __c1, const char_type& __c2) {
        return __c1 < __c2;
    }

    static int compare (const char_type* __s1, const char_type* __s2,
                        _RWSTD_C::size_t __n) {
        _RWSTD_C::size_t __i = 0;
        while (__i < __n) { 
            if (!eq (__s1[__i], __s2[__i])) {
                return lt (__s1[__i], __s2[__i]) ? -1 : 1;
            }
            ++__i;
        }
        return 0;
    }
        
    static _RWSTD_C::size_t length (const char_type *__s) {
        _RWSTD_C::size_t __len = 0;
        while (!eq (*__s++, char_type ()))
            ++__len;
        return __len;
    }
 
    static const char_type* 
    find (const char_type* __s, _RWSTD_C::size_t __n, const char_type& __a) {
        while (__n-- > 0 && !eq (*__s, __a) )
            ++__s;
        return  eq (*__s, __a) ? __s : 0;
    }

    static char_type* move (char_type* __s1, const char_type* __s2,
                            _RWSTD_C::size_t __n) {
        if (__s1 < __s2)
            copy (__s1, __s2, __n);
        else if (__s1 > __s2) {
            __s1 += __n;
            __s2 += __n;
            for (_RWSTD_C::size_t __i = 0; __i < __n; ++__i) 
                assign (*--__s1, *--__s2);
        }
        return __s1;
    }

    static char_type* copy (char_type *__dst, const char_type *__src,
                             _RWSTD_C::size_t __n) {
        memcpy (__dst, __src, __n * sizeof (char_type));
        return __dst;
    }

    static char_type*  assign (char_type* __s, _RWSTD_C::size_t __n, char_type __a) {
        char_type* __tmp = __s;
        while (__n-- > 0) 
            assign (*__tmp++, __a);
        return __s;
    }

    static bool eq_int_type (const int_type& __c1, const int_type& __c2) {
        return __c1 == __c2;
    }

    static int_type not_eof (const int_type& __c) {
        return eq_int_type (eof (), __c) ? 0 : __c;
    }

    static char_type to_char_type (const int_type& __c) {
        return __c;
    }
      
    static int_type to_int_type (const char_type& __c) {
        return __c;
    }
};


// 21.1.3.1
_RWSTD_SPECIALIZED_CLASS  
struct char_traits<char> 
{
    typedef char                      char_type;
    typedef _RWSTD_INT_T              int_type;
    typedef streamoff                 off_type; 
    typedef mbstate_t                 state_type;
    typedef fpos<state_type>          pos_type;

    static int_type eof () {
        return EOF;
    }

    static void assign (char_type& __c1, const char_type& __c2){
        __c1 = __c2;
    }

    static bool eq (const char_type& __c1, const char_type& __c2) {
        return __c1 == __c2;
    }

    static bool lt (const char_type& __c1, const char_type& __c2) {
        return __c1 < __c2;
    }

    static int compare (const char_type* __s1, const char_type* __s2,
                        _RWSTD_C::size_t __n) {
        return memcmp (__s1, __s2, __n);
    }

    static const char_type* find (const char_type* __s, 
                                  _RWSTD_C::size_t __n, const char_type& __a) {
        return (char_type*) memchr (__s, __a, __n);
    }

    static _RWSTD_C::size_t length (const char_type *__s) {
        return strlen (__s);
    }

    static char_type * move (char_type* __s1, const char_type* __s2,
                             _RWSTD_C::size_t __n) {
#ifndef _RWSTD_NO_MEMMOVE
        memmove (__s1, __s2, __n);
#else
        if (__s1 < __s2)
            copy (__s1, __s2, __n);
        else if (__s1 > __s2) {
            __s1 += __n;
            __s2 += __n;
            for (_RWSTD_C::size_t __i = 0; __i < __n; ++__i)
                assign (*--__s1, *--__s2);
        }
#endif
        return __s1;
    }

    static char_type *copy (char_type *__dst, const char_type *__src,
                             _RWSTD_C::size_t __n){
        memcpy (__dst, __src, __n); 
        return __dst;
    }

    static char_type* assign (char_type* __s, _RWSTD_C::size_t __n, char_type __a) {
        memset (__s, __a, __n);
        return __s;
    }

    static bool eq_int_type (const int_type& __c1, const int_type& __c2) {
        return __c1 == __c2;
    }

    static int_type not_eof (const int_type& __c) {
        return eq_int_type (eof (), __c) ? 0 : __c;
    }
                        
    static char_type to_char_type (const int_type& __c) {
        return _RWSTD_STATIC_CAST (char_type, __c);
    }
      
    static int_type to_int_type (const char_type& __c) {
        // make sure (signed char)'\xff' converts to 255 and not -1
        return _RWSTD_STATIC_CAST (unsigned char, __c);
    }
};


#ifndef _RWSTD_NO_WCHAR_T

// 21.1.3.2
_RWSTD_SPECIALIZED_CLASS  
struct char_traits<wchar_t> 
{
    typedef wchar_t                   char_type;
    typedef _RWSTD_WINT_T             int_type;
    typedef streamoff                 off_type;
    typedef mbstate_t                 state_type;
    typedef fpos<state_type>          pos_type;

#ifndef WEOF
#  define WEOF (-1)
#endif

    static int_type eof () {
        return int_type (WEOF);
    }

    static void assign (char_type& __c1, const char_type& __c2) {
        __c1 = __c2;
    }

    static bool eq (const char_type& __c1, const char_type& __c2) {
        return __c1 == __c2;
    }

    static bool lt (const char_type& __c1, const char_type& __c2) {
        return __c1 < __c2;
    }

    static int compare (const char_type* __s1, const char_type* __s2,
                        _RWSTD_C::size_t __n) {
#ifndef _RWSTD_NO_WMEMCMP
        return wmemcmp (__s1, __s2, __n);
#else
        _RWSTD_C::size_t __i=0;
        while (__i < __n) { 
            if (!eq (__s1[__i], __s2[__i])) {
                return lt (__s1[__i], __s2[__i]) ? -1 : 1;
            }
            ++__i;
        }
      return 0;
#endif
    }

    static _RWSTD_C::size_t length (const char_type *__s) {
#ifndef _RWSTD_NO_WCSLEN
        // [harmless] cast necessary on CygWin
        return wcslen (_RWSTD_CONST_CAST (char_type*, __s));
#else
        _RWSTD_C::size_t __len = 0;
        while (!eq (*__s++, char_type ()))
            ++__len;
        return __len;
#endif
    }
      
    static const char_type* find (const char_type* __s, _RWSTD_C::size_t __n, 
                                  const char_type& __a)           {
#ifndef _RWSTD_NO_WMEMCHR
        return _RWSTD_STATIC_CAST (const char_type*, wmemchr (__s, __a, __n));
#else
        while (__n-- > 0 && !eq (*__s, __a))
            ++__s;
        return  eq (*__s, __a) ? __s : 0;
#endif   // _RWSTD_NO_WMEMCHR
    }

    static char_type * copy (char_type *__dst, const char_type *__src,
                             _RWSTD_C::size_t __n) {
        memcpy (__dst, __src, __n * sizeof (char_type));
        return __dst;
    }
      
    static char_type * move (char_type* __s1, const char_type* __s2,
                             _RWSTD_C::size_t __n) {
#ifndef _RWSTD_NO_WMEMMOVE
        wmemmove (__s1, __s2, __n);
#else
        if (__s1 < __s2)
            copy (__s1, __s2, __n);
        else if (__s1 > __s2) {
            __s1 += __n;
            __s2 += __n;
            for (_RWSTD_C::size_t __i = 0; __i < __n; ++__i)
                assign (*--__s1, *--__s2);
        }
#endif   // _RWSTD_NO_WMEMMOVE
        return __s1;
    }

    static char_type* assign (char_type* __s, _RWSTD_C::size_t __n, char_type __a){
#ifndef _RWSTD_NO_WMEMSET
        wmemset (__s, __a, __n);
#else
        char_type* __tmp = __s;
        while (__n-- > 0) 
            assign (*__tmp++, __a);
#endif   // _RWSTD_NO_WMEMSET
        return __s;
    }

    static bool eq_int_type (const int_type& __c1, const int_type& __c2) {
        return __c1 == __c2;
    }

    static int_type not_eof (const int_type& __c) {
        return eq_int_type (eof (), __c) ? 0 : __c;
    }

    static char_type to_char_type (const int_type& __c) {
        return __c;
    }
      
    static int_type to_int_type (const char_type& __c) {
        return __c;
    }
};

#endif


_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)

// Implementation _Traits class, rw_traits, provides specialized
// algorithms to speed up find operations on char and wchar_t strings

template <class _CharT, class _Traits> 
struct rw_traits
{
    static const _CharT* find (const _CharT* __s, const _CharT* __v) { 
        _RWSTD_C::size_t __slen = _Traits::length (__s);
        _RWSTD_C::size_t __vlen = _Traits::length (__v);
        for (_RWSTD_C::size_t __j = 0; (__j + __vlen) <= __slen ; ++__j) {
            bool __found = true;
            for (_RWSTD_C::size_t __i = 0; __i < __vlen ; ++__i) {
                if (!_Traits::eq (__s[__j + __i], __v[__i])) {
                    __found = false;
                    break;
                }
            }
            if (__found)
                return &__s[__j];
        }
        return 0;
    }

    static const _CharT* rfind (const _CharT* __s, _CharT __v, _RWSTD_C::size_t __pos) {
        for  (const _CharT* __p = __s + __pos; __p >= __s; --__p) {
            if (_Traits::eq (*__p, __v))
                return __p;
        }
        return 0;       
    }

    static _RWSTD_C::size_t find_first_of (const _CharT* __s, const _CharT* __v) {
        const _CharT* __p = __s;
        for (; !_Traits::eq (*__p, _CharT ()); ++__p) {
            for (const _CharT* __q = __v; !_Traits::eq (*__q, _CharT ()); ++__q)
                if (_Traits::eq (*__p, *__q))
                    return __p - __s;
        }
        return  __p - __s;
    }

    static _RWSTD_C::size_t find_first_not_of (const _CharT* __s, const _CharT* __v) {
        bool __found;
        const _CharT* __p = __s;
        for (; !_Traits::eq (*__p, _CharT ()); ++__p) {
            __found = false;
            for (const _CharT* __q = __v;
                 !_Traits::eq (*__q, _CharT ()); ++__q){
                if (_Traits::eq (*__p, *__q)){
                    __found = true; 
                    break;
                }  
            }
            if (!__found)      
                return __p - __s;
        }
        return  __p-__s;
    }
};


_RWSTD_SPECIALIZED_CLASS 
struct rw_traits<char, _STD::char_traits<char> >
{
    static const char* find (const char* __s, const char* __v) {
        return _RWSTD_C::strstr (__s, __v);
    }

    static const char* rfind (const char* __s, char __v, _RWSTD_C::size_t __pos) { 
        const char* __p = __s + __pos;
        while (__p >= __s)
        {
            if (_STD::char_traits<char>::eq (*__p, __v))
                return __p;
            __p--; 
        }
        return 0;       
    }

    static _RWSTD_C::size_t find_first_of (const char* __s, const char* __v) {
        return _RWSTD_C::strcspn (__s, __v);
    }

    static _RWSTD_C::size_t find_first_not_of (const char* __s, const char* __v) {
        return _RWSTD_C::strspn (__s, __v);
    }

};

#if !defined(_RWSTD_NO_WCSCHR)  && !defined(_RWSTD_NO_WCSSPN) && !defined(_RWSTD_NO_WCSSPN)
_RWSTD_SPECIALIZED_CLASS 
struct rw_traits<wchar_t, _STD::char_traits<wchar_t> >
{
    static const wchar_t* find (const wchar_t* __s, const wchar_t* __v) {
#ifndef _RWSTD_NO_WCSSTR
        return _RWSTD_C::wcsstr (__s, __v);
#elif defined (__hpux)
        return wcswcs (__s, __v);
#else
        _RWSTD_C::size_t __len1 = _STD::char_traits<wchar_t>::length (__s);
        _RWSTD_C::size_t __len2 = _STD::char_traits<wchar_t>::length (__v);

        for (const wchar_t *__p = __s; (__p = _RWSTD_C::wcschr (__p, *__v));) {
            _RWSTD_C::size_t __off = __p - __s;
            if (__len1 - __off < __len2)
                break;
            if (0 == _RWSTD_C::memcmp (__p, __v, sizeof *__v * __len2))
                return __p;
            ++__p;
        }
        return 0;

#endif   // _RWSTD_NO_WCSSTR
    }

    static const wchar_t* rfind (const wchar_t* __s, wchar_t __v, 
                                _RWSTD_C::size_t __pos) { 
        const wchar_t* __p = __s + __pos;
        while (__p >= __s) {
            if (_STD::char_traits<wchar_t>::eq (*__p, __v))
                return __p;
            __p--;
        }
        return 0;       
    }

    static _RWSTD_C::size_t find_first_of (const wchar_t* __s, const wchar_t* __v) {
        return _RWSTD_C::wcscspn (__s, __v);
    }

    static _RWSTD_C::size_t find_first_not_of (const wchar_t* __s, const wchar_t* __v) {
        return _RWSTD_C::wcsspn (__s, __v);
    }

};
#endif /* !defined(_RWSTD_NO_WCSCHR)  && !defined(_RWSTD_NO_WCSSPN) && !defined(_RWSTD_NO_WCSSPN) */

_RWSTD_NAMESPACE_END   // __rw


#endif   // _RWSTD_TRAITS_H_INCLUDED
