/***************************************************************************
 *
 * _money.h - Definitions of the money facets
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _money.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_MONEY_H_INCLUDED
#define _RWSTD_MONEY_H_INCLUDED


#include <rw/_codecvt.h>
#include <rw/_punct.h>
#include <rw/_locale.h>
#include <rw/_defs.h>


_RWSTD_NAMESPACE_BEGIN (std)


struct money_base
{
    enum part { none, space, symbol, sign, value };
    struct pattern { char field[4]; };
};


_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)

template <class _CharT>
class __rw_money_handler_base_1;

template <class _CharT>
class __rw_money_reader_base_1;

template <class _CharT>
class __rw_money_writer_base_1;

template <class _CharT, class _OutputIter>
class __rw_money_reader_base_2;

template <class _CharT, class _OutputIter>
class __rw_money_writer_base_2;

// --------------------------------------------------------------
// Implementation class template -- __rw_moneypunct_init_data<_CharT>.
// --------------------------------------------------------------

// Structure used to initialize a rwstd::__rw_moneypunct_impl_data<_CharT>.

template <class _CharT>
class __rw_moneypunct_init_data
{
public:
    bool          _C_del;    // Delete when initialization is done
    _CharT        _C_dp,
                  _C_ts;     // Decimal point and thousands separator
    const char   *_C_gr;     // Grouping pattern
    const _CharT *_C_cs;     // Currency symbol string
    const _CharT *_C_ps,
                 *_C_ns;     // Positive and negative sign strings
    int           _C_fd;     // Number of fractional digits

    _STD::money_base::pattern _C_pf; // Positive format pattern
    _STD::money_base::pattern _C_nf; // Negative format pattern
};

// ---------------------------------------------
// Implementation class -- __rw_moneypunct_impl_base.
// ---------------------------------------------

// Contains parts of __rw_moneypunct_impl_data<_CharT> that
// do not depend on the template parameter.

class _RWSTD_EXPORT __rw_moneypunct_impl_base
{
public:
    static const _RW::__rw_moneypunct_init_data<char>*
    _C_get_named_init (const char*, bool);
};

// -------------------------------------------------------------
// Implementation class template -- __rw_moneypunct_impl_data<_CharT>.
// -------------------------------------------------------------

// moneypunct<_CharT, _Intl> derives from this (via
// __rw_moneypunct_impl<_CharT, _Intl>) to get its private data members.

template <class _CharT>
class __rw_moneypunct_impl_data
    : public __rw_moneypunct_impl_base, 
      public __rw_punct_data<_CharT>
{
    friend class __rw_money_handler_base_1<_CharT>;

    friend class _STD::moneypunct<_CharT, false>;
    friend class _STD::moneypunct<_CharT, true>;

    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, _STD::allocator<_CharT> >
    string_type;

    string_type _C_cs;           // Currency symbol
    string_type _C_ps,           // Positive and negative sign strings
                _C_ns;  
    int         _C_fd;           // Number of fractional digits

    // Positive and negative format patterns
    _STD::money_base::pattern _C_pf, _C_nf;

protected:
    __rw_moneypunct_impl_data (const __rw_moneypunct_init_data<_CharT>*);

    const __rw_moneypunct_init_data<_CharT>*
    _C_get_init_by_name_ (const char*, bool);
};

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_moneypunct_impl_data<char>);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_moneypunct_impl_data<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_T


// ------------------------------------------------------------------
// Implementation class template -- __rw_money_handler_base_1<_CharT>.
// ------------------------------------------------------------------

// Contains common __rw_money_reader and __rw_money_writer functionality that depends
// only on the _CharT template parameter.

template <class _CharT>
class __rw_money_handler_base_1
{
    const __rw_moneypunct_impl_data<_CharT> &_C_mpunct;

protected:

    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, _STD::allocator<_CharT> >
    string_type;

    __rw_money_handler_base_1 (const __rw_moneypunct_impl_data<_CharT> &__p)
        : _C_mpunct(__p) { }

    static const __rw_moneypunct_impl_data<_CharT>&
    _C_get_punct_data (const locale &__loc, bool __intl);

    const _STD::money_base::pattern& _C_get_pos_format() const {
        return _C_mpunct._C_pf;
    }

    const _STD::money_base::pattern& _C_get_neg_format() const {
        return _C_mpunct._C_nf;
    }

    const string_type &_C_get_positive_sign () const {
        return _C_mpunct._C_ps;
    }

    const string_type &_C_get_negative_sign () const {
        return _C_mpunct._C_ns;
    }

    const string_type &_C_get_curr_symbol () const {
        return _C_mpunct._C_cs;
    }

    int _C_get_frac_digits () const {
        return _C_mpunct._C_fd;
    }
};

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_money_handler_base_1<char>);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_money_handler_base_1<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_T


// -----------------------------------------------------------------
// Implementation class template -- __rw_money_reader_base_1<_CharT>.
// -----------------------------------------------------------------

// Contains parts of __rw_money_reader<_CharT, _InputIter>
// that depend only on the _CharT template parameter.

template <class _CharT>
class __rw_money_reader_base_1
    : public __rw_money_handler_base_1<_CharT>
{
protected:

    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, _STD::allocator<_CharT> >
    string_type;

    __rw_digit_reader_base_1<_CharT> &reader;

    __rw_money_reader_base_1 (__rw_digit_reader_base_1<_CharT> &__r, 
        const __rw_moneypunct_impl_data<_CharT> &mp)
        : __rw_money_handler_base_1<_CharT>(mp), reader(__r)
        {}

public:
    void _C_get_money_string (string_type&, const char*);
};

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_money_reader_base_1<char>);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_money_reader_base_1<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_T


// ---------------------------------------------------------------------------
// Implementation class template - __rw_money_reader_base_2<_CharT, _InputIter>
// ---------------------------------------------------------------------------

// Contains parts of __rw_money_reader<_CharT, _InputIter> that do not depend
// on the intl constructor parameter.

template <class _CharT, class _InputIter>
class __rw_money_reader_base_2
    : public __rw_digit_reader<_CharT, _InputIter>, 
      public __rw_money_reader_base_1<_CharT>
{
protected:
    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, _STD::allocator<_CharT> >
    string_type;

    __rw_money_reader_base_2 (_InputIter&, _InputIter&, ios_base&, 
                              const __rw_moneypunct_impl_data<_CharT>&);

    __rw_digit_reader_base_1<_CharT> &_C_this_as_digit_reader () {
        return *this;
    }

public:
    char *_C_get_money_digits (void);    // Get monetary-format digits
};

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    __rw_money_reader_base_2<char,
        _STD::istreambuf_iterator<char, _STD::char_traits<char> > >
);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    __rw_money_reader_base_2<wchar_t,
        _STD::istreambuf_iterator<wchar_t, _STD::char_traits<wchar_t> > >
);

#endif   // _RWSTD_NO_WCHAR_T


// ------------------------------------------------------------------------
// Implementation class template -- __rw_money_reader<_CharT, _InputIter>.
// ------------------------------------------------------------------------

template <class _CharT, class _InputIter>
class __rw_money_reader
    : public __rw_money_reader_base_2<_CharT, _InputIter>
{
public:
    __rw_money_reader (_InputIter &__begin, _InputIter &__end, 
                       _STD::ios_base &__flags, bool __intl)
        : __rw_money_reader_base_2<_CharT, _InputIter> (__begin, __end, __flags,
        __rw_money_handler_base_1<_CharT>::_C_get_punct_data(__flags.getloc(),
                                                             __intl))
        { }
};


// -----------------------------------------------------------------
// Implementation class template -- __rw_money_writer_base_1<_CharT>.
// -----------------------------------------------------------------

// Contains parts of __rw_money_writer<_CharT, _Intl, _OutputIter>
// that depend only on the _CharT template parameter.

template <class _CharT>
class __rw_money_writer_base_1
    : public __rw_money_handler_base_1<_CharT>
{
public:

    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, _STD::allocator<_CharT> >
    string_type;

    void _C_put_money (_CharT __fill);
    void _C_put_money (const string_type&, _CharT __fill);
    
protected:

    __rw_digit_writer_base_1<_CharT> &writer;

    __rw_money_writer_base_1 (__rw_digit_writer_base_1<_CharT> &__w, 
                         const __rw_moneypunct_impl_data<_CharT> &__mp)
    : __rw_money_handler_base_1<_CharT>(__mp), writer(__w)
        { }

    virtual void
    _C_put_money_sub (const _CharT*, const _CharT*, bool, _CharT) = 0;
};

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_money_writer_base_1<char>);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_money_writer_base_1<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_T

// ---------------------------------------------------------------------------
// Implementation class template  __rw_money_writer_base_2<_CharT, _OutputIter>
// ---------------------------------------------------------------------------

// Contains parts of __rw_money_writer<_CharT, _OutputIter>
// that do not depend on the intl constructor parameter.

template <class _CharT, class _OutputIter>
class __rw_money_writer_base_2
    : public __rw_digit_writer<_CharT, _OutputIter>, 
      public __rw_money_writer_base_1<_CharT>
{
protected:
    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, _STD::allocator<_CharT> >
    string_type;

    __rw_money_writer_base_2 (_OutputIter &__it, _STD::ios_base &__flags, 
                              const __rw_moneypunct_impl_data<_CharT> &__mp)
        : __rw_digit_writer<_CharT, _OutputIter> (__it, __flags, __mp), 
          __rw_money_writer_base_1<_CharT> (this_as_digit_writer(), __mp)
        { }

    
    __rw_digit_writer_base_1<_CharT> &this_as_digit_writer () {
        return *this;
    }

    virtual void _C_put_money_sub (const _CharT*, const _CharT*, bool, _CharT);
};

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    __rw_money_writer_base_2<char,
        _STD::ostreambuf_iterator<char, _STD::char_traits<char> > >
);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    __rw_money_writer_base_2<wchar_t,
        _STD::ostreambuf_iterator<wchar_t, _STD::char_traits<wchar_t> > >
);

#endif   // _RWSTD_NO_WCHAR_T


// -------------------------------------------------------------------------
// Implementation class template -- __rw_money_writer<_CharT, _OutputIter>.
// -------------------------------------------------------------------------

template <class _CharT, class _OutputIter>
class __rw_money_writer
    : public __rw_money_writer_base_2<_CharT, _OutputIter>
{
public:
    __rw_money_writer (_OutputIter &__it, _STD::ios_base &__flags, bool __intl)
        : __rw_money_writer_base_2<_CharT, _OutputIter> (__it, __flags,
        __rw_money_handler_base_1<_CharT>::_C_get_punct_data(__flags.getloc(),
                                                             __intl))
        { }
};


template <class _CharT>
const __rw_moneypunct_init_data<_CharT>*
__rw_fixup_moneypunct_init (const __rw_moneypunct_init_data<char>*, _CharT*);


_RWSTD_SPECIALIZED_FUNCTION
inline const __rw_moneypunct_init_data<char>*
__rw_fixup_moneypunct_init (const __rw_moneypunct_init_data<char> *__mp, char*)
{
    return __mp;
}

// ------------------------------------------------------------------
// Implementation class template -- __rw_moneypunct_impl<_CharT, _Intl>.
// ------------------------------------------------------------------

// moneypunct<_CharT, _Intl> derives from this to get the parts of its behavior
// that are specialized for the character type and the _Intl format selection.

template <class _CharT, bool _Intl>
class __rw_moneypunct_impl
    : public __rw_moneypunct_impl_data<_CharT>
{
protected:
    __rw_moneypunct_impl ();
};


_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT __rw_moneypunct_impl<char, false>
    : public __rw_moneypunct_impl_data<char>
{
    friend class __rw_moneypunct_impl_base;

protected:

    typedef __rw_moneypunct_impl_data<char> parent_type;

    static const __rw_moneypunct_init_data<char> _C_ivals;

    __rw_moneypunct_impl (const __rw_moneypunct_init_data<char> *__mp =
                          &_C_ivals)
        : parent_type (__mp) { }
};


_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT __rw_moneypunct_impl<char, true>
    : public __rw_moneypunct_impl_data<char>
{
    friend class __rw_moneypunct_impl_base;

protected:

    typedef __rw_moneypunct_impl_data<char> parent_type;

    static const __rw_moneypunct_init_data<char> _C_ivals;

    __rw_moneypunct_impl (const __rw_moneypunct_init_data<char> *__mp =
                          &_C_ivals)
        : parent_type(__mp) { }
};


#ifndef _RWSTD_NO_WCHAR_T
_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT __rw_moneypunct_impl<wchar_t, false>
    : public __rw_moneypunct_impl_data<wchar_t>
{
protected:
    typedef __rw_moneypunct_impl_data<wchar_t> parent_type;

    static const __rw_moneypunct_init_data<wchar_t> _C_ivals;

    __rw_moneypunct_impl (const __rw_moneypunct_init_data<wchar_t> *__mp =
                          &_C_ivals)
        : parent_type(__mp) { }
};

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT __rw_moneypunct_impl<wchar_t, true>
    : public __rw_moneypunct_impl_data<wchar_t>
{
protected:
    typedef __rw_moneypunct_impl_data<wchar_t> parent_type;

    static const __rw_moneypunct_init_data<wchar_t> _C_ivals;

    __rw_moneypunct_impl (const __rw_moneypunct_init_data<wchar_t> *__mp =
                          &_C_ivals)
        : parent_type(__mp) { }
};

#endif // _RWSTD_NO_WCHAR_T


_RWSTD_NAMESPACE_END   // __rw


_RWSTD_NAMESPACE_BEGIN (std)


// ---------------------------------------------------------------
// Standard money parsing facet -- money_get<_CharT, _InputIter>.
// ---------------------------------------------------------------

template <class _CharT,
    class _InputIter _RWSTD_COMPLEX_DEFAULT (istreambuf_iterator<_CharT>) >
class money_get: public locale::facet
{
public:
    typedef _CharT     char_type;
    typedef _InputIter iter_type;
    typedef basic_string<_CharT, char_traits<_CharT>, allocator<_CharT> >
    string_type;

    _EXPLICIT money_get (size_t __ref=0)
        : locale::facet(__ref, locale::monetary) { }

    iter_type get (iter_type __begin, iter_type __end, bool __intl,
                   ios_base& __flags, ios_base::iostate& __err,
                   long double& __units) const {
        return do_get(__begin, __end, __intl, __flags, __err, __units);
    }

    iter_type get (iter_type __begin, iter_type __end, bool __intl,
                   ios_base& __flags, ios_base::iostate& __err,
                   string_type &__digits) const {
        return do_get (__begin, __end, __intl, __flags, __err, __digits);
    }

    static locale::id id;

    // Implementation:
    enum { _C_facet_cat = locale::monetary, _C_ok_implicit = 1 };

protected:

    virtual iter_type do_get (iter_type, iter_type,  bool, ios_base&, 
                              ios_base::iostate&, long double&) const;
    virtual iter_type do_get (iter_type, iter_type,  bool, ios_base&, 
                              ios_base::iostate&, string_type&) const;
private:

#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    locale::id &_C_get_id () const { return id; }
#endif
};

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    money_get<char, istreambuf_iterator<char, char_traits<char> > >
);

#ifndef _RWSTD_NO_WCHAR_T
_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    money_get<wchar_t, istreambuf_iterator<wchar_t, char_traits<wchar_t> > >
);
#endif   // _RWSTD_NO_WCHAR_T

// -------------------------------------------------------------------
// Standard money formatting facet -- money_put<_CharT, _OutputIter>.
// -------------------------------------------------------------------

template <class _CharT,
    class _OutputIter _RWSTD_COMPLEX_DEFAULT (ostreambuf_iterator<_CharT>) >
class money_put: public locale::facet
{
public:

    typedef _CharT char_type;
    typedef _OutputIter iter_type;
    typedef basic_string<_CharT, char_traits<_CharT>, allocator<_CharT> >
    string_type;

    _EXPLICIT money_put (size_t __ref=0)
        : locale::facet(__ref, locale::monetary) { }

#ifndef _RWSTD_NO_LONG_DOUBLE
    iter_type put (iter_type __it, bool __intl, ios_base& __flags,
                   char_type __fill, long double __quant) const
#else
        iter_type put (iter_type __it, bool __intl, ios_base& __flags,
                       char_type __fill, double __quant) const
#endif
        { return do_put (__it, __intl, __flags, __fill, __quant); }

    iter_type put (iter_type __it, bool __intl, ios_base& __flags,
                   char_type __fill, const string_type& __digits) const {
        return do_put(__it, __intl, __flags, __fill, __digits);
    }
    
    static locale::id  id;

    // Implementation:
    enum { _C_facet_cat = locale::monetary, _C_ok_implicit = 1 };

protected:

#ifndef _RWSTD_NO_LONG_DOUBLE
    virtual iter_type do_put (iter_type, bool, ios_base&, 
                              char_type, long double) const __softfp;
#else
    virtual iter_type do_put (iter_type, bool, ios_base&, 
                              char_type, double) const __softfp;
#endif

    virtual iter_type do_put (iter_type, bool, ios_base&, 
                              char_type, const string_type&) const;

private:
#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    locale::id &_C_get_id (void) const { return id; }
#endif
};


_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    money_put<char, ostreambuf_iterator<char, char_traits<char> > >
);

#ifndef _RWSTD_NO_WCHAR_T
_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    money_put<wchar_t, ostreambuf_iterator<wchar_t, char_traits<wchar_t> > >
);
#endif   // _RWSTD_NO_WCHAR_T

// -----------------------------------------------------------
// Standard money punctuation facet -- moneypunct<_CharT, bool>.
// -----------------------------------------------------------

template <class _CharT, bool _Intl _RWSTD_SIMPLE_DEFAULT (false)>
class moneypunct: public locale::facet, public money_base, 
                  public _RW::__rw_moneypunct_impl<_CharT, _Intl>
{
public:
    typedef _CharT char_type;
    typedef basic_string<_CharT, char_traits<_CharT>, allocator<_CharT> >
    string_type;

    _RWSTD_STATIC_CONST (bool, intl = _Intl);

    // The second parameter (i) to the constructor is implementation specific.
    // For portable code, always let it default as shown.
#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE

    _EXPLICIT moneypunct (size_t __ref = 0, 
                          const _RW::__rw_moneypunct_init_data<_CharT>* =
                          moneypunct::_C_get_ivals());
#else

    _EXPLICIT moneypunct (size_t = 0);

    moneypunct (size_t __ref, const _RW::__rw_moneypunct_init_data<_CharT>*);

#endif   // _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE

    char_type    decimal_point() const  { return do_decimal_point(); }
    char_type    thousands_sep() const  { return do_thousands_sep(); }
    string       grouping()      const  { return do_grouping(); }
    string_type  curr_symbol()   const  { return do_curr_symbol(); };
    string_type  positive_sign() const  { return do_positive_sign(); }
    string_type  negative_sign() const  { return do_negative_sign(); }
    int          frac_digits()   const  { return do_frac_digits(); }
    pattern      pos_format()    const  { return do_pos_format(); }
    pattern      neg_format()    const  { return do_neg_format(); }

    static locale::id id;

    // Implementation:
    enum { _C_facet_cat = locale::monetary, _C_ok_implicit = 1 };

protected:

    virtual char_type    do_decimal_point()  const;
    virtual char_type    do_thousands_sep()  const;
    virtual string       do_grouping()       const;
    virtual string_type  do_curr_symbol()    const;
    virtual string_type  do_positive_sign()  const;
    virtual string_type  do_negative_sign()  const;
    virtual int          do_frac_digits()    const;
    virtual pattern      do_pos_format()     const;
    virtual pattern      do_neg_format()     const;

private:
    virtual void _C_initfacet (const locale&);

#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    locale::id &_C_get_id (void) const { return id; }
#endif

public:
    static const _RW::__rw_moneypunct_init_data<_CharT> *_C_get_ivals () {
        return &_RW::__rw_moneypunct_impl<_CharT, _Intl>::_C_ivals;
    }
};

#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE

template <class _CharT, bool _Intl>
inline moneypunct<_CharT, _Intl>::
moneypunct (size_t __ref, const _RW::__rw_moneypunct_init_data<_CharT> *__mp)
    : locale::facet(__ref, locale::monetary),
      _RW::__rw_moneypunct_impl<_CharT, _Intl>(__mp)
{ }

#else   // if defined (_RWSTD_NO_TEMPLATE_ON_RETURN_TYPE)

template <class _CharT, bool _Intl>
inline moneypunct<_CharT, _Intl>::moneypunct (size_t __ref)
    :  locale::facet(__ref, locale::monetary), 
       _RW::__rw_moneypunct_impl<_CharT, _Intl>
              ((_RW::__rw_moneypunct_init_data<_CharT>*)&_RW::
               __rw_moneypunct_impl<_CharT, _Intl>::_C_ivals)
     { }

template <class _CharT, bool _Intl>
inline moneypunct<_CharT, _Intl>::
moneypunct (size_t __ref, const _RW::__rw_moneypunct_init_data<_CharT> *__mp)
    : locale::facet(__ref, locale::monetary), 
      _RW::__rw_moneypunct_impl<_CharT, _Intl>(__mp)
{ }

#endif   // _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE


_RWSTD_INSTANTIATE_2 (class _RWSTD_EXPORT moneypunct<char, false>);
_RWSTD_INSTANTIATE_2 (class _RWSTD_EXPORT moneypunct<char, true>);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_2 (class _RWSTD_EXPORT moneypunct<wchar_t, false>);
_RWSTD_INSTANTIATE_2 (class _RWSTD_EXPORT moneypunct<wchar_t, true>);

#endif   // _RWSTD_NO_WCHAR_T


// --------------------------------------------------------
// Standard derived facet -- moneypunct_byname<_CharT, _Intl>.
// --------------------------------------------------------

template <class _CharT, bool _Intl _RWSTD_SIMPLE_DEFAULT (false)>
class moneypunct_byname: public moneypunct<_CharT, _Intl>
{
    typedef moneypunct<_CharT, _Intl> _Punct;
public:
    _EXPLICIT moneypunct_byname (const char* __name, size_t __ref = 0)
        : moneypunct<_CharT, _Intl>(__ref,
                                    _Punct::_C_get_init_by_name_ (__name,
                                          moneypunct<_CharT, _Intl>::intl))
        { }
};


_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)

_USING (_RWSTD_C::size_t);

#ifndef _RWSTD_NO_FUNC_PARTIAL_SPEC

template <class _CharT, bool _Intl>
inline _STD::moneypunct<_CharT, _Intl>*
__rw_create_named_facet (_STD::moneypunct<_CharT, _Intl>*, 
                         const char *__name, size_t __ref) {

    return new _STD::moneypunct_byname<_CharT, _Intl>(__name, __ref);
}

#else   // if defined (_RWSTD_NO_FUNC_PARTIAL_SPEC)

_RWSTD_SPECIALIZED_FUNCTION
inline _STD::moneypunct<char, false>*
__rw_create_named_facet (_STD::moneypunct<char, false>*, 
                         const char *__name, size_t __ref)
{
    return new _STD::moneypunct_byname<char, false>(__name, __ref);
}

_RWSTD_SPECIALIZED_FUNCTION
inline _STD::moneypunct<char, true>*
__rw_create_named_facet (_STD::moneypunct<char, true>*, 
                         const char *__name, size_t __ref)
{
    return new _STD::moneypunct_byname<char, true>(__name, __ref);
}

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_SPECIALIZED_FUNCTION
inline _STD::moneypunct<wchar_t, false>*
__rw_create_named_facet (_STD::moneypunct<wchar_t, false>*, 
                         const char *__name, size_t __ref)
{
    return new _STD::moneypunct_byname<wchar_t, false>(__name, __ref);
}

_RWSTD_SPECIALIZED_FUNCTION
inline _STD::moneypunct<wchar_t, true>*
__rw_create_named_facet (_STD::moneypunct<wchar_t, true>*, 
                         const char *__name, size_t __ref)
{
    return new _STD::moneypunct_byname<wchar_t, true>(__name, __ref);
}

#endif   // _RWSTD_NO_WCHAR_T

#endif   // _RWSTD_NO_FUNC_PARTIAL_SPEC


_RWSTD_NAMESPACE_END   // __rw


#if _RWSTD_DEFINE_TEMPLATE (MONEY)
#  include <rw/_money.cc>
#endif


#endif   // _RWSTD_MONEY_H_INCLUDED
