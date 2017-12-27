/***************************************************************************
 *
 * _numeric.h - Definitions of the numeric facets
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _numeric.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_NUMERIC_H_INCLUDED
#define _RWSTD_NUMERIC_H_INCLUDED

#include <limits>

#include <rw/_ioiter.h>
#include <rw/_iosbase.h>
#include <rw/_locale.h>
#include <rw/_punct.h>
#include <rw/_defs.h>


_RWSTD_NAMESPACE_BEGIN (__rw)

template <class _CharT, class _InputIterator, class _TypeT>
inline _InputIterator
__rw_num_do_get (_CharT, 
                 _InputIterator           __begin, 
                 _InputIterator           __end, 
                 _STD::ios_base          &__flags, 
                 _STD::ios_base::iostate &__err, 
                 _TypeT                  &__val)
{
    __rw_digit_reader<_CharT, _InputIterator>
        __reader (__begin, __end, __flags);


    _TypeT __tmp = __reader._C_parse (__reader._C_get_digits (_TypeT ()),
                                      _TypeT ());

    if (__reader._C_error)
        __err = _STD::ios_base::failbit;
    else {
        __err = _STD::ios_base::goodbit;
        __val = __tmp;
    }

    if (__reader.eof ())
        __err |= _STD::ios_base::eofbit;

    return __begin;
}


_RWSTD_NAMESPACE_END   // __rw


_RWSTD_NAMESPACE_BEGIN (std)


// 22.2.2.1
template <class _CharT,
     class _InputIterator _RWSTD_COMPLEX_DEFAULT (istreambuf_iterator<_CharT>) >
class num_get: public locale::facet
{
public:
    typedef _CharT         char_type;
    typedef _InputIterator iter_type;

    static locale::id id;

    _EXPLICIT num_get (size_t __refs = 0)
        : locale::facet (__refs, locale::numeric) { }

#ifndef _RWSTD_NO_BOOL

    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags, 
                   ios_base::iostate &__err, bool& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return do_get (__begin, __end, __flags, __err, __val);
    }

#endif   // _RWSTD_NO_BOOL

    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags, 
                   ios_base::iostate &__err,  unsigned short& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return do_get (__begin, __end, __flags, __err, __val);
    }

    // extension
    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags, 
                   ios_base::iostate &__err,  short& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return _RW::__rw_num_do_get (_CharT (), __begin, __end, __flags,
                                            __err, __val);
    }

    // extension
    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags, 
                   ios_base::iostate &__err, int& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return _RW::__rw_num_do_get (_CharT (), __begin, __end, __flags,
                                            __err, __val);
    }

    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags, 
                   ios_base::iostate &__err, unsigned int& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return do_get (__begin, __end, __flags, __err, __val);
    }

    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags, 
                   ios_base::iostate &__err, long& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return do_get (__begin, __end, __flags, __err, __val);
    }

    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags, 
                   ios_base::iostate &__err, unsigned long& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return do_get (__begin, __end, __flags, __err, __val);
    }

    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags, 
                   ios_base::iostate &__err, float& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return do_get (__begin, __end, __flags, __err, __val);
    }

    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags, 
                   ios_base::iostate &__err, double& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return do_get (__begin, __end, __flags, __err, __val);
    }

    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags, 
                   ios_base::iostate &__err, long double& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return do_get (__begin, __end, __flags, __err, __val);
    }

    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags, 
                   ios_base::iostate &__err, void*& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return do_get (__begin, __end, __flags, __err, __val);
    }


#ifdef _RWSTD_LONG_LONG

    // extension
    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags,
                   ios_base::iostate &__err, _RWSTD_LONG_LONG& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return do_get (__begin, __end, __flags, __err, __val);
    }

    // extension
    iter_type get (iter_type __begin, iter_type __end, ios_base &__flags,
                   ios_base::iostate &__err,
                   unsigned _RWSTD_LONG_LONG& __val) const {
        _RWSTD_ASSERT_RANGE (__begin, __end);
        return do_get (__begin, __end, __flags, __err, __val);
    }
#endif   // _RWSTD_LONG_LONG


    // Implementation:
    enum { _C_facet_cat = locale::numeric, _C_ok_implicit = 1 };

protected:

#ifndef _RWSTD_NO_BOOL

    virtual iter_type do_get (iter_type, iter_type, ios_base&, 
                              ios_base::iostate&, bool&) const;
#endif   // _RWSTD_NO_BOOL

    virtual iter_type do_get (iter_type __begin, iter_type __end,
                              ios_base &__flags, ios_base::iostate &__err,
                              unsigned short &__val) const {
        return _RW::__rw_num_do_get (char_type (), __begin, __end,
                                     __flags, __err, __val);
    }

    virtual iter_type do_get (iter_type __begin, iter_type __end,
                              ios_base &__flags, ios_base::iostate &__err,
                              unsigned int &__val) const {
        return _RW::__rw_num_do_get (char_type (), __begin, __end,
                                     __flags, __err, __val);
    }

    virtual iter_type do_get (iter_type __begin, iter_type __end,
                              ios_base &__flags, ios_base::iostate &__err,
                              long &__val) const {
        return _RW::__rw_num_do_get (char_type (), __begin, __end,
                                     __flags, __err, __val);
    }

    virtual iter_type do_get (iter_type __begin, iter_type __end,
                              ios_base &__flags, ios_base::iostate &__err,
                              unsigned long &__val) const {
        return _RW::__rw_num_do_get (char_type (), __begin, __end,
                                     __flags, __err, __val);
    }

    virtual iter_type do_get (iter_type __begin, iter_type __end,
                              ios_base &__flags, ios_base::iostate &__err,
                              float &__val) const {
        return _RW::__rw_num_do_get (char_type (), __begin, __end,
                                     __flags, __err, __val);
    }

    virtual iter_type do_get (iter_type __begin, iter_type __end,
                              ios_base &__flags, ios_base::iostate &__err,
                              double &__val) const {
        return _RW::__rw_num_do_get (char_type (),
                                     __begin, __end,
                                     __flags, __err, __val);
    }

    virtual iter_type do_get (iter_type __begin, iter_type __end,
                              ios_base &__flags, ios_base::iostate &__err,
                              long double &__val) const {
        return _RW::__rw_num_do_get (char_type (),
                                     __begin, __end,
                                     __flags, __err, __val);
    }

    virtual iter_type do_get (iter_type __begin, iter_type __end,
                              ios_base &__flags, ios_base::iostate &__err,
                              void* &__val) const {
        return _RW::__rw_num_do_get (char_type (),
                                     __begin, __end,
                                     __flags, __err, __val);
    }

#ifdef _RWSTD_LONG_LONG

    // extension
    virtual iter_type do_get (iter_type __begin, iter_type __end,
                              ios_base &__flags, ios_base::iostate &__err,
                              _RWSTD_LONG_LONG &__val) const {
        return _RW::__rw_num_do_get (char_type (), __begin, __end,
                                            __flags, __err, __val);
    }

    // extension
    virtual iter_type do_get (iter_type __begin, iter_type __end,
                              ios_base &__flags, ios_base::iostate &__err, 
                              unsigned _RWSTD_LONG_LONG &__val) const {
        return _RW::__rw_num_do_get (char_type (), __begin, __end,
                                            __flags, __err, __val);
    }

#endif   // _RWSTD_LONG_LONG

private:

#ifdef _RWSTD_NO_MEMBER_TEMPLATES

    locale::id &_C_get_id () const {
        return id;
    }

#endif   // _RWSTD_NO_MEMBER_TEMPLATES

};


_RWSTD_INSTANTIATE_3 (class _RWSTD_EXPORT
                      num_get<char,
                      istreambuf_iterator<char, char_traits<char> > >);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_3 (class _RWSTD_EXPORT
                      num_get<wchar_t,
                      istreambuf_iterator<wchar_t, char_traits<wchar_t> > >);

#endif   // _RWSTD_NO_WCHAR_T


// 22.2.2.2
template <class _CharT,
    class _OutputIterator _RWSTD_COMPLEX_DEFAULT (ostreambuf_iterator<_CharT>)>
class num_put: public locale::facet
{
public:
    typedef _CharT          char_type;
    typedef _OutputIterator iter_type;

    _EXPLICIT num_put (size_t __refs = 0)
        : locale::facet (__refs, locale::numeric) { }

#ifndef _RWSTD_NO_BOOL
    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   bool __val) const {
        return do_put(__it, __flags, __fill, __val);
    }
#endif

    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   long __val) const {
        return do_put (__it, __flags, __fill, __val);
    }

    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   unsigned long __val) const {
        return do_put (__it, __flags, __fill, __val);
    }

    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   double __val) const {
        return do_put (__it, __flags, __fill, __val);
    }

    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   long double __val) const {
        return do_put(__it, __flags, __fill, __val);
    }

    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   const void* __val) const {
        return do_put(__it, __flags, __fill, __val);
    }

    // extensions

    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   short __val) const {
        return do_put(__it, __flags, __fill, __val);
    }

    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   unsigned short __val) const {
        return do_put(__it, __flags, __fill, __val);
    }

    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   int __val) const {
        return do_put(__it, __flags, __fill, __val);
    }

    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   unsigned int __val) const {
        return do_put(__it, __flags, __fill, __val);
    }

#ifdef _RWSTD_LONG_LONG

    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   _RWSTD_LONG_LONG __val) const {
        return do_put(__it, __flags, __fill, __val);
    }

    iter_type put (iter_type __it, ios_base& __flags, char_type __fill,
                   unsigned _RWSTD_LONG_LONG __val) const {
        return do_put(__it, __flags, __fill, __val);
    }

#endif   // _RWSTD_LONG_LONG

    static locale::id id;

  // Implementation:
    enum { _C_facet_cat = locale::numeric, _C_ok_implicit = 1 };

protected:

#ifndef _RWSTD_NO_BOOL
    virtual iter_type do_put (iter_type, ios_base&, char_type, bool) const;
#endif
    virtual iter_type do_put (iter_type, ios_base&, char_type, long) const;
    virtual iter_type do_put (iter_type, ios_base&, char_type,
                              unsigned long) const;
    virtual iter_type do_put (iter_type, ios_base&, char_type, 
                              double) const __softfp;
    virtual iter_type do_put (iter_type, ios_base&, char_type,
                              long double) const __softfp;
    virtual iter_type do_put (iter_type, ios_base&, char_type,
                              const void*) const;

    // extensions
    virtual iter_type do_put (iter_type, ios_base&, char_type, short) const;
    virtual iter_type do_put (iter_type, ios_base&, char_type,
                              unsigned short) const;
    virtual iter_type do_put (iter_type, ios_base&, char_type, int) const;
    virtual iter_type do_put (iter_type, ios_base&, char_type,
                              unsigned int) const;

#ifdef _RWSTD_LONG_LONG

    virtual iter_type do_put (iter_type, ios_base&, char_type, 
                              _RWSTD_LONG_LONG) const;

    virtual iter_type do_put (iter_type, ios_base&, char_type, 
                              unsigned _RWSTD_LONG_LONG) const;

#endif   // _RWSTD_LONG_LONG

private:

#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    locale::id &_C_get_id (void) const { return id; }
#endif

};


_RWSTD_INSTANTIATE_3 (class _RWSTD_EXPORT
                      num_put<char,
                      ostreambuf_iterator<char, char_traits<char> > >);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_3 (class _RWSTD_EXPORT
                      num_put<wchar_t,
                      ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >);

#endif   // _RWSTD_NO_WCHAR_T


// 22.2.3.1
template <class _CharT>
class numpunct: public locale::facet, 
                public _RW::__rw_numpunct_impl<_CharT>
{
public:
    typedef _CharT char_type;
    typedef basic_string<_CharT, char_traits<_CharT>, allocator<_CharT> >
    string_type;

    // The second parameter (i) to the constructor is implementation specific.
    // For portable code, always let it default as shown.

#ifndef _MSC_VER
    _EXPLICIT numpunct (size_t __refs = 0, 
                        const _RW::__rw_numpunct_init_data<_CharT>* =
                        _RW::__rw_numpunct_impl<_CharT>::_C_get_ivals ());

#else
    _EXPLICIT numpunct (size_t __refs = 0, 
                        const _RW::__rw_numpunct_init_data<_CharT>* =
                        _C_get_ivals ());
#endif   // _MSC_VER

    char_type decimal_point () const {
        return do_decimal_point();
    }

    char_type thousands_sep () const {
        return do_thousands_sep();
    }

    string  grouping () const {
        return do_grouping();
    }

    string_type truename () const {
        return do_truename ();
    }

    string_type falsename ()  const {
        return do_falsename();
    }

    static locale::id id;

    // Implementation:
    enum { _C_facet_cat = locale::numeric, _C_ok_implicit = 1 };

protected:

    virtual char_type do_decimal_point () const {
        return this->_C_dp;
    }

    virtual char_type do_thousands_sep () const {
        return this->_C_ts;
    }

    virtual string do_grouping () const {
        return this->_C_gr;
    }

    virtual string_type do_truename () const {
        return this->_C_tn;
    }

    virtual string_type do_falsename () const {
        return this->_C_fn;
    }

  // Implementation:

private:
    virtual void _C_initfacet (const locale&);

#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    locale::id &_C_get_id (void) const { return id; }
#endif

};

template <class _CharT>
inline numpunct<_CharT>::
numpunct (size_t __ref, const _RW::__rw_numpunct_init_data<_CharT> *__np)
    : locale::facet(__ref, locale::numeric), 
      _RW::__rw_numpunct_impl<_CharT>(__np)
{ }


_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT numpunct<char>);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT numpunct<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_T


// 22.2.3.2
template <class _CharT>
class numpunct_byname: public numpunct<_CharT>
{
public:

    _EXPLICIT numpunct_byname (const char *__name, size_t __refs = 0) :
#if defined (_MSC_VER) && !defined (__BORLANDC__)
        numpunct<_CharT>(__refs, _C_get_init_by_name_(__name))
#else
        numpunct<_CharT>(__refs,
                         _RW::__rw_numpunct_impl_data<_CharT>::_C_get_init_by_name_(__name))
#endif // _MSC_VER
        { }
};


_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)


_RWSTD_SPECIALIZED_FUNCTION
inline _STD::numpunct<char>*
__rw_create_native_facet (_STD::numpunct<char>*)
{
    return new _STD::numpunct_byname<char>("", 1);
}


#ifndef _RWSTD_NO_FUNC_PARTIAL_SPEC

template <class _CharT>
inline _STD::numpunct<_CharT>*
__rw_create_named_facet (_STD::numpunct<_CharT>*, const char *__name,
                         _RWSTD_C::size_t __ref)
{
    return new _STD::numpunct_byname<_CharT>(__name, __ref);
}

#else   // if defined (_RWSTD_NO_FUNC_PARTIAL_SPEC)

_RWSTD_SPECIALIZED_FUNCTION
inline numpunct<char>*
__rw_create_named_facet (_STD::numpunct<char>*, const char *__name,
                         _RWSTD_C::size_t __ref)
{
    return new _STD::numpunct_byname<char>(__name, __ref);
}


#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_SPECIALIZED_FUNCTION
inline _STD::numpunct<wchar_t>*
__rw_create_named_facet (_STD::numpunct<wchar_t>*, const char *__name,
                         _RWSTD_C::size_t __ref)
{
    return new _STD::numpunct_byname<wchar_t>(__name, __ref);
}

#endif   // _RWSTD_NO_WCHAR_T
#endif   // _RWSTD_NO_FUNC_PARTIAL_SPEC


_RWSTD_NAMESPACE_END   // __rw


#if _RWSTD_DEFINE_TEMPLATE (NUMERIC)
#  include <rw/_numeric.cc>
#endif


#endif   // _RWSTD_NUMERIC_H_INCLUDED
