/***************************************************************************
 *
 * _numeric.cc - Definitions for the Standard Library numeric facets
 *
 * $Id: _numeric.cc 91768 2005-08-10 18:58:48Z drodgman $
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

#include <rw/_punct.h>


_RWSTD_NAMESPACE_BEGIN (__rw)

template <class _CharT>
__rw_numpunct_init_data<_CharT>*
__rw_fixup_numpunct_init (__rw_numpunct_init_data<char> *__np, _CharT*)
{
    if (__np->_C_del)
        delete[] (char*)__np;
    return 0;
}


// -----------------------------------------------
// Template __rw_numpunct_impl_data<_CharT> member functions.
// -----------------------------------------------

template <class _CharT>
void __rw_numpunct_impl_data<_CharT>::_C_initfacetbase (const locale&) {
  tf_defs_[0]._C_name=_C_tn.c_str();
  tf_defs_[0]._C_indx=1;
  tf_defs_[1]._C_name=_C_fn.c_str();
  tf_defs_[1]._C_indx=0;

  tf_map_._C_numdefs=2 ;
  tf_map_._C_defs=tf_defs_;
}

template <class _CharT>
__rw_numpunct_init_data<_CharT>*
__rw_numpunct_impl_data<_CharT>::_C_get_init_by_name_ (const char *__name)
{
    return __rw_fixup_numpunct_init (_C_get_named_init_(__name), (_CharT*)0);
}

_RWSTD_NAMESPACE_END   // __rw


_RWSTD_NAMESPACE_BEGIN (std)

// ----------------------------------------------------
// Facet num_get<_CharT, _InputIter> member templates.
// ----------------------------------------------------


template <class _CharT, class _InputIter>
locale::id num_get<_CharT, _InputIter>::id;


#ifndef _RWSTD_NO_BOOL

template <class _CharT, class __InputIter>
__InputIter num_get<_CharT, __InputIter>::
do_get (__InputIter __first, __InputIter __last,
        ios_base& __flags, ios_base::iostate& __err, bool& __val) const
{
    _RW::__rw_digit_reader<_CharT, __InputIter>
        __reader(__first, __last, __flags);

    long __tmp = __reader._C_parse (__reader._C_get_digits(long()), long());

    __err = ios_base::failbit;

    if (__flags.flags () & ios_base::boolalpha) {
        // 22.2.2.1.2, p 15 - any numeric input rejected
        if (__reader._C_error) {
            const numpunct<_CharT>& __punct =
                _RWSTD_STATIC_CAST(const numpunct<_CharT>&, __reader._C_punct);
            __tmp = __reader._C_get_keyword (__reader._C_get_tf_map (__punct));
            if (__tmp >= 0) {
                __val = 0 != __tmp;
                __err = ios_base::goodbit;
            }
        }
    }
    else if (!__reader._C_error) {
        // 22.2.2.1.2, p 14 - any non-numeric input rejected
        if (0 == __tmp) {
            __val = false;
            __err = ios_base::goodbit;
        }
        else if (1 == __tmp && !(__reader._C_state & __reader._C_negative)) {
            __val = true;
            __err = ios_base::goodbit;
        }
    }

    // 22.2.2.1.2, p 16
    if (__reader.eof ())
        __err |= ios_base::eofbit;

    return __first;
}

#endif // _RWSTD_NO_BOOL


// -----------------------------------------------------
// Facet num_put<_CharT, OutputIterator> member templates.
// -----------------------------------------------------

template <class _CharT, class _OutputIter>
locale::id num_put<_CharT, _OutputIter>::id;

#ifndef _RWSTD_NO_BOOL

template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::do_put
    (_OutputIter __it, ios_base& __flags, _CharT __fill, bool __val) const
{
  _RW::__rw_digit_writer<_CharT, _OutputIter> __writer(__it, __flags);
  if (__flags.flags() & ios_base::boolalpha) {
    const numpunct<_CharT>& __punct =
        _RWSTD_STATIC_CAST(const numpunct<_CharT>&, __writer._C_punct);
    __writer._C_put_keyword(__writer._C_get_tf_string(__punct, __val), __fill);
  } else {
    __writer._C_digitize((unsigned long) (__val? 1 : 0));
    __writer._C_put_digits(__fill);
  }
  return __it;
}

#endif // _RWSTD_NO_BOOL

template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::
do_put (_OutputIter __it, ios_base& __flags, _CharT __fill,
        const void* __val) const
{
  _RW::__rw_digit_writer<_CharT, _OutputIter> __writer(__it, __flags);
  __writer._C_digitize(__val);
  __writer._C_put_digits(__fill);
  return __it;
}

template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::do_put
    (_OutputIter __it, ios_base& __flags, _CharT __fill, long __val) const
{
  _RW::__rw_digit_writer<_CharT, _OutputIter> __writer(__it, __flags);
  __writer._C_digitize(__val);
  __writer._C_put_digits(__fill);
  return __it;
}

template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::
do_put (_OutputIter __it, ios_base& __flags, _CharT __fill,
        unsigned long __val) const
{
  _RW::__rw_digit_writer<_CharT, _OutputIter> __writer(__it, __flags);
  __writer._C_digitize(__val);
  __writer._C_put_digits(__fill);
  return __it;
}

#ifdef _RWSTD_LONG_LONG

template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::
do_put (_OutputIter __it, ios_base& __flags, _CharT __fill, 
        _RWSTD_LONG_LONG __val) const
{
    _RW::__rw_digit_writer<_CharT, _OutputIter> __writer (__it, __flags);
    __writer._C_digitize (__val);
    __writer._C_put_digits (__fill);
    return __it;
}


template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::
do_put (_OutputIter __it, ios_base& __flags, _CharT __fill, 
        unsigned _RWSTD_LONG_LONG __val) const
{
    _RW::__rw_digit_writer<_CharT, _OutputIter> __writer (__it, __flags);
    __writer._C_digitize (__val);
    __writer._C_put_digits (__fill);
    return __it;
}

#endif // _RWSTD_LONG_LONG

template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::do_put
    (_OutputIter __it, ios_base& __flags, _CharT __fill, 
     double __val) const __softfp
{
  _RW::__rw_digit_writer<_CharT, _OutputIter> __writer(__it, __flags);
  __writer._C_digitize(__val);
  __writer._C_put_digits(__fill);
  return __it;
}

#ifndef _RWSTD_NO_LONG_DOUBLE
template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::
do_put (_OutputIter __it, ios_base& __flags, _CharT __fill,
        long double __val) const __softfp
{
  _RW::__rw_digit_writer<_CharT, _OutputIter> __writer(__it, __flags);
  __writer._C_digitize(__val);
  __writer._C_put_digits(__fill);
  return __it;
}
#endif // _RWSTD_NO_LONG_DOUBLE

// Rogue wave extensions
template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::do_put
    (_OutputIter __it, ios_base& __flags, _CharT __fill, short __val) const
{
  _RW::__rw_digit_writer<_CharT, _OutputIter> __writer(__it, __flags);
  __writer._C_digitize(__val);
  __writer._C_put_digits(__fill);
  return __it;
}

template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::
do_put (_OutputIter __it, ios_base& __flags, _CharT __fill,
        unsigned short __val) const
{
  _RW::__rw_digit_writer<_CharT, _OutputIter> __writer(__it, __flags);
  __writer._C_digitize(__val);
  __writer._C_put_digits(__fill);
  return __it;
}

template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::do_put
    (_OutputIter __it, ios_base& __flags, _CharT __fill, int __val) const
{
  _RW::__rw_digit_writer<_CharT, _OutputIter> __writer(__it, __flags);
  __writer._C_digitize(__val);
  __writer._C_put_digits(__fill);
  return __it;
}

template <class _CharT, class _OutputIter>
_OutputIter num_put<_CharT, _OutputIter>::
do_put (_OutputIter __it, ios_base& __flags, _CharT __fill,
        unsigned int __val) const
{
  _RW::__rw_digit_writer<_CharT, _OutputIter> __writer(__it, __flags);
  __writer._C_digitize(__val);
  __writer._C_put_digits(__fill);
  return __it;
}

// ---------------------------------------
// Facet numpunct<_CharT> member templates.
// ---------------------------------------

template <class _CharT>
locale::id numpunct<_CharT>::id;

template <class _CharT>
void numpunct<_CharT>::_C_initfacet (const locale &__loc)
{
  this->_C_dp = do_decimal_point();
  this->_C_ts = do_thousands_sep();
  this->_C_gr = do_grouping();
  this->_C_fn = do_falsename();
  this->_C_tn = do_truename();
  this->_C_initfacetbase(__loc);
}


_RWSTD_NAMESPACE_END   // std
