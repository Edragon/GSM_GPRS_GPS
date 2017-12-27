/***************************************************************************
 *
 * _collate.h - Definitions of the character collation facets
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _collate.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_COLLATE_H_INCLUDED
#define _RWSTD_COLLATE_H_INCLUDED

#include <rw/_locale.h>

_RWSTD_NAMESPACE_BEGIN (__rw)


// -----------------------------------------------------------
// Implementation class template -- __rw_collate_table<_CharT>.
// -----------------------------------------------------------

// Data structure that drives the collate<_CharT> process.  It contains a
// binary tree of nodes, each of which defines the collation treatment to be
// applied to a range of characters.

template <class _CharT>
struct __rw_collate_table
{
  struct _C_node {
    _C_node      *_C_left;   // Node for _CharT values that are too small
    _C_node      *_C_right;  // (resp. too large) for this node
    const _CharT *_C_table;  // Translate table or NULL
    _CharT        _C_min_c,
                  _C_max_c;  // Range of _CharT-s covered by this node
    _CharT        _C_offset; // Amount to add to _CharT if _C_table is NULL
  };

  long            _C_length; // Length of memory block containing all the nodes
  _C_node         _C_root;   // Root node of _C_table
};

// ----------------------------------------------------------
// Implementation class template -- __rw_collate_data<_CharT>.
// ----------------------------------------------------------

// collate<_CharT> derives from this (via __rw::__rw_collate_impl) to get its
// private data members.

template <class _CharT>
class __rw_collate_data
{
public:
    typedef __rw_collate_table<_CharT> table_type;
    typedef _TYPENAME table_type::_C_node node_type;

    const table_type *__table;

    __rw_collate_data (const table_type &__t): __table(&__t) { }
    _CharT _C_coll_order (_CharT) const;
};


template <class _CharT>
inline _CharT __rw_collate_data<_CharT>::_C_coll_order (_CharT __c) const
{
    const node_type *__t = &__table->_C_root;

    while (__t)
        if (__c < __t->_C_min_c)
            __t = __t->_C_left;
        else if (__c > __t->_C_max_c)
            __t = __t->_C_right;
        else {
            if (__t->_C_table)
                __c = __t->_C_table[size_t(__c-__t->_C_min_c)];
            else
                __c += _CharT (__t->_C_offset);
          break;
      }

    return __c;
}

// ----------------------------------------------------------
// Implementation class template -- __rw_collate_impl<_CharT>.
// ----------------------------------------------------------

// Facet collate<_CharT> derives from this to get the part of its behavior
// that is specialized for char and wchar_t.

template <class _CharT>
class __rw_collate_impl: public __rw_collate_data<_CharT>
{
    static __rw_collate_table<_CharT> _C_default_table;
public:
    __rw_collate_impl (void)
        : __rw_collate_data<_CharT>(_C_default_table) { }
};

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_collate_impl<char>);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_collate_impl<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_T


_RWSTD_NAMESPACE_END   // __rw


_RWSTD_NAMESPACE_BEGIN (std)


// -----------------------------------------------------
// Standard character collation facet -- collate<_CharT>.
// -----------------------------------------------------

template <class _CharT>
class collate
    : public locale::facet,
      public _RW::__rw_collate_impl<_CharT>
{
public:
    typedef _CharT char_type;
    typedef basic_string<char_type, char_traits<char_type>,
                         allocator<char_type> >
    string_type;

    _EXPLICIT collate (size_t __refs = 0)
        : locale::facet (__refs, locale::collate) { }

    int compare (const char_type* __low1, const char_type* __high1,
                 const char_type* __low2, const char_type* __high2) const {
        return do_compare(__low1, __high1, __low2, __high2);
    }

    string_type
    transform (const char_type* __low, const char_type* __high) const {
        return do_transform(__low, __high);
    }

    long hash (const char_type* __low, const char_type* __high) const {
        return do_hash(__low, __high);
    }

    static locale::id id;

    // Implementation:
    enum { _C_facet_cat = locale::collate, _C_ok_implicit = 1 };

protected:

    virtual int
    do_compare (const char_type*, const char_type*,
                const char_type*, const char_type*) const;

    virtual string_type
    do_transform (const char_type*, const char_type*) const;

    virtual long do_hash (const char_type*, const char_type*) const;

private:
    
#ifdef _RWSTD_NO_MEMBER_TEMPLATES
    locale::id &_C_get_id (void) const {
        return id;
    }
#endif
};


_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT collate<char>);

#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT collate<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_T


// ------------------------------------------------------
// Standard facet specialization -- collate_byname<char>.
// ------------------------------------------------------

template <class _CharT>
class collate_byname;

_RWSTD_SPECIALIZED_CLASS
class _RWSTD_EXPORT collate_byname<char>: public collate<char>
{
public:

    _EXPLICIT collate_byname (const char*, size_t = 0) { }

protected:

    virtual int
    do_compare (const char*, const char*, const char*, const char*) const;

    virtual string do_transform (const char*, const char*) const;

private:
    string _C_collate_name;
};

// ------------------------------------------------
// Standard derived facet -- collate_byname<_CharT>.
// ------------------------------------------------

template <class _CharT>
class collate_byname: public collate<_CharT>
{
public:
    typedef _CharT char_type;
    typedef basic_string<char_type,char_traits<char_type>,
                        allocator<char_type> >
    string_type;

    _EXPLICIT collate_byname (const char*, size_t = 0) { }

protected:

    virtual int do_compare (const char_type*, const char_type*,
                            const char_type*, const char_type*) const {
        return 0;
    }

    virtual string_type
    do_transform (const char_type*, const char_type*) const {
        return string_type ();
    }
};


#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT collate_byname<wchar_t>);

#endif   // _RWSTD_NO_WCHAR_T


_RWSTD_NAMESPACE_END   // std


_RWSTD_NAMESPACE_BEGIN (__rw)

_USING (_RWSTD_C::size_t);

#ifndef _RWSTD_NO_FUNC_PARTIAL_SPEC
    
template <class __CharT>
inline _STD::collate<__CharT>*
__rw_create_named_facet (collate<__CharT>*, const char *__name, size_t __ref)
{
    return new _STD::collate_byname<__CharT>(__name, __ref);
}

#else   // if defined (_RWSTD_NO_FUNC_PARTIAL_SPEC)

_RWSTD_SPECIALIZED_FUNCTION
inline _STD::collate<char>*
__rw_create_named_facet (_STD::collate<char>*, const char *__name, size_t __ref)
{
    return new _STD::collate_byname<char>(__name, __ref);
}


#ifndef _RWSTD_NO_WCHAR_T

_RWSTD_SPECIALIZED_FUNCTION
inline _STD::collate<wchar_t>*
__rw_create_named_facet (_STD::collate<wchar_t>*,
                         const char *__name, size_t __ref)
{
    return new _STD::collate_byname<wchar_t>(__name, __ref);
}

#endif   // _RWSTD_NO_WCHAR_T

#endif   // _RWSTD_NO_FUNC_PARTIAL_SPEC


_RWSTD_NAMESPACE_END   // __rw


#if _RWSTD_DEFINE_TEMPLATE (COLLATE)
#  include <rw/_collate.cc>
#endif


#endif   // _RWSTD_COLLATE_H_INCLUDED
