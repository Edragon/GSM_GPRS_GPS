/****************************************************************************
 *
 * _punct.cc - Internal classes for numeric formatting and parsing.
 *
 * $Id: _punct.cc 91768 2005-08-10 18:58:48Z drodgman $
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

#include <rw/_locimp.h>
#include <rw/_ctype.h>
#include <rw/_defs.h>

#include _RWSTD_CLOCALE


_RWSTD_NAMESPACE_BEGIN (__rw)

_USING (namespace std);


// -----------------------------------------------------
// Template __rw_timepunct_impl_data<_CharT> member templates.
// -----------------------------------------------------

template <class _CharT>
__rw_timepunct_impl_data<_CharT>::__rw_timepunct_impl_data
    (const __rw_timepunct_init<_CharT> &__p)
{
  int __i;
  for (__i=0; __i<7; __i++) {
      _C_day_nms[__i][0]=__p._C_get_day (__i, false);
      _C_day_nms[__i][1]=__p._C_get_day (__i, true);
  }
  for (__i=0; __i<12; __i++) {
      _C_month_nms[__i][0]=__p._C_get_month (__i, false);
      _C_month_nms[__i][1]=__p._C_get_month (__i, true);
  }
  _C_ampm_nms[0]=__p._C_get_ampm (false);
  _C_ampm_nms[1]=__p._C_get_ampm (true);
  _C_bad_nm=__p._C_get_bad ();
  for (__i=0; __i<100; __i++)
      _C_ord_nms[__i]=__p._C_get_ord (__i);
  for (size_t __j=0; __j<sizeof _C_dt_tm_pats/sizeof _C_dt_tm_pats[0]; __j++)
      _C_dt_tm_pats[__j]=__p._C_get_pattern (int(__j));
}

// struct for storing formatting information in map
struct __rwstd_fmt {
    __rwstd_fmt () {}
    __rwstd_fmt (size_t __pos, size_t __len, char __fmtc)
        : _C_pos(__pos), _C_len(__len), _C_fmt_char(__fmtc) {}
    size_t _C_pos;
    size_t _C_len;
    char _C_fmt_char;
    bool operator < (const __rwstd_fmt& __fmt) {
        return _C_pos < __fmt._C_pos;
    }
};

// initialize data member pat_ which holds formatting strings
template <class _CharT>
void __rw_timepunct_impl_data<_CharT>::_C_initpat (const locale &__loc)
{
  static const char __tm_single_data_patts[] = "YyjSMIHmBbAapdZUWw";

  tm __tmb;
  char __buf[256];
  __rwstd_fmt __fmtstruct_array[20];

  // set up tmp
  __tmb.tm_sec = 56;
  __tmb.tm_min = 54;
  __tmb.tm_hour = 22;
  __tmb.tm_mday = 30;
  __tmb.tm_mon = 10;
  __tmb.tm_year = 90;
  __tmb.tm_wday = 5;
  __tmb.tm_yday = -1;
  __tmb.tm_isdst = -1;
  (void) mktime (&__tmb);

  typedef ctype<_CharT> _Ctype;
  typedef __rw_facet_maker<_Ctype>   _FacetMaker;

  // bypass the mutex in std::use_facet<>() by directly calling
  // the implementation locale member function to prevent a deadlock

  const _Ctype &__ctp =
      _RWSTD_STATIC_CAST (const _Ctype&,
                          __loc._C_unsafe_use_facet (_Ctype::id,
                                                   _Ctype::_C_ok_implicit,
                                                   _Ctype::_C_facet_cat, 
                                                   _FacetMaker::_C_maker_func));

  // Set up passed in locale
  string __oldloc = setlocale (LC_ALL, 0);
#ifdef __BORLANDC__
  string::size_type __posd = __loc->name ().find ('=')+1;
  setlocale (LC_ALL, string (__loc->name (), __posd, __loc->name ().find ('\n')-__posd).c_str ());
#else
    setlocale (LC_ALL, __loc.name ().c_str ());
#endif // __BORLANDC__

  // For each composite format character create format string
  static const char __tm_mult_data_patts[] = "xXcDrT";

  for (unsigned int __j = 0; __j != sizeof __tm_mult_data_patts - 1; ++__j) {
    // create format string to parse
      char __tmfmt_pat[] = "% ";
      __tmfmt_pat[1] = __tm_mult_data_patts[__j];
      strftime (__buf, sizeof (__buf), __tmfmt_pat, &__tmb);
      string __tmfmt_result (__buf);

    // starting at the beginning of the formatted string find each component
    // and save in an ordered structure
    size_t __pos;
    int __fmtstruct_index = 0;
    for (unsigned int __i=0; __i < sizeof (__tm_single_data_patts) - 1; ++__i) {
        char __tmp_fmt_arg[] = "% ";
      __tmp_fmt_arg[1] = __tm_single_data_patts[__i];
      strftime (__buf, sizeof (__buf), __tmp_fmt_arg, &__tmb);
      // Some locales don't have am/pm, in which case strftime (...%p...)
      // returns a space. Exclude this case to ensure valid substring searches.
      if (!(__tm_single_data_patts[__i]=='p' && strcmp (__buf, " ")==0)) {
        if ((__pos = __tmfmt_result.find (__buf)) != size_t (-1)) {
          if (strlen (__buf) > 0) {
            __fmtstruct_array[__fmtstruct_index++] =
                __rwstd_fmt (__pos, strlen (__buf),
                             __tm_single_data_patts[__i]);
            __tmfmt_result.replace (__pos, strlen (__buf), strlen (__buf), ' ');
          }
        }
      }
    }

    // Sort Format Items
    for (int __top = __fmtstruct_index; __top > 0; --__top) {
      int __large = 0;
      for (int __k = 0; __k < __top; ++__k) {
        if (__fmtstruct_array[__k] < __fmtstruct_array[__large])
          __large = __k;
      }
      __rwstd_fmt __temp = __fmtstruct_array[__large];
      __fmtstruct_array[__large] = __fmtstruct_array[__top-1];
      __fmtstruct_array[__top-1] = __temp;
    }

    // Attempt to deal with implementations of strftime that use '%e'
    {
      string __alt_tmfmt (__tmfmt_result); // Alternate timepunct format string
      char __compare_buf[256];                // Buffer to compare with strftime

      // put format characters into formatting string
      for (__rwstd_fmt *__tmpfmt = __fmtstruct_array;
           __tmpfmt < (__fmtstruct_array + __fmtstruct_index);
           ++__tmpfmt) {
        __tmfmt_result.replace ((*__tmpfmt)._C_pos, (*__tmpfmt)._C_len, 
                                 (string ("%").append (1, (*__tmpfmt)._C_fmt_char)));
        // create an alternate formatting string that uses "%e" instead of "%d"
        if ((*__tmpfmt)._C_fmt_char != 'd') {
          __alt_tmfmt.replace ((*__tmpfmt)._C_pos, (*__tmpfmt)._C_len, 
                        (string ("%").append (1, (*__tmpfmt)._C_fmt_char)));
        } else {
          __alt_tmfmt.replace ((*__tmpfmt)._C_pos, (*__tmpfmt)._C_len, 
                        (string ("%e")));
        }                                                              
      }

      // We need a single-digit day to see if spaces or zeroes are used to pad
      __tmb.tm_mday=3;

      // See if our usual format string is correct
      strftime (__buf, sizeof (__buf), __tmfmt_result.c_str (), &__tmb);
      strftime (__compare_buf, sizeof (__compare_buf), __tmfmt_pat, &__tmb);

      // Make a best-effort guess ... we can't verify that a "%e" format
      // string will do the trick, since the 'e' modifier is not specified by
      // the C standard, and hence the result of strftime would be undefined.

      if (strcmp (__buf, __compare_buf)) {
        __tmfmt_result = __alt_tmfmt; // use the alternate format string
      }

      // And restore the "safe" day-of-month for next time.
      __tmb.tm_mday=30;
    }

    // convert to string_type and insert in array _C_dt_tm_pats
    string_type __str;
    for (_TYPENAME string_type::size_type __k = 0;
         __k < __tmfmt_result.length (); ++__k)
        __str += __ctp.widen (__tmfmt_result[__k]);
    _C_dt_tm_pats[__j] = __str;

  }

  setlocale (LC_ALL, __oldloc.c_str ());
}

template <class _CharT>
void __rw_timepunct_impl_data<_CharT>::_C_initfacet (const locale &__loc) {

    int __i;
    for (__i=0; __i<7; ++__i) {
        _C_dn_defs[__i][0]._C_name=_C_day_nms[__i][0].c_str ();
        _C_dn_defs[__i][0]._C_indx=__i;
        _C_dn_defs[__i][1]._C_name=_C_day_nms[__i][1].c_str ();
        _C_dn_defs[__i][1]._C_indx=__i;
    }
    
    for (__i=0; __i<12; ++__i) {
        _C_mn_defs[__i][0]._C_name=_C_month_nms[__i][0].c_str ();
        _C_mn_defs[__i][0]._C_indx=__i;
        _C_mn_defs[__i][1]._C_name=_C_month_nms[__i][1].c_str ();
        _C_mn_defs[__i][1]._C_indx=__i;
    }
    
    _C_ampm_defs[0]._C_name=_C_ampm_nms[0].c_str ();
    _C_ampm_defs[0]._C_indx= 0;
    _C_ampm_defs[1]._C_name=_C_ampm_nms[1].c_str ();
    _C_ampm_defs[1]._C_indx= 1;
    
    _C_dn_defsmap._C_numdefs=7*2;
    _C_dn_defsmap._C_defs=&(_C_dn_defs[0][0]);
    _C_mn_defsmap._C_numdefs=12*2;
    _C_mn_defsmap._C_defs=&(_C_mn_defs[0][0]);
    _C_ampm_defsmap._C_numdefs=2;
    _C_ampm_defsmap._C_defs=&(_C_ampm_defs[0]);
    _C_initpat (__loc);
}


template <class _CharT>
const _CharT __rw_bool_name<_CharT>::_C_name[][6] = {
    { 'f', 'a', 'l', 's', 'e', '\0' },
    { 't', 'r', 'u', 'e', '\0' }
};


template <class _CharT>
const __rw_numpunct_init_data<_CharT>
__rw_numpunct_impl<_CharT>::_C_ivals = {
    false,        // don't delete
    '.',          // decimal point
#if !defined (_WIN32) && !defined (_WIN64)
    '\0',         // standard "C" locale thousands separator
#else
    ',',          // Windows "C" locale thousands separator
#endif
    "",           // Digit grouping rule
    __rw_bool_name<_CharT>::_C_name [true],  // Boolean true value as string
    __rw_bool_name<_CharT>::_C_name [false]  // Boolean false value as string
};


// ----------------------------------------------------
// Facet rwstd::__rw_timepunct<_CharT> member templates.
// ----------------------------------------------------

template <class _CharT>
locale::id __rw_timepunct<_CharT>::id;

template <class _CharT>
void __rw_timepunct<_CharT>::_C_initfacet (const locale &__loc) {
  __rw_timepunct_impl<_CharT>::_C_initfacet (__loc);
}

template <class _CharT>
_TYPENAME __rw_timepunct<_CharT>::string_type
__rw_timepunct<_CharT>::do_dayname (int __day, bool __abbr) const {
  return this->_C_day_nms[__day][__abbr];
}

template <class _CharT>
_TYPENAME __rw_timepunct<_CharT>::string_type
__rw_timepunct<_CharT>::do_monthname (int __mon, bool __abbr) const {
  return this->_C_month_nms[__mon][__abbr];
}

template <class _CharT>
_TYPENAME __rw_timepunct<_CharT>::string_type
__rw_timepunct<_CharT>::do_ordinal (int __num) const {
  return (__num<0 || __num>99) ? this->_C_bad_nm : this->_C_ord_nms[__num];
}


// ---------------------------------------
// Template __rw_digit_reader members
// ---------------------------------------

template <class _CharT, class _InputIter>
char* __rw_digit_reader<_CharT, _InputIter>::
_C_get_digit_string (char *__first, bool __discard /* = false */) {

    char *__last = __first;

    // get a set of punctuator characters
    const _CharT *__punct = this->_C_dmap._C_get_punct ();
    
    // convert a sequence starting at iterator 'in' into
    // a binary (not character) string of digits
    while (!eof ()) {

        _CharT __c  = *_C_in;                        // extracted character
        int __digit = this->_C_dmap._C_eval (__c);   // binary equivalent

        // validate and break out on unknown char
        if (   __digit >= this->_C_base || __digit < 0
            && (   16 != this->_C_base
                || __c != __punct [this->_C_x] && __c != __punct [this->_C_X]))
            break;

        // buffer full, reallocate
        if (__last == this->_C_digits + this->_C_bufsize)
            __last = this->_C_realloc ();

        // append digit to the end of buffer
        // leading zeros may be optionally discarded
        if (   0 < __digit
            || 0 == __digit && (!__discard || __first != __last))
            *__last++ = char (__digit);

        ++_C_in;
    }

    return __last;
}


