/***************************************************************************
 *
 * _time.cc - Definitions for the Standard Library time facets
 *
 * $Id: _time.cc 91768 2005-08-10 18:58:48Z drodgman $
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

#include _RWSTD_CTIME

_RWSTD_NAMESPACE_BEGIN (__rw)

// -------------------------------------------
// Template __rw_time_reader member templates.
// -------------------------------------------

template <class _CharT, class _InputIter>
bool __rw_time_reader<_CharT, _InputIter>::
    _C_get_time_pattern (const string_type &__patt, tm *__tm) {

    enum {
        __edit_year    = 0x01, 
        __edit_month   = 0x02, 
        __edit_century = 0x04, 
        __edit_hour    = 0x08
    };
    

    int __post_edits = 0;

    const _CharT *__p = __patt.c_str (), *__patt_end = __p+__patt.length ();
    for ( ; __p<__patt_end; ++__p) {
        switch (this->_C_ctyp.narrow (*__p, ' ')) {
        case '%':
        {
            int *__iresult;
            int __ampm_dummy;  // scratch space for ampm value
            const __rw_keyword_map<_CharT> *__keywords = 0;
            
            switch (this->_C_ctyp.narrow (*++__p, ' ')) {
            case 'a':
            case 'A':
                // Day of week (abbreviated or not).
                __keywords = &this->_C_get_day_map (_C_timp);
                __iresult  = &__tm->tm_wday;
                break;
            case 'b':
            case 'B':
                // Month name (abbreviated or not).
                __keywords = &this->_C_get_month_map (_C_timp);
                __iresult  = &__tm->tm_mon;
                break;
            case 'm':
                // Numeric month, comes in 1 higher than the number we want.
                __post_edits |= __edit_month;
                __iresult     = &__tm->tm_mon;
                break;
            case 'c':
                // Full date, time and year in default format.
                return _C_get_time_pattern (this->_C_get_patt_string (_C_timp,
                                                                      2),
                                            __tm);
            case 'd':
                // Day of month.
                __iresult = &__tm->tm_mday;
                break;
            case 'I': // 12-hour clock
            case 'H': // 24-hour clock
                // Hours (12/24 distinction is made in the 'p' section)
                __iresult = &__tm->tm_hour;
                break;
            case 'p':
                // am/pm indicator
                __keywords = &this->_C_get_ampm_map (_C_timp);
                //  Assumes _CharT[0] are equal means entire string will be
                if (*this->_C_in == this->_C_get_ampm_string (_C_timp, true)[0])
                    __post_edits |= __edit_hour;
                __iresult = &__ampm_dummy;
                break;
            case 'M':
                // Minutes.
                __iresult = &__tm->tm_min;
                break;
            case 'S':
                // Seconds.
                __iresult = &__tm->tm_sec;
                break;
            case 'Y':
                // Year with Century.
                __post_edits |= __edit_year;
                __iresult = &__tm->tm_year;
                break;
            case 'y':
                // 2-digit Year without Century.
                __post_edits |= (__edit_year|__edit_century);
                __iresult = &__tm->tm_year;
                break;
            default:
                // All other format characters are not supported on input.
                return false;
            }
            
            if (__keywords) {
                if ((*__iresult = this->_C_get_keyword (*__keywords)) < 0)
                    return false;
            }
            else {
                this->_C_base = 10;
                *__iresult =
                    this->_C_parse (this->_C_get_digit_string (this->_C_digits),
                                    0UL);
            }
        }
        break;
        default:
            if (!this->eof () && *this->_C_in==*__p)
                ++(this->_C_in);
            else
                return false;
        }
        
        if (this->_C_error)
            return false;
    }
    
    if (__post_edits&__edit_year)
        if (__post_edits&__edit_century && __tm->tm_year<100)
            ; // 2-digit year is already relative to 1900
        else
            __tm->tm_year -= 1900;
    
    if (__post_edits&__edit_month)
        __tm->tm_mon--;
    
    if (__post_edits&__edit_hour)
        if (__tm->tm_hour < 12)
            __tm->tm_hour+=12;
    
    return true;
}

_RWSTD_NAMESPACE_END   // __rw

_RWSTD_NAMESPACE_BEGIN(std)

// ----------------------------------------------------------
// Facet time_get<_CharT, _InputIter> member templates.
// ---------------------------------------------------------

template <class _CharT, class _InputIter>
locale::id time_get<_CharT, _InputIter>::id;


template <class _CharT, class _InputIter>
time_base::dateorder time_get<_CharT, _InputIter>::do_date_order () const {

    _C_check_init (); 

    // We would prefer to return a value that matches the date format defined
    // in the timepunct facet in the caller's locale, but we can't get there
    // from here ...

    const _TYPENAME _RW::__rw_timepunct<_CharT>::string_type& __s =
        _RW::__rw_keyword_cracker<_CharT>::_C_get_patt_string (*_C_timp, 0);

    bool __haveYear = false;

    for (size_t __i = 0; __i < __s.length (); ++__i) {

        if (__s[__i] == _CharT ('y') || __s[__i] == _CharT ('Y'))
            __haveYear = true;

        if ( __s[__i] == _CharT ('d') || __s[__i] == _CharT ('a')
             || __s[__i] == _CharT ('A')) {

            if (__haveYear)
                return ydm;
            else
                return dmy;
        }
        if (__s[__i] == _CharT ('m') || __s[__i] == _CharT ('b')
            || __s[__i] == _CharT ('B')) {

            if (__haveYear)
                return ymd;
            else
                return mdy;
        }
    }

    return no_order;
}

template <class _CharT, class _InputIter>
_TYPENAME time_get<_CharT, _InputIter>::iter_type
time_get<_CharT, _InputIter>::do_get_time (_InputIter         __in,
                                           _InputIter         __end,
                                           ios_base&          __io,
                                           ios_base::iostate& __err,
                                           tm*                __tm) const {
    _C_check_init (); 

    _RW::__rw_time_reader<_CharT, _InputIter>
        __rdr (__in, __end, __io, *_C_timp);

    __err = ios_base::goodbit;

    // Parse according to pattern 1 (strftime '%X' -- default time pattern)
    if (!__rdr._C_get_time_pattern (__rdr._C_get_patt_string (__rdr._C_timp, 1),
                                    __tm))
        __err = ios_base::failbit;

    if (__rdr.eof ())
        __err |= ios_base::eofbit;
    
    return __in;
}

template <class _CharT, class _InputIter >
_TYPENAME time_get<_CharT, _InputIter>::iter_type
time_get<_CharT, _InputIter>::do_get_date (_InputIter         __in,
                                           _InputIter         __end,
                                           ios_base&          __io,
                                           ios_base::iostate& __err,
                                           tm*                __tm) const {

    _C_check_init (); 
    _RW::__rw_time_reader<_CharT, _InputIter>
        __rdr (__in, __end, __io, *_C_timp);

    __err = ios_base::goodbit;

    // Parse according to pattern 0 (strftime '%x' -- default date pattern)
    if (!__rdr._C_get_time_pattern (__rdr._C_get_patt_string (__rdr._C_timp, 0),
                                    __tm))
        __err = ios_base::failbit;

    if (__rdr.eof ())
        __err |= ios_base::eofbit;

    return __in;
}

template <class _CharT, class _InputIter >
_TYPENAME time_get<_CharT, _InputIter>::iter_type
time_get<_CharT, _InputIter>::do_get_weekday (_InputIter          __in,
                                              _InputIter         __end,
                                              ios_base&          __io,
                                              ios_base::iostate& __err,
                                              tm*                __tm) const {
    _C_check_init (); 

    _RW::__rw_time_reader<_CharT, _InputIter> 
        __rdr (__in, __end, __io, *_C_timp);

    int __wd = __rdr._C_get_keyword (__rdr._C_get_day_map (__rdr._C_timp));
    __err = ios_base::goodbit;

    if (__wd<0)
        __err = ios_base::failbit;
    else
        __tm->tm_wday = __wd;

    if (__rdr.eof ())
        __err |= ios_base::eofbit;

    return __in;
}

template <class _CharT, class _InputIter >
_TYPENAME time_get<_CharT, _InputIter>::iter_type
time_get<_CharT, _InputIter>::do_get_monthname (_InputIter         __in,
                                                _InputIter         __end,
                                                ios_base&          __io,
                                                ios_base::iostate& __err,
                                                tm*              __tm) const {
    _C_check_init (); 

    _RW::__rw_time_reader<_CharT, _InputIter>
        __rdr (__in, __end, __io, *_C_timp);

    int __mo = __rdr._C_get_keyword (__rdr._C_get_month_map (__rdr._C_timp));
    __err = ios_base::goodbit;

    if (__mo<0)
        __err = ios_base::failbit;
    else
        __tm->tm_mon = __mo;

    if (__rdr.eof ())
        __err |= ios_base::eofbit;

    return __in;
}

template <class _CharT, class _InputIter >
_TYPENAME time_get<_CharT, _InputIter>::iter_type
time_get<_CharT, _InputIter>::do_get_year (_InputIter         __in,
                                           _InputIter         __end,
                                           ios_base&          __io,
                                           ios_base::iostate& __err,
                                           tm*                __tm) const {

    _C_check_init (); 

    _RW::__rw_time_reader<_CharT, _InputIter> 
        __rdr (__in, __end, __io, *_C_timp);

    int __yr = __rdr._C_parse (__rdr._C_get_digits (int ()), int ());
    __err = ios_base::goodbit;

    // 2-digit year numbers are accepted, but are not treated specially, and so
    // end up in the 1st century C.E.
    if (__rdr._C_error)
        __err = ios_base::failbit;
    else
        __tm->tm_year = __yr-1900;

    if (__rdr.eof ())
        __err |= ios_base::eofbit;

    return __in;
}

// ----------------------------------------------------------
// Facet time_put<_CharT, _InputIter> member templates.
// ----------------------------------------------------------

template <class _CharT, class _OutputIter>
locale::id time_put<_CharT, _OutputIter>::id;


template <class _CharT, class _OutputIter>
_OutputIter time_put<_CharT, _OutputIter>::put
    (_OutputIter __out, ios_base &__io, _CharT __fill, const tm *__tm, 
     const _CharT* __pattern, const _CharT* __patt_end) const {
    _C_check_init ();

    size_t __patt_len = __patt_end-__pattern;
    char __scratch [40];
    char *__narrow_patt = (__patt_len<=sizeof __scratch)
                          ? __scratch
                          : new char[__patt_len];
  
    _USE_FACET (ctype<_CharT>, __io.getloc ())
        .narrow (__pattern, __patt_end, ' ', __narrow_patt);

    char *__np = __narrow_patt;
    for (const _CharT* __wp = __pattern; __wp<__patt_end; ++__wp, ++__np)
        if (*__np!='%')
            *__out++ = *__wp;
        else if (++__wp<__patt_end) {
            switch (*++__np) {

            case 'O':
                // POSIX-style modifier
                if (++__wp<__patt_end)
                    __out = do_put(__out, __io, __fill,
                                   __tm, *++__np, 'O');
                break;

            case '%':
                // Literal percent sign
                *__out++ = *__wp;
                break;

            default:
                // Treat everything else as a time format specifier.
                __out = do_put (__out, __io, __fill, __tm, *__np, ' ');
            }
        }

    if (__narrow_patt!=__scratch)
        delete[] __narrow_patt;

    return __out;
}

template <class _CharT, class _OutputIter>
_OutputIter time_put<_CharT, _OutputIter>::
do_put (_OutputIter __out, ios_base &__io, _CharT __fill, const tm *__tm, 
        char __fmat, char __modif) const {

    _C_check_init (); 

    const _CharT __pcnt[] = { '%', '%', '\0' };
    if (*__pcnt == _CharT (__fmat))
        return put (__out, __io, __fill, __tm, __pcnt, __pcnt + 2);

    static const char do_as_pattern[] = "xXcDrT";
    const char *__p = strchr (do_as_pattern, __fmat);
    if (__p) {
        // Catch formats implemented as calls to the pattern-based
        // method before going to the expense of constructing a
        // __rw_digit_writer.
        unsigned __i = __p-do_as_pattern;

        const _TYPENAME _RW::__rw_timepunct<_CharT>::string_type& __s =
            _RW::__rw_keyword_cracker<_CharT>::_C_get_patt_string (*_C_timp,
                                                                   __i);

        return put (__out, __io, __fill, __tm,
                    __s.c_str (), __s.c_str ()+__s.length ());
    }
    
    _RW::__rw_digit_writer<_CharT, _OutputIter> __writer (__out, __io);
    __writer._C_width = 0;
    
    if ('O' == __modif) {
        // Uppercase letter O (not zero)
        // -- requests ordinal string if defined.
        int __n, __m;
        switch (__fmat) {
        case 'C':
            // Exclusive Rogue Wave extension: Lets you announce, "Welcome
            // to the %OC century!" (See note on unmodified 'C' below.)
            __n = (__tm->tm_year+1999)/100;
            break;
        case 'd':
        case 'e':
            __n = __tm->tm_mday;
            break;
        case 'H':
            __n = __tm->tm_hour+1;
            break;
        case 'I':
            __n = __tm->tm_hour+1;
            if (__n>12)
                __n -= 12;
            break;
        case 'm':
            __n = __tm->tm_mon+1;
            break;
        case 'M':
            __n = __tm->tm_min+1;
            break;
        case 'S':
            __n = __tm->tm_sec+1;
            break;
        case 'u':
            __n = __tm->tm_wday;
            if (__n==0) __n = 7;
            break;
        case 'w':
            __n = __tm->tm_wday;
            break;
        case 'U':
            // Week number of year (assuming weeks start on Sunday).
            // set __m to wday of first day of year
            __m = (__tm->tm_wday)-((__tm->tm_yday)%7);
            // Correct __m to account for "days before first Sunday"
            // and "Sunday first day of year" conditions
            if ( __m <= 0 ) 
                __m += 7;
            if ( __m == 7 )
                // Sunday is first day of year
                __n = ((__tm->tm_yday)/7)+1;
            else
                // if not, shift Sunday to first day of week 1
                __n = ((__tm->tm_yday)+__m)/7;
            break;
        case 'W':
            // Week number of year (assuming weeks start on Monday).
            // set __m to wday of first day of year
            __m = (__tm->tm_wday)-((__tm->tm_yday)%7);
            // Correct __m to account for "days before first Monday"
            // and "Monday first day of year" conditions
            if ( __m <= 0 ) 
                __m += 7;
            if ( __m == 1 )
                // Monday is first day of year
                __n = ((__tm->tm_yday)/7)+1;
            else
                // if not, shift Monday to first day of week 1
                __n = (((__tm->tm_yday)+(__m-1))/7);
            break;
        case 'y':
            __n = ((__tm->tm_year+1900)%100)+1;
            break;
        default:
            __n = 999; // Cause error output.
        }
        
        __writer._C_put_keyword
            (__writer._C_get_ord_string (*_C_timp, __n), __fill);
        
    }
    else {
        bool __abbrev = false;

        const _TYPENAME
            _RW::__rw_timepunct<_CharT>::string_type *__s = 0;

        _TYPENAME _RW::__rw_timepunct<_CharT>::string_type __tzs;

        int __n = 0, __m = 0;

        __writer._C_base = 10;
        __writer._C_separable = false;

        switch (__fmat) {
        case 'a':
            __abbrev = true;
        case 'A':
            // Week day name or abbreviation.
            __s = &__writer._C_get_day_string (*_C_timp,
                                               __tm->tm_wday, __abbrev);
            break;
            
        case 'b':
        case 'h':
            __abbrev = true;
        case 'B':
            // Month name or abbreviation.
            __s = &__writer
                ._C_get_month_string (*_C_timp, __tm->tm_mon, __abbrev);
            break;
            
        case 'C':
            // Century. Note that we begin the 20th century in 1901, not 1900.
            // The draft standard does not seem to address this controversy.
            __n = (__tm->tm_year+1999)/100;
            break;
            
        case 'd':
            // Day of month with leading zero.
            __writer._C_iprecision = 2;
        case 'e':
            // Day of month with leading blank.
            __n = __tm->tm_mday;
            __writer._C_width = 2;
            break;
            
        case 'H':
            // Hour (24-hour clock).
            __n = __tm->tm_hour;
            __writer._C_iprecision = 2;
            break;
            
        case 'I':
            // Hour (12-hour clock, caller must append am/pm to resolve
            // ambiguity).
            __n = __tm->tm_hour;
            if (__n==0)
                __n = 12;
            else if (__n>12)
                __n -= 12;
            __writer._C_iprecision = 2;
            break;
            
        case 'j':
            // 3-digit day of year.
            __n = __tm->tm_yday+1;
            __writer._C_iprecision = 3;
            break;
            
        case 'm':
            // Month number.
            __n = __tm->tm_mon+1;
            __writer._C_iprecision = 2;
            break;
            
        case 'M':
            // Minutes.
            __n = __tm->tm_min;
            __writer._C_iprecision = 2;
            break;
            
        case 'n':
            // Newline character.
            *__out++ = __writer._C_ctyp.widen ('\n');
            break;
            
        case 'p':
            // ante/post meridian string.
            __s = &__writer._C_get_ampm_string (*_C_timp, __tm->tm_hour>=12);
            break;
            
        case 'S':
            // Seconds.
            __n = __tm->tm_sec;
            __writer._C_iprecision = 2;
            break;
            
        case 't':
            // Tab character.
            *__out++ = __writer._C_ctyp.widen ('\t');
            break;
            
        case 'u':
            // Weekday (1-7, 1==Monday)
            __n = __tm->tm_wday;
            if (__n==0)
                __n = 7;
            break;
            
        case 'U':
            // Week number of year (assuming weeks start on Sunday).
            // set __m to wday of first day of year
            __m = (__tm->tm_wday)-((__tm->tm_yday)%7);
            // Correct __m to account for "days before first Sunday"
            // and "Sunday first day of year" conditions
            if ( __m <= 0 ) 
                __m += 7;
            if ( __m == 7 )
                // Sunday is first day of year
                __n = ((__tm->tm_yday)/7)+1;
            else
                // if not, shift Sunday to first day of week 1
                __n = ((__tm->tm_yday)+__m)/7;
            __writer._C_iprecision = 2;
            break;
            
        case 'w':
            // Weekday (0-6, 0==Sunday).
            __n = __tm->tm_wday;
            break;
            
        case 'W':
            // Week number of year (assuming weeks start on Monday).
            // set __m to wday of first day of year
            __m = (__tm->tm_wday)-((__tm->tm_yday)%7);
            // Correct __m to account for "days before first Monday"
            // and "Monday first day of year" conditions
            if ( __m <= 0 ) 
                __m += 7;
            if ( __m == 1 )
                // Monday is first day of year
                __n = ((__tm->tm_yday)/7)+1;
            else
                // if not, shift Monday to first day of week 1
                __n = (((__tm->tm_yday)+(__m-1))/7);
            __writer._C_iprecision = 2;
            break;
            
        case 'y':
            // Year without century.
            __n = (__tm->tm_year+1900)%100;
            __writer._C_iprecision = 2;
            break;
            
        case 'Y':
            // Year with century.
            __n = __tm->tm_year+1900;
            break;
            
        case 'Z':
            // Time Zone.
        {
            _RW::__rw_use_c_lib_locale __loc (__io.getloc ().name ().c_str ());
            const size_t __bufsize = 64U;
            char __buf[__bufsize];
            size_t __buflen = 0;
            if (0 != (__buflen = strftime (__buf, __bufsize, "x%Z", __tm))) {
                _CharT *__temp = new _CharT[__buflen];
                __writer._C_ctyp.widen (__buf+1, __buf+__buflen, __temp);
                __temp[__buflen-1] = _CharT ();
                __tzs = __temp;
                delete[] __temp;
                __s = &__tzs;
                break;
            }
        }
        
        default:
            // Everything else is an error.
            __s = &__writer._C_get_day_string (*_C_timp, 99, false);
        }
        
        if (__s)
            __writer._C_put_keyword (*__s, __fill);
        else {
            __writer._C_digitize ((unsigned long) __n);
            __writer._C_put_digits (__fill);
        }
    }
    
    return __out;
}


_RWSTD_NAMESPACE_END   // std
