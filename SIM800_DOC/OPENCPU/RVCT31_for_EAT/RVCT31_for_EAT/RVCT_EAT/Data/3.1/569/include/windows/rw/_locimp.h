/***************************************************************************
 *
 * _locimp.h - Declarations for the Standard Library locale private
 *             implementation classes.
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _locimp.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_LOCIMP_H_INCLUDED
#define _RWSTD_LOCIMP_H_INCLUDED

#include <rw/_mutex.h>
#include <rw/_defs.h>

_RWSTD_NAMESPACE_BEGIN (std)

class _RWSTD_EXPORT locale;

template <class _CharT>
class ctype;

template <class _CharT>
class ctype_byname;

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT ctype<char>;

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT ctype_byname<char>;

#ifndef _RWSTD_NO_WCHAR_TYPE

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT ctype<wchar_t>;

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT ctype_byname<wchar_t>;

#endif   // _RWSTD_NO_WCHAR_TYPE

_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)

// Implementation class template -- __rw_timepunct<_CharT>
//
// A facet such as this should have been included in the standard.  We just
// declare it here; the definition occurs below, after locale::facet has been
// defined.

template <class _CharT> class __rw_timepunct;


// Implementation forward declarations:

class _RWSTD_EXPORT __rw_locale_imp;

class _RWSTD_EXPORT __rw_facet_base;

struct _RWSTD_EXPORT __rw_digit_map_base;

template <class _CharT>
class __rw_digit_map;

template <class _CharT>
struct __rw_keyword_def;

template <class _CharT>
struct __rw_keyword_map;

template <class _CharT>
class __rw_keyword_cracker;

class _RWSTD_EXPORT __rw_digit_reader_base;

template <class _CharT>
class __rw_digit_reader_base_1;

template <class _CharT, class _InputIter>
class __rw_digit_reader;

class _RWSTD_EXPORT __rw_digit_writer_base;

template <class _CharT>
class __rw_digit_writer_base_1;

template <class _CharT, class _OutputIter>
class __rw_digit_writer;


// We would prefer to define locale::id and locale::facet entirely as nested
// classes of locale, but current compilers have problems with out-of-line
// definition of members of such classes, so we have to derive most of their
// behavior from unnested implementation classes:

class _RWSTD_EXPORT __rw_facet_base: public __rw_synchronized
{
    friend class __rw_locale_imp;
    friend class _STD::locale;

public:

    enum {
        _C_facet_cat    = 0,   // facet category
        _C_ok_implicit  = 0,   // ok to create implicitly?
        _C_initialized = 1    // has facet been initialized?
    };

    void _C_set_ref (size_t __ref) {
        _C_ref_count = __ref;
    }

private:

    int              _C_category;     // same type as locale::category
    int              _C_flags;        // various flags (used externally)
    _RWSTD_C::size_t _C_ref_count;    // reference counter

protected:

    __rw_facet_base (_RWSTD_C::size_t __ref, int __cat = 0)
        : _C_category (__cat),  _C_flags (0), _C_ref_count (__ref) { }

    __rw_facet_base (const __rw_facet_base &__rhs)
        : __rw_synchronized (),
          _C_category (__rhs._C_category),
          _C_flags (__rhs._C_flags),
          _C_ref_count (__rhs._C_ref_count) { }

    __rw_facet_base& operator= (const __rw_facet_base &__rhs) {
        if (this != &__rhs) {
            _C_category  = __rhs._C_category;
            _C_flags     = __rhs._C_flags;
            _C_ref_count = __rhs._C_ref_count;
        }
        return *this;
    }

    virtual ~__rw_facet_base () { }

    // _C_initfacet() is called by locale::__install the first time a facet
    // is installed in its first locale.  Some facets override it to set up
    // private  data that depends on return values of virtual do_xxx
    // functions that can't be called yet in a constructor.

    virtual void _C_initfacet (const _STD::locale&) { }
};


_RWSTD_NAMESPACE_END   // __rw


#include <rw/_locvector.h>


_RWSTD_NAMESPACE_BEGIN (__rw)

// ----------------------------------------
// Implementation class -- __rw_locale_imp.
// ----------------------------------------

class _RWSTD_EXPORT __rw_locale_imp: public __rw_synchronized
{
public:
    // for convenience
    typedef _RWSTD_C::size_t size_type;

    friend class _RWSTD_EXPORT _STD::locale;
    
    enum {
        _C_none     = 0x0000,
        _C_collate  = 0x0010,
        _C_ctype    = 0x0020,
        _C_monetary = 0x0040,
        _C_numeric  = 0x0080,
        _C_time     = 0x0100,
        _C_messages = 0x0200,
        _C_all      =   _C_none | _C_collate | _C_ctype | _C_monetary
                      | _C_numeric | _C_time | _C_messages,

        // implementation details
        _C_libc_constants = 0x000f,
        _C_first_category = _C_collate,
        _C_n_categories   = 6
    };

    __rw_locale_vector<_STD::string>     _C_facet_names;  // names of facets
    __rw_locale_vector<__rw_facet_base*> _C_facets;       // pointers to facets

    int _C_native;
    int _C_named;

    // locale name; 0 if unnamed, "", "C", or implementation-defined
    const char *_C_name;

    size_type _C_ref_count;   // reference count

    __rw_locale_imp (const char* = 0, size_type = 36, size_type = 0);

    __rw_locale_imp (const __rw_locale_imp&, const char*, size_type);

    ~__rw_locale_imp () {
        // cast is for non-conforming compilers, such as MSVC6.3
        delete [] _RWSTD_CONST_CAST(char*,_C_name);
    }

    __rw_facet_base* _C_get_facet (size_type __inx) const {
        return __inx < _C_facets.size () ? _C_facets [__inx] : 0;
    }

private:
    inline const char *_C_category_name (int) const;

    // Map C library LC_xxx constants into facet categories.
    static int _C_map_category (int);

    // Parse a locale name into category names.
    static bool _C_parse_name (__rw_locale_vector<_STD::string>&, const char*);
};


// ---------------------------------
// Inline members of __rw_locale_imp
// ---------------------------------

inline __rw_locale_imp::__rw_locale_imp (const char *__name,
                                         size_type __size, size_type __ref)
    : _C_facets (__size, 0), 
      _C_native (0), 
      _C_named (0),
      _C_name (0),
      _C_ref_count (__ref) {


    if (__name) {
        __size = _RWSTD_C::strlen (__name) + 1;

        _C_name = _RWSTD_STATIC_CAST (char*,
                                      _RWSTD_C::memcpy (new char [__size],
                                                        __name, __size));
    }

    _C_facet_names.resize (_C_n_categories, _STD::string ());
}


inline __rw_locale_imp::__rw_locale_imp (const __rw_locale_imp &__rhs,
                                         const char *__name, size_type __ref)
    : _C_facet_names (__rhs._C_facet_names), 
      _C_facets (__rhs._C_facets), 
      _C_native (__rhs._C_native), 
      _C_named (__rhs._C_named), 
      _C_name (0),
      _C_ref_count (__ref) {

    
    if (__name) {
        size_type __size = _RWSTD_C::strlen (__name) + 1;
        _C_name =
            _RWSTD_STATIC_CAST (char*,
                                _RWSTD_C::memcpy (new char [__size], __name,
                                                  __size));
    }

    for (size_type __i = _C_facets.size (); __i--; ) {
        if (_C_facets [__i])
            _RWSTD_ATOMIC_PREINCREMENT (_C_facets [__i]->_C_ref_count,
                                        _C_facets [__i]->_C_mutex);
    }
}


inline const char*
__rw_locale_imp::_C_category_name (int __cat) const {

    int __mask = _C_first_category;

    for (size_type __i = 0;
         __i < size_type (_C_n_categories); ++__i, __mask <<= 1)

        if (__cat & __mask)
            return _C_facet_names[__i].c_str();

    return "";
}

// --------------------------------------------
// Implementation class -- __rw_digit_map_base.
// --------------------------------------------

// A place to stash some static constants, so that each instantiation of the
// derived class rwstd::__rw_digit_map does not have to have a separate copy.

struct _RWSTD_EXPORT __rw_digit_map_base
{
    enum { _C_zero, _C_minus, _C_plus, _C_X, _C_x, _C_E, _C_e };
    static const char _C_punct_chars[7];   // "0-+XxEe"
    static const char _C_digit_chars[22];  // "0123456789ABCDEFabcdef"
    static const char _C_char_values[22];  // values in range 0-15
};

// -------------------------------------------------------
// Implementation class template -- __rw_digit_map<_CharT>.
// -------------------------------------------------------

// Maps digits into their corresponding numeric values, and caches widened
// equivalents of some number-related punctuation characters that don't depend
// on the numpunct facet.  A private instance of this class is hidden in
// ctype<_CharT> for use by numeric conversion facets.  A call to init must
// precede the first call to _C_eval if _C_is_inited() is false.  Eval returns 0-15
// if argument is a valid digit, a negative value otherwise.
//
// Specialized for char for performance.  The specialization assumes digits
// fit into the char code-set in an ASCII-like manner ('0'..'9' contiguous,
// 'A'..'F' contiguous, 'a'..'f' contiguous, '0' < 'A' < 'a').

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT __rw_digit_map<char>
    : public __rw_digit_map_base
{
 public:
  typedef char char_type;

  bool        _C_is_inited (void) const {
      return true;
  }
  const char *_C_get_punct (void) const {
      return _C_punct_chars;
  }
  inline int  _C_eval (char) const;

  static inline const __rw_digit_map<char>&
  _C_get_digit_map (const _STD::ctype<char>&);
};

// Inline members of __rw_digit_map<char>:
//
// (Note that the definition of _C_get_digit_map is in <rw/_ctype.h> because it
//  has to come after the definition of ctype<char>.)

inline int __rw_digit_map<char>::_C_eval (char __c) const
{
    int __n =__c;
    if ((__n -= '0') > 9) {
        if ((   (__n -= ('A' - '0')) > 5
             && (__n -= ('a' - 'A')) > 5) || (__n += 10) < 10)
            __n = -1;
    }
    return __n;
}

template <class _CharT>
class __rw_digit_map
    : public __rw_digit_map_base
{
  bool _C_inited;
  _CharT _C_punct_array[7];
  _CharT _C_digit_array[22];
  char _C_value_array[22];
 public:
  typedef _CharT char_type;

  __rw_digit_map (void): _C_inited(false)
        { }
  bool          _C_is_inited (void) const {
      return _C_inited;
  }
  void          _C_init (const _STD::ctype<_CharT>& ct);

  const _CharT *_C_get_punct (void) const {
      return _C_punct_array;
  }
  int           _C_eval (_CharT) const;

    // Can-opener for getting the __rw_digit_map out of a ctype.
    // (Works because of the friend declaration in
    // __rw_ctype_helper<_CharT> below.)
  static const __rw_digit_map<char_type>&
  _C_get_digit_map (const _STD::ctype<char_type>& __ctp) {
      if (!__ctp._C_digit_map._C_inited) {
          __rw_digit_map<char_type> &__map = 
              _RWSTD_CONST_CAST (__rw_digit_map<char_type>&,
                                 __ctp._C_digit_map);

          __map._C_init (__ctp);
      }
      return __ctp._C_digit_map;
  }
};


template <class _CharT>
inline void 
__rw_digit_map<_CharT>::_C_init (const _STD::ctype<_CharT> &__ctp) {

    __ctp.widen (_C_punct_chars, _C_punct_chars + sizeof _C_punct_chars,
                _C_punct_array);

    __ctp.widen (_C_digit_chars, _C_digit_chars + sizeof _C_digit_chars,
                _C_digit_array);

    _RWSTD_C::memcpy (_C_value_array, _C_char_values, sizeof _C_value_array);
    _C_inited = true;
}


template <class _CharT>
inline int __rw_digit_map<_CharT>::_C_eval (_CharT __c) const {

    const _CharT *__end = _C_digit_array + sizeof _C_value_array;

    for (const _CharT *__p = _C_digit_array; __p != __end; ++__p)
        if (*__p == __c)
            return _C_value_array [__p - _C_digit_array];

    return -1;
}


// ------------------------------------------------------
// Implementation function templates -- create_xxx_facet.
// ------------------------------------------------------

// The __rw_facet_maker<Facet>::_C_maker_func functions described above delegate
// actual construction of facets to three inline function templates named
// create_xxx_facet, where xxx is 'classic' or 'native' or 'named'.  The
// default (template) versions of these functions construct facets as follows:
//
//   classic -- default constructor for the facet with only the refs argument.
//   native -- calls create_named_facet with a name of "".
//   named -- calls create_classic_facet, ignoring the passed name.
//
// This default behavior is overridden (specialized) for certain facet types.
// In particular, create_named_facet is specialized for all facet types that
// have a derived _byname version, to construct that version with the passed
// name (see <rw/_locale.h>) and create_native_facet is specialized for all
// facet types whose "native" behavior (as determined by the vendor) differs
// from the byname facet with a name of "" (see <rw/vendor>).

template <class _Facet>
inline _Facet*
__rw_create_named_facet (_Facet*, const char*, _RWSTD_C::size_t);

template <class _Facet>
inline _Facet*
__rw_create_native_facet (_Facet*);

template <class _Facet>
inline _Facet*
__rw_create_classic_facet (_Facet*)
{
    return new _Facet ();
}

// ---------------------------------------------------------
// Implementation class template -- __rw_facet_maker<Facet>.
// ---------------------------------------------------------

// When use_facet (inline) finds that a locale does not contain an explicit
// facet of the requested type, it calls locale::_C_make_facet
// (non-template) to create or find the facet in a cache, and install it
// in the locale.  As a parameter to __make_explicit, use_facet passes
// a call-back function which _C_make_facet can call to construct
// a facet of the requested type if needed.  The call-back functions are
// obtained by instantiating the following helper class template:

template <class _Facet>
struct __rw_facet_maker
{
    static __rw_facet_base* _C_maker_func (int, const char*, _RWSTD_C::size_t);
};


template <class _Facet>
inline
__rw_facet_base* __rw_facet_maker<_Facet>::
_C_maker_func (int __t, const char* __name, _RWSTD_C::size_t __ref)
{
    switch (__t) {
    case 0:  return __rw_create_classic_facet ((_Facet*)0);
    case 1:  return __rw_create_native_facet ((_Facet*)0);
    }
    return __rw_create_named_facet ((_Facet*)0, __name, __ref);
}


// Typedef for the above __rw_facet_maker functions, for use in the declaration
// of locale::_C_make_facet.

typedef __rw_facet_base*
__rw_facet_maker_func (int, const char*, _RWSTD_C::size_t);


_RWSTD_NAMESPACE_END   // __rw


#endif   //_RWSTD_LOCIMP_H_INCLUDED
