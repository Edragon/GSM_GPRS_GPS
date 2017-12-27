/***************************************************************************
 *
 * _locale.h - Declarations for the Standard Library locale classes
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _locale.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_LOCALE_H_INCLUDED
#define _RWSTD_LOCALE_H_INCLUDED


#include <string>
#include <iosfwd>

#include <rw/_defs.h>
#include <rw/_error.h>
#include <rw/_locimp.h>

#include _RWSTD_CTIME
#include _RWSTD_CLOCALE


_RWSTD_NAMESPACE_BEGIN (std)

class _RWSTD_EXPORT locale;

template <class _CharT>
class collate;


#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE

template <class _Facet>
inline const _Facet& use_facet (const locale&);

template <class _Facet>
inline bool has_facet (const locale&) _THROWS (());

#else   // if defined (_RWSTD_NO_TEMPLATE_ON_RETURN_TYPE)

template <class _Facet>
inline const _Facet& use_facet (const locale&, _Facet*);

template <class _Facet>
inline bool has_facet (const locale&, _Facet*) _THROWS (());

#endif // _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE

_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (std)


class _RWSTD_EXPORT locale
{
public:
    class facet;
    class id;

    friend class facet;
    friend class id;

    typedef int category;

    _RWSTD_STATIC_CONST (category, none     = _RW::__rw_locale_imp::_C_none);
    _RWSTD_STATIC_CONST (category, collate  = _RW::__rw_locale_imp::_C_collate);
    _RWSTD_STATIC_CONST (category, ctype    = _RW::__rw_locale_imp::_C_ctype);
    _RWSTD_STATIC_CONST (category, monetary = _RW::__rw_locale_imp::_C_monetary);
    _RWSTD_STATIC_CONST (category, numeric  = _RW::__rw_locale_imp::_C_numeric);
    _RWSTD_STATIC_CONST (category, time     = _RW::__rw_locale_imp::_C_time);
    _RWSTD_STATIC_CONST (category, messages = _RW::__rw_locale_imp::_C_messages);
    _RWSTD_STATIC_CONST (category, all      = _RW::__rw_locale_imp::_C_all);
 
    // The default constructor creates a copy of the current global locale,
    // the locale specified in the most recent call to locale::global().
    // If locale::global() has not been called, it is the classic "C" locale.

    inline locale () _THROWS (());

    // The copy constructor (and the assignment operator, below) can be used
    // freely.  Like a string, most of a locale's contents are in a separate, 
    // reference-counted implementation object, so copying and assigning
    // locales/ has little overhead.

    inline locale (const locale& __rhs) _THROWS (())
        : _C_imp (__rhs._C_imp) {
        _RWSTD_ATOMIC_PREINCREMENT (_C_imp->_C_ref_count, _C_imp->_C_mutex);
    }

    // The following constructor creates a locale composed of by-name facets
    // and assigns it a name. The valid arguments are "", "C", and a set of
    // strings defined by the compiler vendor. These cause the facets of
    // the locale to be obtained, respectively, from the user's preferred
    // locale, from the classic locale, or from the compiler's locale
    // database. (In many cases, the preferred locale is specified by
    // environment variables such as LANG or LC_ALL.)  If the argument is
    // not recognized, the constructor throws runtime_error.

    _EXPLICIT locale (const char*);

    // The following constructor copies its first argument except for
    // the facets in the categories identified by the third argument,
    // which are obtained by name using the second argument.
    // Can throw runtime_error.

    locale (const locale&, const char*, category);

    // The following templatized constructor copies its first argument except
    // the single _Facet of type _Facet

#ifndef _RWSTD_NO_MEMBER_TEMPLATES

    template <class _Facet>
    locale (const locale&, _Facet*);

#else

    // If your compiler does not support member function templates, we provide
    // the following work-around to let you accrete facets onto a locale.
    // This constructor copies its first argument except for the single
    // _Facet of the type of the second argument, for which it uses
    // the second argument. To determine the type of the second argument,
    // it calls the non-standard virtual method _C_get_id in the second
    // argument.  If you are creating your own _Facet types on a compiler
    // that does not support member templates, you must code a _C_get_id
    // member as follows in each new base class _Facet (i.e. in each _Facet
    // class that has its own static member id of type locale::id):
    //
    //    virtual locale::id &_C_get_id (void) const { return id; }
    //
    // See the _C_get_id members in the standard facets below for examples.

    locale (const locale&, facet*);

#endif // _RWSTD_NO_MEMBER_TEMPLATES

    // The following constructor copies its first argument except for
    // the facets in the categories identified by the third argument,
    // which are obtained from the second argument.

    locale (const locale&, const locale&, category);

    ~locale () {
        // HACK: _C_imp should normally never be 0; internally, though,
        // it's set to 0 in locale::_C_make_facet to prevent a deadlock
        if (_C_imp && 0 == _RWSTD_ATOMIC_PREDECREMENT (_C_imp->_C_ref_count,
                                                       _C_imp->_C_mutex))
            delete _C_imp;
    }

    const locale& operator = (const locale&) _THROWS (());

    // The following member function returns a copy of this locale (*this)
    // except the single _Facet of type _Facet obtained from the other locale.

#ifndef _RWSTD_NO_MEMBER_TEMPLATES
#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE

    template <class _Facet>
    locale combine (const locale &__rhs) const {
        // The cast from (const _Facet*) to (_Facet*) highlights a possible
        // defect in the Standard ... see the issues list.
        return locale (*this,
                       _RWSTD_CONST_CAST(_Facet*, &_USE_FACET (_Facet, __rhs)));
    }


#endif // _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
#endif // _RWSTD_NO_MEMBER_TEMPLATES

    // The following returns the locale name, or "*" if the locale is unnamed.

    string name () const;

    // Two locales are equal if they are the same object, or one is a copy
    // of the other (i.e. they have the same implementation object),
    // or both are named and their names are the same.

    bool operator== (const locale &__rhs) const {
        return    this == &__rhs || _C_imp == __rhs._C_imp
               || name() != "*" && name () == __rhs.name();
    }

    bool operator != (const locale& __rhs) const {
        return !(*this == __rhs);
    }

    // The following templatized operator () satisfies STL requirements for
    // a comparator predicate template argument for comparing strings
    // according to the collating sequence of the locale. It lets you use
    // a locale directly as a comparator using syntax like
    //   sort(v.begin(), v.end(), loc),
    // where v is a vector of some string type and loc is a locale.

#ifndef _RWSTD_NO_MEMBER_TEMPLATES

    template <class _CharT, class _Traits, class _Allocator>
    bool
    operator() (const basic_string<_CharT, _Traits, _Allocator>& __x, 
                const basic_string<_CharT, _Traits, _Allocator>& __y) const;

#else   // if defined (_RWSTD_NO_MEMBER_TEMPLATES)

    bool operator() (const string &__x, const string &__y) const;

#ifndef _RWSTD_NO_WCHAR_T

    bool operator() (const wstring &__x, const wstring &__y) const;

#endif   // _RWSTD_NO_WCHAR_T
#endif   // _RWSTD_NO_MEMBER_TEMPLATES


    // Replaces the current global locale
    static locale global (const locale&);

    // Returns the classic "C" locale
    static const locale& classic ();

    // class _Facet -- base class for locale feature sets.

    // Any class deriving from _Facet that wants to be perceived as a distinct
    // _Facet, as opposed to a re-implementation of an existing _Facet, must
    // declare a static member: static std::locale::id id;

    class _RWSTD_EXPORT  facet: public _RW::__rw_facet_base {
        friend class _RW::__rw_locale_imp;
        friend class locale;

    protected:

        _EXPLICIT facet (size_t __ref = 0, int __cat = 0)
            : _RW::__rw_facet_base (__ref, __cat) { }
        
#ifdef _RWSTD_NO_MEMBER_TEMPLATES
        virtual id &_C_get_id (void) const = 0;
#endif

    private:

        facet (const facet&);            // not defined
        void operator= (const facet&);   // not defined
    };

    // class id -- _Facet type identifier.

    // This is an implementation class.  It is used internally as an index
    // to find facets within a locale.  Each distinct _Facet (i.e. each T
    // that can be the parameter of a use_facet<_TypeT> call) has a unique
    // static member of type locale::id named id.  The class is made public
    // to enable extension of the set of standard facets.  Objects of this
    // type don't need to be constructed or referenced in any other
    // circumstances.

    // the class must be `exported' in order for MSVC to collapse
    // the static in _C_init into a single object if linking to a DLL
    class _RWSTD_EXPORT id {

        friend class locale;

        _MUTABLE size_t _C_id;    // unique id
  
        void _C_init () const;

        operator size_t () const {
            return _C_id;
        }

    public:
        id (): _C_id (0) { }
        
    private:
        id (const id&);               // undefined
        void operator= (const id&);   // undefined
    };

    // Typedef for the implementation-defined call-back functions
    // that must be passed to _C_make_facet (below).
    typedef _RW::__rw_facet_maker_func __facet_maker_func;

    // MT-safe std::use_facet<>() implementation
    const facet& _C_use_facet (const id &__id, bool __implicit, category __cat,
                               __facet_maker_func __maker) const;

    // same as above but not MT-safe
    const facet& _C_unsafe_use_facet (const id &__id, bool __implicit,
                                      category __cat,
                                      __facet_maker_func __maker) const;

    // The following function retrieves an implicit _Facet from a cache, 
    // or creates one if needed (via call to the passed call-back
    // function), and makes it an explicit _Facet of the locale.

    facet* _C_make_facet (const id &__id, bool __implicit, 
                             category __cat, __facet_maker_func) const;

    facet* _C_get_facet (const id &__id) const {
        return _RWSTD_STATIC_CAST (facet*, _C_imp->_C_get_facet (__id));
    }


    void _C_install_facet (_RW::__rw_facet_base*, const id&) const;

private:

    friend class _RW::__rw_locale_imp;

    // Construct a locale from an implementation object.
    _EXPLICIT locale (_RW::__rw_locale_imp*);

    // Create initial implementation objects.
    static void _C_init ();

    // remove all facets satisfying the given category
    inline void _C_remove_facets (category);

    static _RW::__rw_locale_imp *_C_classic;   // the classic "C" locale
    static _RW::__rw_locale_imp *_C_native;    // the native "C++" locale
    static _RW::__rw_locale_imp *_C_global;    // the global locale

    _RW::__rw_locale_imp *_C_imp;   // pointer to a ref counted implementation
};


#if defined (_RWSTD_MULTI_THREAD) && !defined (_RWSTD_NO_STATIC_MUTEX_INIT)

_RWSTD_NAMESPACE_END   // std

_RWSTD_NAMESPACE_BEGIN (__rw)

// explicitly instantiated to work around a g++ 2.95.2 bug on COFF systems
// (such as IBM AIX or DEC OSF1) where it "forgets" to do so implicitly for
// explicitly initialized static data members
_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_static_mutex<_STD::locale>);

_RWSTD_NAMESPACE_END   // __rw

_RWSTD_NAMESPACE_BEGIN (std)

#endif   // _RWSTD_MULTI_THREAD && !_RWSTD_NO_STATIC_MUTEX_INIT


// ----------------------------
// Class locale inline members.
// ----------------------------


// Private constructor for use by implementation, constructs a locale from
// a __rw_locale_imp implementation object.
inline locale::locale (_RW::__rw_locale_imp *__rhs)
    : _C_imp (__rhs)
{
    _RWSTD_ASSERT (0 != __rhs);

    if (!_C_global)
        _C_init ();

    // no lock here to prevent a deadlock, called only from guarded contexts
    ++_C_imp->_C_ref_count;
}


// Default constructor, returns a copy of the current global locale.
inline locale::locale () _THROWS (())
{
    if (!_C_global)
        _C_init ();

    _C_imp = _C_global;

    _RWSTD_ASSERT (0 != _C_imp);

    _RWSTD_ATOMIC_PREINCREMENT (_C_imp->_C_ref_count, _C_imp->_C_mutex);
}


#ifndef _RWSTD_NO_MEMBER_TEMPLATES

// Constructor to accrete or replace a single _Facet.
template <class _Facet>
inline locale::locale (const locale& __rhs, _Facet* __facet)
{
    _RWSTD_ASSERT (0 != __rhs._C_imp);

    _RWSTD_MT_GUARD (__rhs._C_imp->_C_mutex);

    // Identical, EXCEPT no name -- so we can't just increment
    // the ref count on the existing imp -- we need a new imp.

    _C_imp = new _RW::__rw_locale_imp (*__rhs._C_imp, 0, 1);

    if (__facet) {
        _C_install_facet (__facet, _Facet::id);
    }
}


// Operator () compares two strings according to the collate<_CharT> facet of
// the locale.  It lets you use a locale object directly as a comparator
// predicate for strings in various algorithms.  For instance, you can sort
// a vector<string> according to the collating sequence of a locale with an
// expression like: sort(v.begin(),v.end(),mylocale).

template <class _CharT, class _Traits, class _Allocator>
inline bool
locale::operator() (const basic_string<_CharT,_Traits,_Allocator>& __x,
                    const basic_string<_CharT,_Traits,_Allocator>& __y) const
{
    return _USE_FACET (_STD::collate<_CharT>, *this)
        .compare (__x.data (), __x.data () + __x.length (),
                  __y.data (), __y.data () + __y.length ()) < 0;
}

#else   // if defined (_RWSTD_NO_MEMBER_TEMPLATES)

inline locale::locale (const locale& __rhs, facet* __facet):
    _C_imp (0)
{
    _RWSTD_MT_GUARD (__rhs._C_imp->_C_mutex);

    _C_imp = new _RW::__rw_locale_imp (*__rhs._C_imp, 0, 1);
    if (__facet)
        _C_install_facet (__facet, __facet->_C_get_id ());
}

#endif   // _RWSTD_NO_MEMBER_TEMPLATES

#ifdef _INLINE_WITH_STATICS

_INLINE_WITH_STATICS void locale::id::_C_init () const
{
    // implicit initialization used to prevent a g++ 2.95.2 warning
    // on Tru64: sorry: semantics of inline function static data are
    // wrong (you'll wind up with multiple copies)
    static size_t _C_id_gen /* = 0 */;

    if (!_C_id)
        _C_id = _RWSTD_ATOMIC_PREINCREMENT (_C_id_gen, false);
}