template <class _CharT, class _InputIter>
char* __rw_digit_reader<_CharT, _InputIter>::_C_get_digits (int)
{
    char *__last          = this->_C_digits;
    const _CharT* __punct = this->_C_dmap._C_get_punct ();

    if (!eof () && (this->_C_state & this->_C_signed)) {
        
        _CharT __ch = *_C_in;

        // If a sign is present, set negative if it's a minus sign.
        if (   __ch == __punct [this->_C_plus]
            || __ch == __punct [this->_C_minus]) {

            if (__ch == __punct [this->_C_minus])
                this->_C_state |= this->_C_negative;

            do
                ++_C_in;
            while (!eof () && this->_C_ctyp.is (ctype_base::space, *_C_in));

            this->_C_state |= this->_C_advanced;
        }
    }

    if (!this->_C_base) {

        this->_C_base = _C_io._C_base ();

        if (0 == this->_C_base || (_C_io.flags () & ios_base::showbase)) {
            // No radix was forced by the ios_base flags, so poke around
            // for a radix specifier on the front of the input value.

            if (0 == this->_C_base)
                this->_C_base = 10;

            if (!eof () && *_C_in == __punct[this->_C_zero]) {
                this->_C_base = 8;
                ++_C_in;
                *__last++ = 0;
                this->_C_state |= this->_C_advanced;

                _CharT __ch;

                if (  !eof ()
                    && (   (__ch = *_C_in) == __punct[this->_C_x]
                        || __ch == __punct[this->_C_X])) {
                    this->_C_base = 16;
                    ++_C_in;
                    __last--; // Leading 0 was not a digit after all.
                }
            }
        }
    }

    if (!*(_C_grouping = this->_C_get_grouping ()))
        __last = _C_get_digit_string (__last);
    else {
        _C_gpos = _C_groups;

        thousands_sep = this->_C_get_thousands_sep ();
        __last        = _C_get_digit_groups (__last);

        if (!this->_C_error && --_C_gpos > _C_groups) {
            // Digit grouping is optional, but if present it must be right.
            const char *__gdef = _C_grouping;
            do {
                if (*__gdef != *_C_gpos && *__gdef != CHAR_MAX)
                    break;

                if (__gdef[1])
                    ++__gdef;
            } while (--_C_gpos > _C_groups);

            if (_C_gpos > _C_groups || *_C_gpos > *__gdef)
                this->_C_error |= this->_C_bad_grouping;
        }
    }

    if (__last > this->_C_digits)
        this->_C_state |= this->_C_advanced;

    return __last;
}


