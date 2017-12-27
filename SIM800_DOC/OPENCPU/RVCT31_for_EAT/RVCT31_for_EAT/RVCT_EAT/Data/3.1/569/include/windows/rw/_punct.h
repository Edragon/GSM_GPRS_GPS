/****************************************************************************
 *
 * _punct.h - Definitions of the punct and supporting template classes
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _punct.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_PUNCT_H_INCLUDED
#define _RWSTD_PUNCT_H_INCLUDED

#include <rw/_iosbase.h>
#include <rw/_defs.h>

#include _RWSTD_CTIME


_RWSTD_NAMESPACE_BEGIN (std)

template <class _CharT>
class numpunct;

template <class _CharT, bool _Intl>
class moneypunct;

_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)

// ---------------------------------------------------------
// Implementation class template -- __rw_keyword_def<_CharT>.
// ---------------------------------------------------------

// Helper class used in parsing keywords from input_C_indx (such as true/false
// in num_get, month and day names in time_get, etc).

template <class _CharT>
struct __rw_keyword_def
{
    const _CharT *_C_name;
    int           _C_indx;
};

// ---------------------------------------------------------
// Implementation class template -- __rw_keyword_map<_CharT>.
// ---------------------------------------------------------

// Defines a set of keywords to be recognized on input and to be written to
// output.  Private instances are hidden in numpunct (for true/false) and
// rwstd::__rw_timepunct (for month and weekday names).

template <class _CharT>
struct __rw_keyword_map
{
    int                             _C_numdefs;
    const __rw_keyword_def<_CharT> *_C_defs;
};


// --------------------------------------------------------
// Implementation class template -- __rw_punct_data<_CharT>.
// --------------------------------------------------------

template <class _CharT>
class __rw_digit_handler_base_1;

// Common base class for __rw::__rw_numpunct_impl_data and
// __rw::__rw_moneypunct_impl_data.

template <class _CharT>
class __rw_punct_data
{
    friend class __rw_digit_handler_base_1<_CharT>;

public:
    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, _STD::allocator<_CharT> >
    string_type;

protected:

    _CharT _C_dp, _C_ts;
    _STD::string _C_gr;
};

// -----------------------------------------------------------
// Implementation class template -- __rw_numpunct_init_data<_CharT>.
// -----------------------------------------------------------

template <class _CharT>
struct __rw_numpunct_init_data
{
    bool _C_del;                     // Kill the bearer of this message
    _CharT _C_dp, _C_ts;             // Decimal point and thousands separator
    const char *_C_gr;               // Digit grouping rule
    const _CharT *_C_tn, *_C_fn;     // Strings for boolean true and false
};

// ------------------------------------------
// Implementation class -- __rw_numpunct_impl_base.
// ------------------------------------------

// Contains parts of numpunct<_CharT> that don't depend on the _CharT template
// parameter.

class _RWSTD_EXPORT __rw_numpunct_impl_base
{
public:
  static __rw_numpunct_init_data<char> *_C_get_named_init_ (const char*);
};

// -----------------------------------------------------------
// Implementation class template -- __rw_numpunct_impl_data<_CharT>.
// -----------------------------------------------------------

// numpunct<_CharT> derives from this (via __rw::__rw_numpunct_impl) to get its
// private data members.

template <class _CharT>
class __rw_numpunct_impl_data
    : public __rw_numpunct_impl_base, 
      public __rw_punct_data<_CharT>
{
protected:
    inline __rw_numpunct_impl_data (const __rw_numpunct_init_data<_CharT>*);
    static __rw_numpunct_init_data<_CharT> *_C_get_init_by_name_ (const char*);
    void _C_initfacetbase (const locale&);

public:
    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, allocator<_CharT> >
    string_type;

private:
    friend class numpunct<_CharT>;
    friend class __rw_keyword_cracker<_CharT>;

    string_type _C_tn, _C_fn;

  __rw_keyword_def<_CharT> tf_defs_[2];
  __rw_keyword_map<_CharT> tf_map_;

};


template <class _CharT>
inline
__rw_numpunct_impl_data<_CharT>::
__rw_numpunct_impl_data (const __rw_numpunct_init_data<_CharT> *__np)
{
    if (!__np) {
        this->_C_dp = _CharT ('.');
        this->_C_ts = _CharT (',');
    } else {
        this->_C_dp = __np->_C_dp;
        this->_C_ts = __np->_C_ts;
        this->_C_gr = __np->_C_gr;
        _C_tn       = __np->_C_tn;
        _C_fn       = __np->_C_fn;

        if (__np->_C_del)
            delete[] _RWSTD_CONST_CAST (char*,
                                        _RWSTD_REINTERPRET_CAST (const char*,
                                                                 __np));
    }
}

template <class _CharT>
__rw_numpunct_init_data<_CharT>*
__rw_fixup_numpunct_init (__rw_numpunct_init_data<char>*, _CharT*);

_RWSTD_SPECIALIZED_FUNCTION
inline __rw_numpunct_init_data<char>*
__rw_fixup_numpunct_init (__rw_numpunct_init_data<char> *__np, char*)
{
    return __np;
}

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_numpunct_impl_data<char>);

#ifndef _RWSTD_NO_WCHAR_TYPE

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_numpunct_impl_data<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_TYPE


// ------------------------------------------------------------
// Implementation class template -- __rw_numpunct_impl<_CharT>.
// ------------------------------------------------------------

// numpunct<_CharT> derives from this to obtain the part of its behavior that
// must be specialized for char and wchar_t.  This lets us avoid specializing
// the whole numpunct<_CharT> template.  Currently the only specialized behavior
// is the initialization of private data members in the constructor.

template <class _CharT>
class __rw_numpunct_impl
    : public __rw_numpunct_impl_data<_CharT>
{
    static const __rw_numpunct_init_data<_CharT> _C_ivals;

protected:

    __rw_numpunct_impl (const __rw_numpunct_init_data<_CharT>* __np)
        : __rw_numpunct_impl_data<_CharT>(__np) { }

public:

    static const __rw_numpunct_init_data<_CharT> *_C_get_ivals () {
        return &_C_ivals;
    }
};


template <class _CharT>
struct __rw_bool_name
{
    static const _CharT _C_name[][6];
};


// __rw_bool_name explicitly instantiated to work around a g++ 2.95.2 bug
// on AIX (and a few other platforms) where it "forgets" to do so implicitly
_RWSTD_INSTANTIATE_1 (struct _RWSTD_EXPORT __rw_bool_name<char>);
_RWSTD_INSTANTIATE_1 (class  _RWSTD_EXPORT __rw_numpunct_impl<char>);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_1 (struct _RWSTD_EXPORT __rw_bool_name<wchar_t>);
_RWSTD_INSTANTIATE_1 (class  _RWSTD_EXPORT __rw_numpunct_impl<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_T


// -------------------------------------------------------------
// Implementation class template -- __rw_timepunct_init_data<_CharT>.
// -------------------------------------------------------------

// Structure used to initialize the private data members of
// __rw_timepunct<_CharT>.
template <class _CharT>
class __rw_timepunct_init_data
{
public:
  const _CharT *_C_day_nms[7][2];   // Day names and abbreviations
  const _CharT *_C_month_nms[12][2];// Month names and abbreviations
  const _CharT *_C_ampm_nms[2];     // AM/PM strings
  const _CharT *_C_bad_nm;          // Bad value output signal
  const _CharT *_C_ord_nms[100];    // Ordinal numbers
  const _CharT *_C_dt_tm_pats[6];   // Patterns for formats "xXcDrT" resp.
};

template <class _CharT>
class __rw_timepunct_init;

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT __rw_timepunct_init<char> 
{
public:
    typedef _STD::char_traits<char> traits_type;
    typedef _STD::basic_string<char, traits_type, _STD::allocator<char> >
    string_type;

  const __rw_timepunct_init_data<char>& _C_tinit;

  __rw_timepunct_init (const __rw_timepunct_init_data<char> &__np)
    : _C_tinit (__np) 
  {}

  const string_type _C_get_day (int, bool) const;
  const string_type _C_get_month (int, bool) const;
  const string_type _C_get_ampm (bool) const;
  const string_type _C_get_bad() const  {
      return _C_tinit._C_bad_nm;
  }

  const string_type _C_get_ord(int __i) const {
      return _C_tinit._C_ord_nms[__i];
  }

  const string_type _C_get_pattern(int __i) const {
      return _C_tinit._C_dt_tm_pats[__i];
  }
};

#ifndef _RWSTD_NO_WCHAR_T
_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT __rw_timepunct_init<wchar_t> 
{

public:
    typedef _STD::char_traits<wchar_t> traits_type;
    typedef basic_string<wchar_t, traits_type, _STD::allocator<wchar_t> >
    string_type;

  const __rw_timepunct_init_data<wchar_t>& _C_tinit;

  __rw_timepunct_init (const __rw_timepunct_init_data<wchar_t> &__np)
    : _C_tinit(__np) 
  {}

  const string_type _C_get_day(int __i, bool __abr) const;
  const string_type _C_get_month(int __i, bool __abr) const;
  const string_type _C_get_ampm(bool __pm) const;

#if 0

  const string_type _C_get_day (int __i, bool __abr) const {
      return _C_tinit._C_day_nms[__i][__abr];
  }

  const string_type _C_get_month(int __i, bool __abr) const {
      return _C_tinit._C_month_nms[__i][__abr];
  }

  const string_type _C_get_ampm(bool __pm) const {
      return _C_tinit._C_ampm_nms[__pm];
  }

#endif   // 0/1


  const string_type _C_get_bad() const  {
      return _C_tinit._C_bad_nm;
  }

  const string_type _C_get_ord(int __i) const {
      return _C_tinit._C_ord_nms[__i];
  }

  const string_type _C_get_pattern(int __i) const {
      return _C_tinit._C_dt_tm_pats[__i];
  }
};
#endif // _RWSTD_NO_WCHAR_T


template <class _CharT>
class __rw_timepunct_init
{

public:
    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, _STD::allocator<_CharT> >
    string_type;

    const __rw_timepunct_init_data<_CharT>& _C_tinit;

    __rw_timepunct_init (const __rw_timepunct_init_data<_CharT> &__p)
        : _C_tinit(__p) 
        {}

  const string_type _C_get_day(int __i, bool __abr) const {
      return _C_tinit._C_day_nms[__i][__abr];
  }

  const string_type _C_get_month(int __i, bool __abr) const {
      return _C_tinit._C_month_nms[__i][__abr];
  }

  const string_type _C_get_ampm(bool __pm) const {
      return _C_tinit._C_ampm_nms[__pm];
  }

  const string_type _C_get_bad() const  {
      return _C_tinit._C_bad_nm;
  }

  const string_type _C_get_ord(int __i) const {
      return _C_tinit._C_ord_nms[__i];
  }

  const string_type _C_get_pattern(int __i) const {
      return _C_tinit._C_dt_tm_pats[__i];
  }
};

// -------------------------------------------------------------
// Implementation class template -- __rw_timepunct_impl_data<_CharT>.
// -------------------------------------------------------------

// __rw_timepunct<_CharT> derives from this (via rwstd::__rw_timepunct_impl)
// to get its private data members.

template <class _CharT>
class __rw_timepunct_impl_data
{
    friend class __rw_timepunct<_CharT>;
    friend class __rw_keyword_cracker<_CharT>;
public:
    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, _STD::allocator<_CharT> >
    string_type;

private:

  string_type _C_day_nms[7][2];
  string_type _C_month_nms[12][2];
  string_type _C_ampm_nms[2];
  string_type _C_bad_nm;
  string_type _C_ord_nms[100];
  string_type _C_dt_tm_pats[6];

  __rw_keyword_def<_CharT> _C_dn_defs[7][2];
  __rw_keyword_map<_CharT> _C_dn_defsmap;
  __rw_keyword_def<_CharT> _C_mn_defs[12][2];
  __rw_keyword_map<_CharT> _C_mn_defsmap;
  __rw_keyword_def<_CharT> _C_ampm_defs[2];
  __rw_keyword_map<_CharT> _C_ampm_defsmap;

 protected:
  __rw_timepunct_impl_data (const __rw_timepunct_init<_CharT>&);
  void _C_initpat (const _STD::locale&);

  void _C_initfacet (const _STD::locale&);
};

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_timepunct_impl_data<char>);

#ifndef _RWSTD_NO_WCHAR_TYPE

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_timepunct_impl_data<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_TYPE


// -------------------------------------------------------------
// Implementation class template -- __rw_timepunct_impl<_CharT>.
// -------------------------------------------------------------

// __rw_timepunct<_CharT> derives from this to get the parts of its behavior that
// must be specialized on the character type.  Currently the only specialized
// behavior is the initialization logic in the constructor.

template <class _CharT>
class __rw_timepunct_impl
    : public __rw_timepunct_impl_data<_CharT>
{
protected:
  __rw_timepunct_impl(void);
};

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT __rw_timepunct_impl<char>
    : public __rw_timepunct_impl_data<char>
{
    static const __rw_timepunct_init_data<char> _C_ivals;

 protected:
  __rw_timepunct_impl (void):
    __rw_timepunct_impl_data<char>(__rw_timepunct_init<char>(_C_ivals)) { }
};


#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT __rw_timepunct_impl<wchar_t>
    : public __rw_timepunct_impl_data<wchar_t>
{
    static const __rw_timepunct_init_data<wchar_t> _C_ivals;

 protected:
  __rw_timepunct_impl(void):
      __rw_timepunct_impl_data<wchar_t>(__rw_timepunct_init<wchar_t>(_C_ivals)) { }
};
#endif // _RWSTD_NO_WCHAR_T


// ----------------------------------------------------------
// Time and date punctuation facet -- __rw_timepunct<_CharT>.
// ----------------------------------------------------------

template <class _CharT>
class __rw_timepunct : public locale::facet, 
                       public __rw_timepunct_impl<_CharT>
{
public:
    typedef _CharT char_type;
    typedef _STD::char_traits<char_type> traits_type;
    typedef _STD::basic_string<char_type, traits_type,
        _STD::allocator<char_type> >
    string_type;

  _EXPLICIT __rw_timepunct (size_t __refs=0):
      locale::facet(__refs, locale::time) { }

  string_type dayname (int __day, bool __abbreviated) const
      { return do_dayname(__day, __abbreviated); }
  string_type monthname (int __month, bool __abbreviated) const
      { return do_monthname(__month, __abbreviated); }
  string_type ordinal (int __number) const
      { return do_ordinal(__number); }

  static _STD::locale::id id;

  // Implementation:
  enum { _C_facet_cat = locale::time, _C_ok_implicit = 1 };

 protected:

  virtual string_type do_dayname (int day, bool abbreviated) const;
  virtual string_type do_monthname (int month, bool abbreviated) const;
  virtual string_type do_ordinal (int number) const;

 private:
  virtual void _C_initfacet (const _STD::locale&);

#ifdef _RWSTD_NO_MEMBER_TEMPLATES
  _STD::locale::id &_C_get_id (void) const { return id; }
#endif
};

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_timepunct<char>);

#ifndef _RWSTD_NO_WCHAR_TYPE

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_timepunct<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_TYPE


// --------------------------------------------------------------
// Implementation class template -- __rw_keyword_cracker<_CharT>.
// --------------------------------------------------------------

// Gives derived classes a backdoor way to get the private
// __rw_keyword_map and other members out of numpunct and __rw_timepunct facets.

template <class _CharT>
class __rw_keyword_cracker
{
public:

    static const __rw_keyword_map<_CharT>&
    _C_get_tf_map (const numpunct<_CharT> &__np) {
        return __np.tf_map_;
    }

    static const __rw_keyword_map<_CharT>&
    _C_get_day_map (const __rw_timepunct<_CharT>& __p) {
        return __p._C_dn_defsmap;
    }

    static const __rw_keyword_map<_CharT>&
    _C_get_month_map (const __rw_timepunct<_CharT>& __p) {
        return __p._C_mn_defsmap;
    }

    static const __rw_keyword_map<_CharT>&
    _C_get_ampm_map (const __rw_timepunct<_CharT>& __p) {
        return __p._C_ampm_defsmap;
    }

    static const _TYPENAME __rw_numpunct_impl_data<_CharT>::string_type&
    _C_get_tf_string (const __rw_numpunct_impl_data<_CharT>& __np, bool __val) {
        return __val ? __np._C_tn : __np._C_fn;
    }

    static const _TYPENAME __rw_timepunct<_CharT>::string_type&
    _C_get_day_string (const __rw_timepunct<_CharT>& __p, unsigned __val, bool __abr) {
        return __val < 7 ? __p._C_day_nms[__val][__abr] : __p._C_bad_nm;
    }

    static const _TYPENAME __rw_timepunct<_CharT>::string_type&
    _C_get_month_string (const __rw_timepunct<_CharT>& __p, unsigned __val, bool __abr)
{
        return __val < 12 ? __p._C_month_nms[__val][__abr] : __p._C_bad_nm;
    }

    static const _TYPENAME __rw_timepunct<_CharT>::string_type&
    _C_get_ampm_string (const __rw_timepunct<_CharT>& __p, bool __pm) {
        return __p._C_ampm_nms[__pm];
    }

    static const _TYPENAME __rw_timepunct<_CharT>::string_type&
    _C_get_ord_string (const __rw_timepunct<_CharT>& __p, unsigned __val) {
        return __val < 99 ? __p._C_ord_nms[__val] :  __p._C_bad_nm;
    }

    static const _TYPENAME __rw_timepunct<_CharT>::string_type&
    _C_get_patt_string (const __rw_timepunct<_CharT>& __p, unsigned __val) {
        return __val < 6 ? __p._C_dt_tm_pats[__val] : __p._C_bad_nm;
    }
};

// -------------------------------------------------------------------
// Implementation class template -- __rw_digit_handler_base_1<_CharT>.
// -------------------------------------------------------------------

// Contains _CharT-dependent functionality that is common to the digit_reader
// and __rw_digit_writer classes, particularly related to getting access
// to ctype and punct facets they both use.

template <class _CharT>
class __rw_digit_handler_base_1
    : public __rw_keyword_cracker<_CharT>
{
public:

    const _STD::ctype<_CharT>     &_C_ctyp;
    const __rw_digit_map<_CharT>  &_C_dmap;
    const __rw_punct_data<_CharT> &_C_punct;

    // ctor for general numerics, gets punct from loc's numpunct facet
    inline __rw_digit_handler_base_1 (const _STD::locale&);

    // ctor for money, passed __rw_punct_data is in fact a moneypunct facet
    __rw_digit_handler_base_1 (const _STD::locale            &__loc,
                               const __rw_punct_data<_CharT> &__pun)
        : _C_ctyp (_USE_FACET (_STD::ctype<_CharT>, __loc)), 
          _C_dmap (__rw_digit_map<_CharT>::_C_get_digit_map (_C_ctyp)), 
          _C_punct (__pun)
        { }

    _CharT _C_get_decimal_point () const {
        return _C_punct._C_dp;
    }

    _CharT _C_get_thousands_sep () const {
        return _C_punct._C_ts;
    }

    const char* _C_get_grouping () const {
        return _C_punct._C_gr.c_str();
    }
};


#ifndef _RWSTD_INLINE_INSTANTIATION_HACK

template <class _CharT>
inline __rw_digit_handler_base_1<_CharT>::
__rw_digit_handler_base_1 (const locale& __loc)
    : _C_ctyp (_USE_FACET (_STD::ctype<_CharT>, __loc)), 
      _C_dmap (__rw_digit_map<_CharT>::_C_get_digit_map (_C_ctyp)), 
      _C_punct (_USE_FACET (_STD::numpunct<_CharT>, __loc))
{ }

#endif   // _RWSTD_INLINE_INSTANTIATION_HACK


// ------------------------------------------------
// Implementation class -- __rw_digit_reader_base.
// ------------------------------------------------

// Contains digit_reader members that do not depend on the digit_reader
// template parameters, particularly for converting strings of char digits
// to long and floating-point binary values.

class _RWSTD_EXPORT __rw_digit_reader_base : public __rw_digit_map_base
{
public:

    enum {
        _C_good              = 0x0000, 
        _C_no_digits         = 0x0001, 
        _C_too_many_groups   = 0x0004, 
        _C_group_too_long    = 0x0008, 
        _C_group_is_empty    = 0x0010, 
        _C_bad_grouping      = 0x0020, 
        _C_bad_exponent      = 0x0040, 
        _C_overflow          = 0x0080, 
        _C_bad_curr_symbol   = 0x0100, 
        _C_bad_sign          = 0x0200,

        // state bits
        _C_signed         = 0x01000,   // sign allowed in input
        _C_negative       = 0x02000,   // minus sign was present
        _C_exp_negative   = 0x04000,   // minus sign was present on exponent
        _C_advanced       = 0x08000,   // iterator was advanced
        _C_infinity       = 0x10000,   // inf/infinity
        _C_nan            = 0x20000,   // NaN - Not a Number

        _C_DEF_BUFSIZE    = 64         // room for at most this many digits
    };

    // Fields defining digits to be converted, usually filled in by derived
    // class before calling one of the conversion methods below.
    int _C_error;                    // Bit-field
    int _C_base;                  // 2, 8, 10, or 16, 0=not yet determined

    int _C_state;                 // bitmap of state bits

    char *_C_frac_beg;               // Start of fractional part of digits
    char *_C_exp_beg;                // Start of exponent in digits

    size_t _C_bufsize;            // actual size of buffer

    // Binary digits (i.e. 0, 1, 2, not '0', '1', '2')
    char  _C_buffer [_C_DEF_BUFSIZE];   // sufficient in most cases
    char *_C_digits;                    // may be dynamically allocated

    __rw_digit_reader_base (int __radix = 0)
        : _C_error (_C_good), _C_base (__radix), _C_state (_C_signed),
          _C_frac_beg (0), _C_exp_beg (0), _C_bufsize (_C_DEF_BUFSIZE),
          _C_digits (_C_buffer) { }

    ~__rw_digit_reader_base () {
        if (_C_digits != _C_buffer)
            delete[] _C_digits;
    }


    // parse a string of digits into a long
    long _C_parse (const char *__end, long) {
        return _RWSTD_STATIC_CAST (unsigned long, _C_parse (__end, 0UL));
    }

    // parse a string of digits into an unsigned long
    unsigned long _C_parse (const char *__end, unsigned long = 0);

    // overloads provided for genericity
    short _C_parse (const char *__end, short) {
        return _RWSTD_STATIC_CAST (short, _C_parse (__end, 0L));
    }

    unsigned short _C_parse (const char *__end, unsigned short) {
        return _RWSTD_STATIC_CAST (unsigned short, _C_parse (__end, 0UL));
    }

    int _C_parse (const char *__end, int) {
        return _RWSTD_STATIC_CAST (int, _C_parse (__end, 0L));
    }

    unsigned int _C_parse (const char *__end, unsigned int) {
        return _RWSTD_STATIC_CAST (unsigned int, _C_parse (__end, 0UL));
    }

#ifdef _RWSTD_LONG_LONG

    unsigned _RWSTD_LONG_LONG _C_parse (const char *__end, 
                                        unsigned _RWSTD_LONG_LONG);

    _RWSTD_LONG_LONG _C_parse (const char *__end, _RWSTD_LONG_LONG) {
        return _RWSTD_STATIC_CAST (unsigned _RWSTD_LONG_LONG,
                                _C_parse (__end, (unsigned _RWSTD_LONG_LONG)0));
  }

#endif   // _RWSTD_LONG_LONG

    // _C_parse a string of digits and interpret it as a pointer
    void *_C_parse (const char *__end, void* __val);

    float _C_parse (const char *__end, float __val, const char *__fmt = "%g") 
        __softfp {
        return _C_parse_float (__end, __fmt, &__val), __val;
    }
        
    double _C_parse (const char *__end, double __val,
                     const char *__fmt = "%lg") __softfp {
        return _C_parse_float (__end, __fmt, &__val), __val;
    }

    long double _C_parse (const char *__end, long double __val, 
                          const char *__fmt =
                          "%" _RWSTD_LDBL_SCANF_PREFIX "g") __softfp {
        // careful with format specifier on platform where
        // sizeof (double) == sizeof (long double)
        return _C_parse_float (__end, __fmt, &__val), __val;
    }

protected:

    // reallocates a buffer when the existing one fills up
    static char* _C_realloc (char*&, char*, _RWSTD_C::size_t&);

    // reallocates digit buffer when the existing one fills up
    char* _C_realloc ();

private:

    // parse a string of decimal only digits into an unsigned long
    unsigned long _C_parse_decimal (const char *__beg, const char *__end);

    // parse a string of digits and interpret it as a floating point value
    void _C_parse_float (const char *__end, const char *__fmt, void *__val);
};


inline char* __rw_digit_reader_base::_C_realloc ()
{
    // compute offsets into buffer
    _RWSTD_C::ptrdiff_t __frac_off = _C_frac_beg - _C_digits;
    _RWSTD_C::ptrdiff_t __exp_off  = _C_exp_beg  - _C_digits;

    char *__cur = _C_realloc (this->_C_digits, this->_C_buffer,
                              this->_C_bufsize);

    // adjust pointers into buffer
    if (_C_frac_beg)
        _C_frac_beg = _C_digits + __frac_off;

    if (_C_exp_beg)
        _C_exp_beg = _C_digits + __exp_off;

    return __cur;
}

// ------------------------------------------------------------------
// Implementation class template -- __rw_digit_reader_base_1<_CharT>.
// ------------------------------------------------------------------

// Contains parts of digit_reader that depend on _CharT but not on the iterator
// type.

template <class _CharT>
class __rw_digit_reader_base_1
    : public __rw_digit_reader_base, 
      public __rw_digit_handler_base_1<_CharT>
{
public:
    typedef _CharT                       char_type;
    typedef _STD::char_traits<char_type> traits_type;
    typedef _STD::basic_string<char_type, traits_type,
        _STD::allocator<char_type> >
    string_type;

protected:
    // Constructor for general numerics, gets punct from loc's numpunct<_CharT>.
    __rw_digit_reader_base_1 (const _STD::locale &__loc, int __radix = 0)
        : __rw_digit_reader_base (__radix),
          __rw_digit_handler_base_1<_CharT>(__loc) { }


    // Constructor for use by derived __rw_money_reader, caller passes punct,
    // which in fact is loc's moneypunct<_CharT, true|false> facet.
    __rw_digit_reader_base_1 (const _STD::locale            &__loc,
                              const __rw_punct_data<_CharT> &__mp)
        : __rw_digit_handler_base_1<_CharT>(__loc, __mp) { }
};

// ---------------------------------------------------------------------
// Implementation class template -- digit_reader<_CharT, _InputIter>.
// ---------------------------------------------------------------------

// Facets that do numeric input use temporary objects of this class to
// extract keywords and strings of digits and punctuation from the input
// stream.

template <class _CharT, class _InputIter>
class __rw_digit_reader
    : public __rw_digit_reader_base_1<_CharT>
{
public:
    // Constructor for reading general numerics.
    __rw_digit_reader (_InputIter     &__first,
                       _InputIter     &__last,
                       _STD::ios_base &__flags)
        : __rw_digit_reader_base_1<_CharT> (__flags.getloc (),
                                            __flags._C_base ()),
          _C_io (__flags), _C_in (__first), _C_end (__last) { }

protected:
    // Constructor for use by derived __rw_money_reader class.
    __rw_digit_reader (_InputIter &__first, _InputIter &__last,
                       _STD::ios_base &__flags,
                       const __rw_punct_data<_CharT> &__mp)
        : __rw_digit_reader_base_1<_CharT>(__flags.getloc (), __mp), 
          _C_io (__flags), _C_in (__first), _C_end (__last) { }

public:
    char *_C_get_digits (int);        // Get integer-format digits

    // overloads provided for genericity
    char *_C_get_digits (short) {
        return _C_get_digits (0);
    }

    char *_C_get_digits (unsigned short) {
        return _C_get_digits (0);
    }

    char *_C_get_digits (unsigned int) {
        return _C_get_digits (0);
    }

    char *_C_get_digits (long) {
        return _C_get_digits (0);
    }

    char *_C_get_digits (unsigned long) {
        return _C_get_digits (0);
    }

#ifdef _RWSTD_LONG_LONG

    char *_C_get_digits (_RWSTD_LONG_LONG) {
        return _C_get_digits (0);
    }

    char *_C_get_digits (unsigned _RWSTD_LONG_LONG) {
        return _C_get_digits (0);
    }


#endif   // _RWSTD_LONG_LONG

    char *_C_get_digits (double) __softfp;     // Get floating point-format digits

    // overloads provided for genericity
    char *_C_get_digits (float) {
        return _C_get_digits (0.0);
    }

    char *_C_get_digits (long double) {
        return _C_get_digits (0.0);
    }

    char *_C_get_digits (void*) {      // Get pointer-format digits
        this->_C_base = 16;
        return _C_get_digits (0);
    }

    // Parse keyword from input, return associated integer value
    int _C_get_keyword (const __rw_keyword_map<_CharT>&);

    bool eof () const {
        return _C_in == _C_end;
    }

protected:

    _STD::ios_base &_C_io;
    _InputIter     &_C_in;
    _InputIter     &_C_end;

    const char *_C_grouping;
    _CharT thousands_sep;
    char _C_groups[40];
    char *_C_gpos;

    // get a string of digits optionally discarding leading zeros
    char *_C_get_digit_string (char*, bool = false);

    char *_C_get_digit_groups (char*);       // Get grouped digits (recursive)
};

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    __rw_digit_reader<char,
        _STD::istreambuf_iterator<char, _STD::char_traits<char> > >
);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    __rw_digit_reader<wchar_t,
        _STD::istreambuf_iterator<wchar_t, _STD::char_traits<wchar_t> > >
);

#endif   // _RWSTD_NO_WCHAR_T


// ------------------------------------------------
// Implementation class -- __rw_digit_writer_base.
// ------------------------------------------------

// Contains the parts of __rw_digit_writer that do not depend on the template
// parameters.

class _RWSTD_EXPORT __rw_digit_writer_base
    : public __rw_digit_map_base
{
public:
    __rw_digit_writer_base (_STD::ios_base&);

    virtual ~__rw_digit_writer_base () {
        if (_C_start != _C_buffer)
            delete[] _C_start;
    }

    void _C_get_pattern (char*, const char*, bool);

    bool _C_get_f_pattern(char*, char);

    void _C_digitize (unsigned long, bool __is_signed, const char *__fmat);

    void _C_digitize (const void *__val);

    void _C_digitize (unsigned short __val) {
        _C_digitize (_RWSTD_STATIC_CAST (unsigned long, __val), false, "h");
    }

    void _C_digitize (short __val) {
        _C_digitize (_RWSTD_STATIC_CAST (unsigned long, __val), true, "h");
    }

    void _C_digitize (unsigned int __val) {
        _C_digitize (_RWSTD_STATIC_CAST (unsigned long, __val), false, "");
    }

    void _C_digitize (int __val) {
        _C_digitize (_RWSTD_STATIC_CAST (unsigned long, __val), true, "");
    }

    void _C_digitize (long __val) {
        _C_digitize (_RWSTD_STATIC_CAST (unsigned long, __val), true, "l");
    }

    void _C_digitize (unsigned long __val) {
        _C_digitize (_RWSTD_STATIC_CAST (unsigned long, __val), false, "l");
    }

#ifdef _RWSTD_LONG_LONG

    void _C_digitize (unsigned _RWSTD_LONG_LONG, bool = false);

    void _C_digitize (_RWSTD_LONG_LONG __val) {
        _C_digitize (_RWSTD_STATIC_CAST (unsigned _RWSTD_LONG_LONG, __val), !0);
    }

    // convert to a binary string
    void _C_to_binary (unsigned _RWSTD_LONG_LONG);

#endif

    void _C_digitize (double) __softfp;

#ifndef _RWSTD_NO_LONG_DOUBLE
    void _C_digitize (long double) __softfp;
#endif

    // convert to a binary string
    void _C_to_binary (unsigned long);

    _STD::ios_base           &_C_io;      // associated stream
    _STD::ios_base::fmtflags  _C_flags;   // modified flags from _C_io

    int _C_precision, _C_width, _C_base, _C_iprecision, _C_num_groups;
    enum { _C_left, _C_internal, _C_right } _C_adjust;
    bool _C_fractional, _C_separable;
    char *_C_start, *_C_end, *_C_group;

    enum { _C_DEF_BUFSIZE = 65 };       // large enough for a 64 bit int
    char _C_buffer [_C_DEF_BUFSIZE ];   // default buffer
    char _C_groups[150];

    int _C_calc_groups (int, const char*);
};


inline
__rw_digit_writer_base::__rw_digit_writer_base (_STD::ios_base &__b)
    : _C_io(__b), _C_flags(__b.flags()),  _C_precision(int(__b.precision())),
      _C_width(int(__b.width())), _C_iprecision(0), _C_num_groups(0), 
      _C_fractional(false), _C_separable(true), _C_start(_C_buffer),
      _C_end(_C_buffer) {

    
    _C_base = __b._C_base ();

    switch (_C_flags & _STD::ios_base::adjustfield) {

    case _STD::ios_base::left:
        _C_adjust = _C_left;
        break;

    case _STD::ios_base::internal:
        _C_adjust = _C_internal;
        break;

    default:
        _C_adjust = _C_right;
        break;
    }
}


// ------------------------------------------------------------------
// Implementation class template -- __rw_digit_writer_base_1<_CharT>.
// ------------------------------------------------------------------

// Contains parts of __rw_digit_writer<_CharT, OutputWriter>
// that depend only on the _CharT parameter.

template <class _CharT>
class __rw_digit_writer_base_1
    : public __rw_digit_writer_base, 
      public __rw_digit_handler_base_1<_CharT>
{
public:
    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, _STD::allocator<_CharT> >
    string_type;

protected:
    // Constructor for general numerics, gets punct from loc's numpunct<_CharT>.
    __rw_digit_writer_base_1(ios_base &__b, const _STD::locale &__loc) 
        : __rw_digit_writer_base (__b), __rw_digit_handler_base_1<_CharT>(__loc)
        { }


    // Constructor for money, passed punct is really a moneypunct<_CharT, Intl>.
    __rw_digit_writer_base_1  (_STD::ios_base&, const __rw_punct_data<_CharT>&);
};

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_digit_writer_base_1<char>);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_digit_writer_base_1<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_T


// --------------------------------------------------------------------------
// Implementation class template -- __rw_digit_writer<_CharT, _OutputIter>.
// --------------------------------------------------------------------------

// Facets that do numeric output use temporary objects of this type (on the
// stack) to convert numeric quantities into sequences of digits and other
// punctuation.

template <class _CharT, class _OutputIter>
class __rw_digit_writer
    : public __rw_digit_writer_base_1<_CharT>
{
public:
    typedef _STD::char_traits<_CharT> traits_type;
    typedef _STD::basic_string<_CharT, traits_type, _STD::allocator<_CharT> >
    string_type;

  // Constructor for general numerics.
  __rw_digit_writer (_OutputIter& __it, _STD::ios_base& __flags)
      : __rw_digit_writer_base_1<_CharT>(__flags, __flags.getloc ()),
        _C_out (__it) { }

 protected:
    // Constructor for use by derived __rw_money_writer classes,
    // passed __rw_punct_data is really a moneypunct<_CharT, Intl> facet.
    __rw_digit_writer (_OutputIter &__it, _STD::ios_base &__flags, 
                       const __rw_punct_data<_CharT> &__mp)
        : __rw_digit_writer_base_1<_CharT>(__flags, __mp), _C_out (__it) 
        { }

 public:
  void _C_put_digits (_CharT);
  void _C_put_keyword (const string_type&, _CharT);

 protected:
  _OutputIter &_C_out;
};

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    __rw_digit_writer<char,
        _STD::ostreambuf_iterator<char, _STD::char_traits<char> > >
);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_3 (
    class _RWSTD_EXPORT
    __rw_digit_writer<wchar_t,
        _STD::ostreambuf_iterator<wchar_t, _STD::char_traits<wchar_t> > >
);

#endif   // _RWSTD_NO_WCHAR_T


_RWSTD_NAMESPACE_END   // __rw


#if _RWSTD_DEFINE_TEMPLATE (PUNCT)
#  include <rw/_punct.cc>
#endif


#endif   // _RWSTD_PUNCT_H_INCLUDED