#endif   // _INLINE_WITH_STATICS


inline const locale::facet&
locale::_C_use_facet (const id &__id, bool __implicit, category __cat,
                      __facet_maker_func __maker) const
{
    _RWSTD_ASSERT (0 != _C_imp);

    // not guarded (heavily used in iostreams)
    const facet *__facet = _C_get_facet (__id);

    // If facet is not explicitly present in locale yet, use private function
    // locale::_C_make_facet to construct it or retrieve it from a cache,
    // and install it in the locale.  This function can throw bad_cast or
    // other exceptions.

    if (!__facet) {
        _RWSTD_MT_GUARD (_C_imp->_C_mutex);

        __facet = _C_make_facet (__id, __implicit, __cat, __maker);
    }

    _RWSTD_ASSERT (__facet);

    return *__facet;
}

// same as above but not guarded; called during initialization
// of the num_get<> and num_put<> facets (this is a hack)
inline const locale::facet&
locale::_C_unsafe_use_facet (const id &__id, bool __implicit, category __cat,
                             __facet_maker_func __maker) const
{
    _RWSTD_ASSERT (0 != _C_imp);

    const facet *__facet = _C_get_facet (__id);

    if (!__facet)
        __facet = _C_make_facet (__id, __implicit, __cat, __maker);

    _RWSTD_ASSERT (__facet);

    return *__facet;
}