template <class _CharT, class _InputIter>
char* __rw_digit_reader<_CharT, _InputIter>::_C_get_digits (double) __softfp
{
    // get leading digits if any, discaring leading zeros
    this->_C_base = 10;
    char *__first = _C_get_digits (0);

    // get a set of valid punctuators
    const _CharT *__punct = this->_C_dmap._C_get_punct ();

    this->_C_frac_beg = __first;

    if (!this->_C_error && !eof ()) {
        if (*_C_in == this->_C_get_decimal_point ()) {
            // advance past decimal point and append fractional part
            // to the end of internal buffer
            ++_C_in;
            __first = _C_get_digit_string (__first);
        }
    }

    // 7.19.6.1 of C99: determine whether we're parsing one of:
    // "inf", "infinity", "nan", "nan(n-char-sequence)"

    if (__first == this->_C_digits && !eof ()) {

        const ctype<char> &__ctp = _USE_FACET (ctype<char>, _C_io.getloc ());

        char __c = (__ctp.tolower)(char (*_C_in));

        if ('i' == __c) {
            static const char __inf[] = "infinity";

            for (const char *__p = __inf; ; ++__p, ++_C_in) {
                // both "inf" and "infinity" are recognized
                if (   !*__p || 3 == __p - __inf
                    && *__p != (__ctp.tolower)(char(*_C_in))) {
                    this->_C_state |= this->_C_infinity;
                    return __first;
                }
                if (eof () || *__p != (__ctp.tolower)(char(*_C_in)))
                    break;   // error
            }
        }
        else if ('n' == __c) {
            for (const char *__p = "nan"; ; ++__p, ++_C_in) {
                // both "nan" and "nan(n-char-sequence)" are recognized
                if (!*__p) {
                    if (eof ()) {
                        this->_C_state |= this->_C_nan;
                        return __first;
                    }
                    if ('(' == *_C_in) {
                        // FIXME: "(n-char-sequence)" ignored
                        while (!eof ())
                            if (')' == *_C_in++) {
                                this->_C_state |= this->_C_nan;
                                return __first;
                            }
                    }
                    break;   // error
                }
                else if (eof () || (__ctp.tolower)(char(*_C_in)) != *__p)
                    break;   // error
            }
        }
    }

    if (__first == this->_C_digits)
        this->_C_error |= this->_C_no_digits;

    this->_C_exp_beg  = __first;
    this->_C_state   &= ~this->_C_exp_negative;

    _CharT __ch;

    if (   !this->_C_error && !eof ()
        && (  (__ch = *_C_in) == __punct [this->_C_e]
            || __ch == __punct [this->_C_E])) {

        // advance past 'e' or 'E' and append exponent
        // to the end of internal buffer
        ++_C_in;
        if (eof ())
            this->_C_error |= this->_C_bad_exponent;
        else {
            // check for '+' or '-' after 'e' or 'E'
            if ((__ch = *_C_in) == __punct [this->_C_plus])
                ++_C_in;
            else if (__ch == __punct [this->_C_minus]) {
                this->_C_state |= this->_C_exp_negative;
                ++_C_in;
            }

            // read _C_in (decimal) exponent digits
            int __radix   = this->_C_base;
            this->_C_base = 10;
            __first       = _C_get_digit_string (__first);
            this->_C_base = __radix;

            if (__first == this->_C_exp_beg)
                this->_C_error |= this->_C_bad_exponent;
        }
    }

    return __first;
}


