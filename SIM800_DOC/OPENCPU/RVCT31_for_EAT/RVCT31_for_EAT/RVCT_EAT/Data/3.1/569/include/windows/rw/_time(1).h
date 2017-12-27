/***************************************************************************
 *
 * _time.h - Definitions of the time facets
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _time.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_TIME_H_INCLUDED
#define _RWSTD_TIME_H_INCLUDED

#include <streambuf>

#include <rw/_punct.h>
#include <rw/_locale.h>
#include <rw/_defs.h>


_RWSTD_NAMESPACE_BEGIN(std)

// 22.2.5.1
struct _RWSTD_EXPORT time_base
{
   enum dateorder { no_order, dmy, mdy, ymd, ydm };
};

_RWSTD_NAMESPACE_END   // std



_RWSTD_NAMESPACE_BEGIN (__rw)

_USING (_RWSTD_C::size_t);

// -------------------------------------------------------------------------
// Implementation class template -- __rw_time_reader<_CharT, _InputIterator>.
// -------------------------------------------------------------------------

template <class _CharT, class _InputIterator>
class _RWSTD_EXPORT __rw_time_reader
    : public __rw_digit_reader<_CharT, _InputIterator>
{
public:
    typedef _TYPENAME __rw_timepunct<_CharT>::string_type string_type;
    const __rw_timepunct<_CharT> &_C_timp;

    __rw_time_reader (_InputIterator &__in, _InputIterator &__end, 
                      _STD::ios_base &__io, 
                      const __rw_timepunct<_CharT> &__tpunct)
        :  __rw_digit_reader<_CharT, _InputIterator>(__in, __end, __io), 
           _C_timp (__tpunct) {

        // Don't recognize signs on any numeric input.
        this->_C_state &= ~this->_C_signed;
    }

    bool _C_get_time_pattern (const string_type&, tm *);
};

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    __rw_time_reader<char,
        _STD::istreambuf_iterator<char, _STD::char_traits<char> > >);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    __rw_time_reader<wchar_t,
        _STD::istreambuf_iterator<wchar_t, _STD::char_traits<wchar_t> > >);

#endif   // _RWSTD_NO_WCHAR_T


_RWSTD_NAMESPACE_END   //__rw


_RWSTD_NAMESPACE_BEGIN (std)

// 22.2.5.1
template <class _CharT,
    class _InputIterator _RWSTD_COMPLEX_DEFAULT (istreambuf_iterator<_CharT>) >
class time_get: public locale::facet, public time_base
{
public:
    typedef _CharT                         char_type;
    typedef _InputIterator                 iter_type;
    typedef time_get<char_type, iter_type> time_get_type;

    _EXPLICIT time_get (size_t __refs = 0)
        : locale::facet (__refs, locale::time), _C_timp (0)
        { ; }

    dateorder date_order() const { 
        return do_date_order(); 
    }

    iter_type get_time (iter_type __start, iter_type __end, ios_base& __fmat, 
                        ios_base::iostate &__err, tm* __time) const { 
        return do_get_time (__start, __end, __fmat, __err, __time); 
    }

    iter_type get_date (iter_type __start, iter_type __end, ios_base& __fmat, 
                        ios_base::iostate &__err, tm* __time) const { 
        return do_get_date(__start, __end, __fmat, __err, __time); 
    }

    iter_type get_weekday (iter_type __start, iter_type __end,
                           ios_base& __fmat, 
                           ios_base::iostate &__err, tm* __time) const { 
        return do_get_weekday(__start, __end, __fmat, __err, __time); 
    }

    iter_type get_monthname (iter_type __start, iter_type __end,
                             ios_base& __fmat, 
                             ios_base::iostate &__err, tm* __time) const {
        return do_get_monthname(__start, __end, __fmat, __err, __time); 
    }

    iter_type get_year (iter_type __start, iter_type __end, ios_base& __fmat, 
                        ios_base::iostate &__err, tm* __time) const { 
        return do_get_year(__start, __end, __fmat, __err, __time); 
    }

    static locale::id id;

    // Implementation:
    enum { _C_facet_cat = locale::time, _C_ok_implicit = 1 };

protected:

    virtual dateorder do_date_order()  const;

    virtual iter_type  do_get_time (iter_type, iter_type, ios_base&,
                                    ios_base::iostate&, tm*) const;
    virtual iter_type do_get_date (iter_type, iter_type, ios_base&,
                                   ios_base::iostate&, tm*) const;
    virtual iter_type do_get_weekday (iter_type, iter_type, ios_base&,
                                      ios_base::iostate&, tm*) const;
    virtual iter_type do_get_monthname (iter_type, iter_type, ios_base&,
                                        ios_base::iostate&, tm*) const;
    virtual iter_type do_get_year (iter_type, iter_type, ios_base&,
                                   ios_base::iostate&, tm*) const;

    // Implementation:

protected:
    const _RW::__rw_timepunct<char_type>* _C_timp;
    string                                _C_name;

private:
    void _C_check_init () const { 
        if (!_C_timp)
            _RWSTD_CONST_CAST (time_get_type*,
                               this)->_C_initfacet (locale ());
    }

    virtual void _C_initfacet (const locale &__loc);

#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    locale::id &_C_get_id (void) const { return id; }
#endif
};


template <class _CharT, class _InputIterator>
void time_get<_CharT, _InputIterator>::_C_initfacet (const locale &__loc)
{
    _RW::__rw_use_c_lib_locale __cloc (_C_name.c_str(), LC_TIME);

    // bypass the mutex in std::use_facet<>() by directly calling
    // the implementation locale member function to prevent a deadlock

    typedef _RW::__rw_timepunct<char_type>  _TimePunct;
    typedef _RW::__rw_facet_maker<_TimePunct>    _FacetMaker;
    
    const locale::facet& __facet =
        __loc._C_unsafe_use_facet (_TimePunct::id, _TimePunct::_C_ok_implicit,
                                   _TimePunct::_C_facet_cat,
                                   _FacetMaker::_C_maker_func);

    _C_timp = _RWSTD_STATIC_CAST (const _TimePunct*, &__facet);
}


_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    time_get<char, istreambuf_iterator<char, char_traits<char> > >);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    time_get<wchar_t,
        istreambuf_iterator<wchar_t, char_traits<wchar_t> > >);

#endif   // _RWSTD_NO_WCHAR_T


// 22.2.5.2
template <class _CharT,
    class _InputIterator _RWSTD_COMPLEX_DEFAULT (istreambuf_iterator<_CharT>) >
class time_get_byname: public time_get<_CharT, _InputIterator>
{
public:
    _EXPLICIT time_get_byname (const char *__name, size_t __refs = 0)
        : time_get<_CharT, _InputIterator>(__refs) {
        this->_C_name = __name;
    }
};


// 22.2.5.3
template <class _CharT,
    class _OutputIterator _RWSTD_COMPLEX_DEFAULT (ostreambuf_iterator<_CharT>) >
class time_put: public locale::facet
{
public:
    typedef _CharT                         char_type;
    typedef _OutputIterator                iter_type;
    typedef time_put<char_type, iter_type> time_put_type;

    _EXPLICIT time_put (size_t __refs = 0)
        : locale::facet (__refs, locale::time), _C_timp (0)
        { }

    iter_type put (iter_type, ios_base&, char_type, const tm*,
                   const char_type*, const char_type*) const;

    iter_type put (iter_type __start, ios_base& __fmat, char_type __fill, 
                   const tm* __time, char __fmt, char __modifier = 0) const { 
        return do_put (__start, __fmat, __fill, __time, __fmt, __modifier); 
    }

    static locale::id id;

    // Implementation:
    enum { _C_facet_cat = locale::time, _C_ok_implicit = 1 };

protected:

    virtual iter_type do_put (iter_type, ios_base&, char_type, const tm*,
                              char, char) const;

protected:
    const _RW::__rw_timepunct<char_type>* _C_timp;
    string                                _C_name;

private:

    void _C_check_init () const { 
        if (!_C_timp)
            _RWSTD_CONST_CAST (time_put_type*,
                               this)->_C_initfacet (locale ());
    }
    
    virtual void _C_initfacet (const locale &__loc);

#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    locale::id &_C_get_id (void) const { return id; }
#endif
};


template <class _CharT, class _InputIterator>
void time_put<_CharT, _InputIterator>::_C_initfacet (const locale &__loc)
{
    _RW::__rw_use_c_lib_locale __cloc (_C_name.c_str(), LC_TIME);

    // bypass the mutex in std::use_facet<>() by directly calling
    // the implementation locale member function to prevent a deadlock

    typedef _RW::__rw_timepunct<char_type>    _TimePunct;
    typedef _RW::__rw_facet_maker<_TimePunct> _FacetMaker;

    const locale::facet &__facet =
        __loc._C_unsafe_use_facet (_TimePunct::id, _TimePunct::_C_ok_implicit,
                                   _TimePunct::_C_facet_cat,
                                   _FacetMaker::_C_maker_func);

    _C_timp = _RWSTD_STATIC_CAST (const _TimePunct*, &__facet);
}


_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    time_put<char, ostreambuf_iterator<char, char_traits<char> > >);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    time_put<wchar_t,
        ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >);

#endif   // _RWSTD_NO_WCHAR_T


// 22.2.5.4
template <class _CharT,
    class _OutputIterator _RWSTD_COMPLEX_DEFAULT (ostreambuf_iterator<_CharT>) >
class time_put_byname: public time_put<_CharT, _OutputIterator>
{
public:
    _EXPLICIT time_put_byname (const char *__name, size_t __refs = 0)
        : time_put<_CharT, _OutputIterator>(__refs) { 
        this->_C_name = __name;
    }
};

_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)


#ifndef _RWSTD_NO_FUNC_PARTIAL_SPEC

template <class _CharT, class _InputIterator>
inline _STD::time_get<_CharT, _InputIterator>*
__rw_create_named_facet (_STD::time_get<_CharT, _InputIterator>*, 
                         const char *__name, size_t __refs)
{
    return new _STD::time_get_byname<_CharT, _InputIterator>(__name, __refs);
}


template <class _CharT, class _OutputIterator>
inline _STD::time_put<_CharT, _OutputIterator>*
__rw_create_named_facet (_STD::time_put<_CharT, _OutputIterator>*, 
                         const char *__name, size_t __refs)
{
    return new _STD::time_put_byname<_CharT, _OutputIterator>(__name, __refs);
}


#else   // _RWSTD_NO_FUNC_PARTIAL_SPEC


#define _RWSTD_TIME(which, buf, _CharT) \
_STD::which <_CharT, _STD::buf##_iterator <_CharT, _STD::char_traits<_CharT> > >

_RWSTD_SPECIALIZED_FUNCTION 
inline _RWSTD_TIME (time_get, istreambuf, char)*
__rw_create_named_facet (_RWSTD_TIME (time_get, istreambuf, char)*, 
                         const char *__name, size_t __refs)
{
    return new _RWSTD_TIME (time_get_byname, istreambuf, char)(__name, __refs);
}


_RWSTD_SPECIALIZED_FUNCTION
inline _RWSTD_TIME (time_put, ostreambuf, char)*
__rw_create_named_facet (_RWSTD_TIME (time_put, ostreambuf, char)*, 
                         const char *__name, size_t __refs)
{
    return new _RWSTD_TIME (time_put_byname, ostreambuf, char)(__name, __refs);
}


#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_SPECIALIZED_FUNCTION 
inline _RWSTD_TIME (time_get, istreambuf, wchar_t)*
__rw_create_named_facet (_RWSTD_TIME (time_get, istreambuf, wchar_t)*, 
                            const char *__name, size_t __refs)
{
    return new
        _RWSTD_TIME (time_get_byname, istreambuf, wchar_t)(__name, __refs);
}


_RWSTD_SPECIALIZED_FUNCTION
inline _RWSTD_TIME (time_put, ostreambuf, wchar_t)*
__rw_create_named_facet (_RWSTD_TIME (time_put, ostreambuf, wchar_t)*, 
                         const char *__name, size_t __refs)
{
    return new
        _RWSTD_TIME (time_put_byname, ostreambuf, wchar_t)(__name, __refs);
}

#undef _RWSTD_TIME


#endif   // _RWSTD_NO_WCHAR_T

#endif   // _RWSTD_NO_FUNC_PARTIAL_SPEC


_RWSTD_NAMESPACE_END   // __rw


#if _RWSTD_DEFINE_TEMPLATE (TIME)
#  include <rw/_time.cc>
#endif


#endif   // _RWSTD_TIME_H_INCLUDED
