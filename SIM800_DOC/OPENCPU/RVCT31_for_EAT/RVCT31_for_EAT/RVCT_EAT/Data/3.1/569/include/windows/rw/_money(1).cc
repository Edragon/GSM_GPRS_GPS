/***************************************************************************
 *
 * _money.cc - Definitions for the Standard Library money facets
 *
 * $Id: _money.cc 91768 2005-08-10 18:58:48Z drodgman $
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

#include _RWSTD_CWCHAR

_RWSTD_NAMESPACE_BEGIN (__rw)

_USING (_RWSTD_C::size_t);

// ------------------------------------------------------
// Template __rw_moneypunct_impl_data<_CharT> member templates.
// ------------------------------------------------------

template <class _CharT>
__rw_moneypunct_impl_data<_CharT>::__rw_moneypunct_impl_data
    (const __rw_moneypunct_init_data<_CharT> *__mp)
{
  if (!__mp) {
    this->_C_dp = _CharT('.');
    this->_C_ts = _CharT(',');
    _C_fd       = 0;
  } else {
    this->_C_dp = __mp->_C_dp;
    this->_C_ts = __mp->_C_ts;
    this->_C_gr = __mp->_C_gr;
    _C_cs       = __mp->_C_cs;
    _C_ps       = __mp->_C_ps;
    _C_ns       = __mp->_C_ns;
    _C_fd       = __mp->_C_fd;
    _C_pf       = __mp->_C_pf;
    _C_nf       = __mp->_C_nf;

    if (__mp->_C_del)
        delete[] _RWSTD_CONST_CAST (char*,
                                    _RWSTD_REINTERPRET_CAST (const char*,
                                                             __mp));
  }
}


template <class _CharT>
const __rw_moneypunct_init_data<_CharT>*
__rw_fixup_moneypunct_init (const __rw_moneypunct_init_data<char> *__mp,
                            _CharT*)
{
    if (!__mp)
        return 0;

    typedef _STD::char_traits<char>                      _CharTraits;
    typedef _STD::char_traits<_CharT>                    _Traits;
    typedef _STD::ctype<_CharT>                          _Ctype;
    typedef _STD::codecvt<_CharT, char, _STD::mbstate_t> _Codecvt;
    typedef _STD::basic_string<_CharT, _Traits, _STD::allocator<_CharT> >
        _String;

    _STD::locale __loc = _STD::locale::classic ();

    const _Codecvt &__cvt = _USE_FACET (_Codecvt, __loc);

    _String __cs = __cvt.in (__mp->_C_cs);
    _String __ps = __cvt.in (__mp->_C_ps);
    _String __ns = __cvt.in (__mp->_C_ns);

    size_t __xtra_chars  = _CharTraits::length (__mp->_C_gr);
    size_t __xtra_charTs = __cs.length () +__ps.length () + __ns.length() + 3;

    __rw_moneypunct_init_data<_CharT> *__res;
   __res = _RWSTD_REINTERPRET_CAST (__rw_moneypunct_init_data<_CharT>*, 
                                 new char [  sizeof (*__res)
                                           + __xtra_chars
                                           + __xtra_charTs * sizeof(_CharT)]);

    const _Ctype &__ctp = _USE_FACET (_Ctype, __loc);

    __res->_C_del = true;
    __res->_C_dp  = __ctp.widen (__mp->_C_dp);
    __res->_C_ts  = __ctp.widen (__mp->_C_ts);
    __res->_C_fd  = __mp->_C_fd;
    __res->_C_pf  = __mp->_C_pf;
    __res->_C_nf  = __mp->_C_nf;

    size_t __n;
    _CharT *__p = (_CharT*)(__res + 1);
    __res->_C_cs = _Traits::copy (__p, __cs.c_str (), __n = __cs.length ());
    *(__p += __n)++ = 0;
    __res->_C_ps = _Traits::copy (__p, __ps.c_str (), __n = __ps.length ());
    *(__p += __n)++ = 0;
    __res->_C_ns = _Traits::copy (__p, __ns.c_str (), __n = __ns.length ());
    *(__p += __n)++ = 0;

    __res->_C_gr = _CharTraits::copy (_RWSTD_REINTERPRET_CAST (char*,
                                                               __p + __n), 
                                      __mp->_C_gr, 
                                      _CharTraits::length (__mp->_C_gr));

    if (__mp->_C_del)
        // redundant const_cast for compilers such as MSVC 6.0
        // that do not conform to 5.3.5, p2, Note 2 of the IS
        delete[] _RWSTD_CONST_CAST (char*, 
                                    _RWSTD_REINTERPRET_CAST (const char*, 
                                                             __mp));

    return __res;
}


template <class _CharT>
const __rw_moneypunct_init_data<_CharT>*
__rw_moneypunct_impl_data<_CharT>::
_C_get_init_by_name_ (const char *__name, bool __intl)
{
    return __rw_fixup_moneypunct_init (_C_get_named_init (__name, __intl),
                                       (_CharT*)0);
}


// -----------------------------------------------------------------
// Template class __rw_money_handler_base_1<_CharT> member templates.
// -----------------------------------------------------------------

template <class _CharT>
const __rw_moneypunct_impl_data<_CharT>&
__rw_money_handler_base_1<_CharT>::
_C_get_punct_data (const locale &__loc, bool __intl)
{
    if (__intl) {
        typedef _STD::moneypunct<_CharT, true> _MoneyPunct;
        return _USE_FACET (_MoneyPunct, __loc);
    }
    else {
        typedef _STD::moneypunct<_CharT, false> _MoneyPunct;
        return _USE_FACET (_MoneyPunct, __loc);
    }
}

// ----------------------------------------------------------------
// Template class __rw_money_reader_base_1<_CharT> member templates.
// ----------------------------------------------------------------

template <class _CharT>
void __rw_money_reader_base_1<_CharT>::_C_get_money_string
    (string_type &__res, const char *__end)
{
    _RWSTD_ASSERT (__end >= reader._C_digits);

    _RWSTD_C::size_t __len = __end - reader._C_digits;
    if (reader._C_state & reader._C_negative)
        __len++;

  __res.resize(0);
  __res.reserve(__len);

  if (reader._C_state & reader._C_negative)
    __res+=reader._C_dmap._C_punct_chars[__rw_digit_map_base::_C_minus];

  const char *__d=reader._C_digits;
  const char *__last=__end-1;
  while (__d<__last && *__d==0) __d++;
  for ( ; __d<=__last; __d++)
    __res+=reader._C_dmap._C_digit_chars[*__d];
}

// ---------------------------------------------------------------------------
// Template class __rw_money_reader_base_2<CharT, InputIter> member templates.
// ---------------------------------------------------------------------------

template <class _CharT, class _InputIter>
__rw_money_reader_base_2<_CharT, _InputIter>::
__rw_money_reader_base_2 (_InputIter &__begin, _InputIter &__end, 
                          _STD::ios_base &__flags,
                          const __rw_moneypunct_impl_data<_CharT> &__mp)
 :  __rw_digit_reader<_CharT, _InputIter>(__begin, __end, __flags, __mp), 
    __rw_money_reader_base_1<_CharT>(_C_this_as_digit_reader(), __mp)
{
    this->_C_base   = 10;
    this->_C_state |= this->_C_signed;
}


template <class _CharT, class _InputIter>
char* __rw_money_reader_base_2<_CharT, _InputIter>::
    _C_get_money_digits (void)
{
  _CharT __c;
  // Always parse input according to negative format.
  const _STD::money_base::pattern &patt = this->_C_get_neg_format();

  // If this ends up non-0, it points to trailing sign char(s) that are
  // required at the end of the pattern.
  const _CharT *__sign = 0;

  bool need_curr = 0 != (this->_C_io.flags() & _STD::ios_base::showbase);
  bool got_curr=false;
  int __frac=this->_C_get_frac_digits();

  char *__end = this->_C_digits;
  const char *__p=patt.field, *__pend=__p+sizeof patt.field-1;

  for ( ; !this->_C_error && __p<=__pend; __p++) {
    switch (*__p) {
     case _STD::money_base::space:
     case _STD::money_base::none:
      while (   !this->eof()
             && this->_C_ctyp.is (_STD::ctype_base::space, *this->_C_in))
        this->_C_in++;
      break;

     case _STD::money_base::symbol:
      if (!this->eof()) {
        const _CharT *curr=this->_C_get_curr_symbol().c_str();
        if ((__c=*this->_C_in)==*curr) {
          // We ate a character, so rest of symbol must be present.
          need_curr=true;
          do {
            this->_C_in++;
            if (*++curr==_CharT('\0')) {
              got_curr=true;
              break;
            }
          } while (!this->eof() && (__c=*this->_C_in)==*curr);
        }
      }
      if (need_curr && !got_curr)
        this->_C_error|=this->_C_bad_curr_symbol;
      break;

     case _STD::money_base::sign:
      if (!this->eof()) {
        __sign = this->_C_get_negative_sign().c_str();
        if ((__c = *this->_C_in) == *__sign) {
          this->_C_state |= this->_C_negative;
          __sign++;
          this->_C_in++;
        } else {
          __sign=this->_C_get_positive_sign().c_str();
          if (__c == *__sign) {
            __sign++;
            this->_C_in++;
          } else
            __sign = 0;
        }
      }
      break;

     case _STD::money_base::value:
      __end=this->_C_get_digits (int ());
      if (!this->_C_error && __frac && !this->eof() &&
          *this->_C_in==this->_C_get_decimal_point())
      {
        this->_C_in++;
        char *frac_start=__end;
        __end=this->_C_get_digit_string(__end);
        if (!this->_C_error) {
          __frac-=int(__end-frac_start);
          if (__frac<0) {
            // Too many fractional digits in input.  We truncate.  The
            // alternative would be to return an error.
            __end+=__frac;
            __frac=0;
          }
        }
      }
      if (__end == this->_C_digits)
        this->_C_error|=this->_C_no_digits;
      else if (!this->_C_error && __frac > 0) {
        // Add trailing zeros until the correct number of fractional digits
        // is reached.

        if (__end == this->_C_digits + this->_C_bufsize)
            __end = this->_C_realloc ();

        do
            *__end++ = 0;
        while (--__frac);
      }
      break;
    }
  }

  if (__sign && !this->_C_error)
    // We ate one _CharT of a sign earlier, rest of sign must be present at end.
    while (*__sign)
      if (!this->eof() && *this->_C_in == *__sign++)
        this->_C_in++;
      else {
        this->_C_error|=this->_C_bad_sign;
        break;
      }

  this->_C_frac_beg=this->_C_exp_beg=__end;
  return __end;
}

// ------------------------------------------------------------------
// Template __rw_money_reader<_CharT, _InputIter> member templates.
// ------------------------------------------------------------------

// ----------------------------------------------------------
// Template __rw_money_writer_base_1<_CharT> member templates.
// ----------------------------------------------------------


template <class _CharT>
void __rw_money_writer_base_1<_CharT>::_C_put_money (_CharT __fill)
{
    const char *__start = writer._C_start;

    bool __neg = '-' == *__start;   // negative?

    if ('-' == *__start || '+' == *__start || ' ' == *__start)
        ++__start;

    // small auto buffer for the average case
    _CharT __wide_buf [__rw_digit_writer_base_1<_CharT>::_C_DEF_BUFSIZE];

    // required buffer size
    const size_t __size = writer._C_end - __start;

    // allocate more space if necessary
    _CharT *__pbuf = __size > __rw_digit_writer_base_1<_CharT>::_C_DEF_BUFSIZE ?
        new _CharT [__size] : __wide_buf;

    // widen into temporary buffer
    writer._C_ctyp.widen (__start, writer._C_end, __pbuf);

    // reformat as required
    _C_put_money_sub (__pbuf, __pbuf + __size, __neg, __fill);

    // delete only if allocated
    if (__pbuf != __wide_buf)
        delete[] __pbuf;
}


template <class _CharT>
void __rw_money_writer_base_1<_CharT>::_C_put_money
    (const string_type& __digits, _CharT __fill)
{
  const _CharT *punct =
      __rw_digit_map<_CharT>::_C_get_digit_map(writer._C_ctyp)
      ._C_get_punct();

  const _CharT *__start = __digits.c_str();
  bool __neg;
  if (*__start==punct[__rw_digit_map_base::_C_minus]) {
    __neg=true;
    __start++;
  } else
    __neg=false;

  const _CharT *end=writer._C_ctyp.scan_not(_STD::ctype_base::digit,
                                            __start, 
                                            __digits.c_str()+__digits.length());

  _C_put_money_sub(__start, end, __neg, __fill);
}

// -------------------------------------------------------------------------
// Template __rw_money_writer_base_2<_CharT, _OutputIter> member templates.
// -------------------------------------------------------------------------

template <class _CharT, class _OutputIter>
void __rw_money_writer_base_2<_CharT, _OutputIter>::_C_put_money_sub
    (const _CharT *__begin, const _CharT *__end, bool __neg, _CharT __fill)
{
  const _STD::money_base::pattern *__pat;
  const string_type               *__sign;

  if (__neg) {
      __pat  = &this->_C_get_neg_format();
      __sign = &this->_C_get_negative_sign();
  } else {
      __pat  = &this->_C_get_pos_format();
      __sign = &this->_C_get_positive_sign();
  }

  int __frac_dig = this->_C_get_frac_digits();
  int __int_dig  = int (__end - __begin) - __frac_dig;

  int __ungrp;
  int __pad;

  if (__int_dig<0) {
    __pad = -__int_dig;
    __int_dig = 0;
  } else
    __pad = 0;

  _CharT __sep = _CharT ();

  if (__int_dig > 0) {
    __ungrp = this->_C_calc_groups (__int_dig, this->_C_get_grouping ());
    if (this->_C_num_groups)
      __sep = this->_C_get_thousands_sep();
  } else
    __ungrp = 0;

  const char *__p;
  const char *__pend = __pat->field + sizeof __pat->field;

  int __l_fill = 0;   // left fill
  int __i_fill = 0;   // internal fill
  int __r_fill = 0;   // right fill

  if (this->_C_width) {
    int __n=0;
    for (__p=__pat->field; __p<__pend; __p++) {
      switch (*__p) {

       case _STD::money_base::space:
        __n++;
        break;

       case _STD::money_base::symbol:// save pptr pos
        if (this->_C_flags& ios_base::showbase)
          __n+=int(this->_C_get_curr_symbol().length());
        break;

       case _STD::money_base::sign:
        __n += int (__sign->length());
        break;

       case _STD::money_base::value:
        __n+=__int_dig+this->_C_num_groups;
        if (__frac_dig)
          __n+=__frac_dig+1;
        break;
      }
    }

    if ((__n-=this->_C_width)>0) {
      switch (this->_C_adjust) {
       case __rw_digit_writer_base::_C_left:
        __r_fill=__n;
        break;
       case __rw_digit_writer_base::_C_internal:
        __i_fill=__n;
        break;
       default:
        __l_fill=__n;
      }
    }

    this->_C_width=0;
  }

  if (__l_fill)
    do *this->_C_out++=__fill;
    while (--__l_fill);

  const _CharT *__schar = __sign->c_str();
  for (__p=__pat->field; __p<__pend; __p++)
    switch (*__p) {

     case _STD::money_base::symbol:
      if (this->_C_flags& ios_base::showbase)
        this->_C_put_keyword (this->_C_get_curr_symbol(), __fill);
      break;

     case _STD::money_base::sign:
      if (*__schar)
        *this->_C_out++=*__schar++;
      break;

     case _STD::money_base::value:
      while (__ungrp--)
        *this->_C_out++=*__begin++;
      while (this->_C_num_groups--) {
        *this->_C_out++=__sep;
        while ((*this->_C_group)--)
          *this->_C_out++=*__begin++;
        this->_C_group++;
      }
      if (__frac_dig) {
        *this->_C_out++=this->_C_get_decimal_point();
        while (__pad--) {
          __frac_dig--;
          *this->_C_out++=this->_C_ctyp.widen('0');
        }
        while (__frac_dig-->0)
          *this->_C_out++=*__begin++;
      }
      break;

     case _STD::money_base::space:
      if (!__i_fill) {
        *this->_C_out++=this->_C_ctyp.widen(' ');
        break;
      }

      // Fall through ...
     case _STD::money_base::none:
      if (__i_fill)
        do *this->_C_out++=__fill;
        while (--__i_fill);
      break;
    }

  while (*__schar)
    *this->_C_out++=*__schar++;

  if (__r_fill+=__i_fill) {
    do *this->_C_out++=__fill;
    while (--__r_fill);
  }
}


_RWSTD_NAMESPACE_END   // __rw


_RWSTD_NAMESPACE_BEGIN (std)


// ------------------------------------------------------
// Facet money_get<_CharT, _InputIter> member templates.
// ------------------------------------------------------

template <class _CharT, class _InputIter>
locale::id money_get<_CharT, _InputIter>::id;

// Warning -- these functions do not input actual monetary value; they just
// input numbers that represent monetary value.

template <class _CharT, class _InputIter>
_InputIter money_get<_CharT, _InputIter>::
do_get (_InputIter __begin, _InputIter __end, bool __intl, ios_base& __flags, 
     ios_base::iostate& __err, long double& __units) const {

    _RW::__rw_money_reader<_CharT, _InputIter> __reader (__begin, __end,
                                                         __flags,__intl);

    long double __val = __reader._C_parse (__reader._C_get_money_digits(),
                                           0.0L);
    
    __err = ios_base::goodbit;
    
    if (__reader._C_error)
        __err = ios_base::failbit;
    else
        __units = __val;
    
    if (__reader.eof ())
        __err |= ios_base::eofbit;
    
    return __begin;
}

template <class _CharT, class _InputIter>
_InputIter money_get<_CharT, _InputIter>::
do_get (_InputIter __begin, _InputIter __end, bool __intl, ios_base &__flags,
        ios_base::iostate &__err, string_type &__digits) const
{
    _RW::__rw_money_reader<_CharT, _InputIter> reader(__begin, __end,
                                                      __flags, __intl);
    __err = ios_base::goodbit;
    
    if (reader._C_error)
        __err = ios_base::failbit;
    else
        reader._C_get_money_string (__digits, reader._C_get_money_digits());
    
    if (reader.eof ())
        __err |= ios_base::eofbit;
    
    return __begin;
}

// -------------------------------------------------------
// Facet money_put<_CharT, _OutputIter> member templates.
// -------------------------------------------------------

template <class _CharT, class _OutputIter>
locale::id money_put<_CharT, _OutputIter>::id;


template <class _CharT, class _OutputIter>
_OutputIter money_put<_CharT, _OutputIter>::
do_put (_OutputIter __it, bool __intl, ios_base& __flags, _CharT __fill, 
#ifndef _RWSTD_NO_LONG_DOUBLE
        long double __quant) const __softfp
#else
        double __quant) const __softfp
#endif
{
    _RW::__rw_money_writer<_CharT, _OutputIter> __writer(__it, __flags, __intl);
    __writer._C_digitize(__quant);
    __writer._C_put_money(__fill);
    return __it;
}

template <class _CharT, class _OutputIter>
_OutputIter money_put<_CharT, _OutputIter>::do_put
    (_OutputIter __it, bool __intl, ios_base& __flags, _CharT __fill, 
     const string_type& __digits) const {

    _RW::__rw_money_writer<_CharT, _OutputIter> __writer(__it, __flags, __intl);
    __writer._C_put_money (__digits, __fill);
    return __it;
}

// ----------------------------------------------
// Facet moneypunct<_CharT, Intl> member templates.
// ----------------------------------------------

template <class _CharT, bool _Intl>
locale::id moneypunct<_CharT, _Intl>::id;

#ifndef _RWSTD_NO_STATIC_CONST_MEMBER_INIT
template <class _CharT, bool _Intl>
const bool moneypunct<_CharT, _Intl>::intl;
#endif

template <class _CharT, bool _Intl>
_CharT moneypunct<_CharT, _Intl>::do_decimal_point () const {
    return this->_C_dp;
}

template <class _CharT, bool _Intl>
_CharT moneypunct<_CharT, _Intl>::do_thousands_sep () const {
    return this->_C_ts;
}

template <class _CharT, bool _Intl>
string moneypunct<_CharT, _Intl>::do_grouping () const {
    return this->_C_gr;
}

template <class _CharT, bool _Intl>
_TYPENAME moneypunct<_CharT, _Intl>::string_type
moneypunct<_CharT, _Intl>::do_curr_symbol () const {
    return this->_C_cs;
}

template <class _CharT, bool _Intl>
_TYPENAME moneypunct<_CharT, _Intl>::string_type
moneypunct<_CharT, _Intl>::do_positive_sign () const {
    return this->_C_ps;
}

template <class _CharT, bool _Intl>
_TYPENAME moneypunct<_CharT, _Intl>::string_type
moneypunct<_CharT, _Intl>::do_negative_sign () const {
    return this->_C_ns;
}

template <class _CharT, bool _Intl>
int moneypunct<_CharT, _Intl>::do_frac_digits () const {
    return this->_C_fd;
}

template <class _CharT, bool _Intl>
money_base::pattern
moneypunct<_CharT, _Intl>::do_pos_format () const {
    return this->_C_pf;
}

template <class _CharT, bool _Intl>
money_base::pattern
moneypunct<_CharT, _Intl>::do_neg_format () const {
    return this->_C_nf;
}

template <class _CharT, bool _Intl>
void moneypunct<_CharT, _Intl>::_C_initfacet (const locale&)
{
    this->_C_dp = do_decimal_point();
    this->_C_ts = do_thousands_sep();
    this->_C_gr = do_grouping();
    this->_C_cs = do_curr_symbol();
    this->_C_ps = do_positive_sign();
    this->_C_ns = do_negative_sign();
    this->_C_fd = do_frac_digits();
    this->_C_pf = do_pos_format();
    this->_C_nf = do_neg_format();
}

_RWSTD_NAMESPACE_END   // std