template <class _CharT, class _InputIter>
char* __rw_digit_reader<_CharT, _InputIter>::
    _C_get_digit_groups (char *__first) {

    char *__eod=_C_get_digit_string (__first);
    
    if (_C_gpos==_C_groups+sizeof _C_groups)
        this->_C_error|=this->_C_too_many_groups;
    else {
        ptrdiff_t __i=__eod-__first;
        if (__i >= CHAR_MAX)
            this->_C_error|=this->_C_group_too_long;
        else {
            if (__i!=0) {
                *_C_gpos++=char(__i);
                if (!eof () && *_C_in==thousands_sep) {
                    ++_C_in;
                    __eod=_C_get_digit_groups (__eod);
                }
            }
        }
    }
    
    return __eod;
}


template <class _CharT, class _InputIter>
int __rw_digit_reader<_CharT, _InputIter>::
    _C_get_keyword (const __rw_keyword_map<_CharT> &__in_map) {

    const __rw_keyword_def<_CharT> *__map_ptr=__in_map._C_defs;
    const __rw_keyword_def<_CharT> *__end_ptr=__in_map._C_defs+__in_map._C_numdefs;
    
    __rw_keyword_def<_CharT> __can_ary[40],       // Candidate names.
        *__can_ptr=__can_ary; 
    
    const _CharT *__name;
    int           __res;
    _CharT        __input;
    
    // The first pass produces a set of candidate
    // names in the local array. Successive passes
    // deal with local array exclusively -- iteratively
    // stripping matching characters and keeping their
    // related map info while discarding map info if not
    // matching.
    
    for ( ; ; ) {
        if (eof ()) {
            for ( ; __map_ptr<__end_ptr; ++__map_ptr) 
                if (!*__map_ptr->_C_name)               
                    return __map_ptr->_C_indx;            
            return -1;                                
        }
        
        __input=*_C_in;
        __res=-1;
        
        for ( ; __map_ptr<__end_ptr; ++__map_ptr) {
            __name=__map_ptr->_C_name;
            if (__input==*__name) {
                __can_ptr->_C_name=++__name;
                __can_ptr->_C_indx=__map_ptr->_C_indx;
                ++__can_ptr;                 
            }
            else if (*__name==0 )        // End of abbreviated name.
                __res=__can_ptr->_C_indx;// NOTE: If an abbreviation is followed
                                         // by characters that could match the 
        }                                // full name, continues reading until
                                         // full name is matched, else fails
                                         // (per 22.2.5.1.2-6 in the Standard)  
        if (__can_ptr==__can_ary)        // 
            return __res;                // EX: If table has "Sunday" and "Sun",
                                         // input "Sund" will fail, but "Suni"
                                         // evals to "Sun".
        
        ++_C_in;
        
        if (__can_ptr==__can_ary+1) { // Only one matching candidate name left.
            for (__name=__can_ary[0]._C_name; *__name; ++__name) {
                if (eof () || *_C_in!=*__name) 
                    return -1;
                ++_C_in;
            }
            return __can_ary[0]._C_indx;
        }
        
        __end_ptr =__can_ptr;         // After first pass, vars refer
        __can_ptr =__can_ary;         // to local array.
        __map_ptr =__can_ary;         
        
    }

}