_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)

// -------------------------------------------------------
// Implementation function template -- create_named_facet.
// -------------------------------------------------------
// 
// The default for facets with no derived byname version is to create a _Facet
// with classic ("C") behavior, ignoring the passed name.

template <class _Facet>
inline _Facet*
__rw_create_named_facet (_Facet*, const char *__name, _RWSTD_C::size_t __ref)
{
    _RWSTD_UNUSED (__name);

    // 22.1.1.1.2, p1: a facet class need not have a public copy
    // constructor, assignment, default constructor, destructor, etc.

    // this, of course, makes the assumption that a default
    // ctor is available
    _Facet *__facet = new _Facet ();

    __facet->_C_set_ref (__ref);

    return __facet;
}

// -----------------------------------------------------
// Implementation helper class -- __rw_use_c_lib_locale.
// -----------------------------------------------------

class _RWSTD_EXPORT __rw_use_c_lib_locale
{
    int _C_saved_cat;
    char *_C_previous_locale_name;
public:
    __rw_use_c_lib_locale (const char* = 0, int = LC_ALL);

    ~__rw_use_c_lib_locale ();

    operator bool () const {
        return _C_previous_locale_name != NULL;
    }
};


// ---------------------------------------
// Inline members of __rw_use_c_lib_locale
// ---------------------------------------