// ---------------------------------------------------
// Template __rw_digit_writer_base_1 member templates.
// ---------------------------------------------------


template <class _CharT>
__rw_digit_writer_base_1<_CharT>::
__rw_digit_writer_base_1(ios_base                      &__flags,
                         const __rw_punct_data<_CharT> &__mp)
    : __rw_digit_writer_base (__flags),
      __rw_digit_handler_base_1<_CharT>(__flags.getloc (), __mp) {

    _C_flags    &= ~(ios_base::floatfield | ios_base::showpos);
    _C_flags    |= ios_base::fixed;
    _C_base      = 10;
    _C_precision =  0;
}

// --------------------------------------------
// Template __rw_digit_writer member templates.
// --------------------------------------------

template <class _CharT, class _OutputIter>
void __rw_digit_writer<_CharT, _OutputIter>::_C_put_digits (_CharT __fill) {

    char *__p=this->_C_start;
    bool __has_sign=false, __has_point=false,
        __has_prefix_8=false, __has_prefix_16=false;
    
    if (__p<this->_C_end && (*__p==' ' || *__p=='-' || *__p=='+')) {
        __has_sign=true;
        __p++;
    }

    if (__p+1 < this->_C_end
        && (this->_C_base == 16 && *__p == '0'
            && (*(__p+1) == 'x') || *(__p+1) == 'X'))
        __has_prefix_16 = true;
    
    if (__p+1 < this->_C_end
        && (this->_C_base == 8 && *__p == '0'))
        __has_prefix_8 = true;
            
    //  Locate the end of the integral digits.
    char *__dec;
    if (!this->_C_fractional)
        __dec=this->_C_end;
    else {
        __dec=this->_C_start;
        if (__has_sign) __dec++;
        for ( ; __dec<this->_C_end; ++__dec)
            if (*__dec<'0' || *__dec>'9') {
                if (*__dec!='e' && *__dec!='E')
                    __has_point=true;
                break;
            }
    }

    
    // Calculate the number and pattern of separators needed if any.
    _CharT __separator = _CharT ();
    
    _RWSTD_C::ptrdiff_t __ungrouped = __dec - this->_C_start;
    if (__has_sign)
        __ungrouped--;
    if (__has_prefix_16)
        __ungrouped -= 2;
    if (__has_prefix_8)
        __ungrouped--;

    if (this->_C_separable) {
        __ungrouped = this->_C_calc_groups (int (__ungrouped),
                                            this->_C_get_grouping ());
        if (this->_C_num_groups)
            __separator=this->_C_get_thousands_sep ();
    }
    
    // Compute the number of __fill _CharT-s needed, and where they should be put.
    ptrdiff_t __left_fill=0, __internal_fill=0, __right_fill=0;
    if (this->_C_width>0) {
        ptrdiff_t __w=this->_C_width - (this->_C_end - this->_C_start)
                                     - this->_C_num_groups;
        this->_C_width=0;

        if (__w>0) {
            switch (this->_C_adjust) {
            case __rw_digit_writer_base::_C_left:
                __right_fill=__w;
                break;
            case __rw_digit_writer_base::_C_internal:
                __internal_fill=__w;
                break;
            default:
                __left_fill=__w;
            }
        }
    }
    
    // widen the sign + digits + exponent string

    // default buffer, will allocate larger if necessary
    _CharT  __widebuf [__rw_digit_writer_base::_C_DEF_BUFSIZE];
    _CharT *__wide_digits = __widebuf;

    // increase the size of the buffer
    if (this->_C_end - this->_C_start >= __rw_digit_writer_base::_C_DEF_BUFSIZE)
        __wide_digits = new _CharT [this->_C_end - this->_C_start + 1];
    
    this->_C_ctyp.widen (this->_C_start, this->_C_end, __wide_digits);
    
    // Write the widened string with fill and decorations to output.
    _CharT *__digit_pos = __wide_digits;
    while (__left_fill--)
        *_C_out++ = __fill;

    if (__has_sign)
        *_C_out++ = *__digit_pos++;    // the widened sign

    if (__has_prefix_16) {
        *_C_out++ = *__digit_pos++; // the "0"
        *_C_out++ = *__digit_pos++; // the "x"
    }
    if (__has_prefix_8) 
        *_C_out++ = *__digit_pos++; // the "0"

    while (__internal_fill--)
        *_C_out++ = __fill;

    while (__ungrouped--)
        *_C_out++ = *__digit_pos++;

    while (this->_C_num_groups--) {
        *_C_out++ = __separator;

        while (this->_C_group[0]--)
            *_C_out++=*__digit_pos++;
        ++this->_C_group;
    }
    
    if (__has_point) {
        *_C_out++=this->_C_get_decimal_point ();
        ++__digit_pos;
    }
    
    __ungrouped = this->_C_end - __dec;

    if (__has_point)
        __ungrouped--;

    while ((__ungrouped--) > 0)
        *_C_out++ = *__digit_pos++;
    
    while (__right_fill--)
        *_C_out++ = __fill;

    if (__wide_digits != __widebuf)
        delete[] __wide_digits;
}

template <class _CharT, class _OutputIter>
void __rw_digit_writer<_CharT, _OutputIter>::
    _C_put_keyword (const string_type &__instr, _CharT __fillchar) {

    ptrdiff_t __left_fill=0, __right_fill=0;
    ptrdiff_t __fillnum=this->_C_width-__instr.length ();
    if (__fillnum>0) {
        switch (this->_C_adjust) {
        case __rw_digit_writer_base::_C_left:
            __right_fill=__fillnum;
            break;
        case __rw_digit_writer_base::_C_internal:
        default:
            __left_fill=__fillnum;
        }
        if (__left_fill)
            do *_C_out++=__fillchar;
            while (--__left_fill);
    }
    
    const _CharT *__tmp=__instr.c_str (), *__endtmp=__tmp+__instr.length ();
    while (__tmp<__endtmp)
        *_C_out++=*__tmp++;
    
    if (__right_fill)
        do *_C_out++=__fillchar;
        while (--__right_fill);
}

_RWSTD_NAMESPACE_END   // __rw