// Destructor restores the C library locale that was in effect when the object
// was constructed.
inline __rw_use_c_lib_locale::~__rw_use_c_lib_locale ()
{
    if (_C_previous_locale_name) {
        setlocale(_C_saved_cat, _C_previous_locale_name);

        delete[] _C_previous_locale_name;
    }
}


template <class _Facet>
inline _Facet*
__rw_create_native_facet (_Facet*)
{
    return __rw_create_named_facet ((_Facet*)0, "", _RWSTD_C::size_t (1));
}


_RWSTD_NAMESPACE_END   // __rw


// functions below are defined here as a hack to allow explicit instantiation
// with automatic template instantiation (and implicit file inclusion); needed
// to guarantee that definitions of all instantiated entities have been seen


_RWSTD_NAMESPACE_BEGIN (std)

// Template use_facet<>() returns a reference to a facet.  Its result is
// guaranteed by locale's value semantics to last at least as long as the
// locale or any copy of the locale it came from.

#ifndef _RWSTD_NO_TEMPLATE_ON_RETURN_TYPE
#  define _RWSTD_SELECT_FACET(ignore)
#else
#  define _RWSTD_SELECT_FACET(type) , type*
#endif


#ifndef _RWSTD_INLINE_INSTANTIATION_HACK

template <class _Facet>
inline const _Facet&
use_facet (const locale &__loc _RWSTD_SELECT_FACET (_Facet))
{
    const _Facet &__facet = _RWSTD_STATIC_CAST (const _Facet&,
                               __loc._C_use_facet (_Facet::id,
                                                   _Facet::_C_ok_implicit,
                                                   _Facet::_C_facet_cat,
                               _RW::__rw_facet_maker<_Facet>::_C_maker_func));

    return __facet;
}

#endif   // _RWSTD_INLINE_INSTANTIATION_HACK


template <class _Facet>
inline bool
has_facet (const locale &__loc _RWSTD_SELECT_FACET (_Facet)) _THROWS (())
{
    return _Facet::_C_ok_implicit || __loc._C_get_facet (_Facet::id);
}


#undef _RWSTD_SELECT_FACET


_RWSTD_NAMESPACE_END   // std


#endif   // _RWSTD_LOCALE_H_INCLUDED
