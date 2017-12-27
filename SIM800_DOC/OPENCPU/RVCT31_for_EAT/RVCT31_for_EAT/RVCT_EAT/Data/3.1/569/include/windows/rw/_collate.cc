/***************************************************************************
 *
 * _collate.cc - Definitions for the Standard Library character collation
 *               facet
 *
 * $Id: _collate.cc 91768 2005-08-10 18:58:48Z drodgman $
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


_RWSTD_NAMESPACE_BEGIN (__rw)

template <class _CharT>
__rw_collate_table<_CharT>
__rw_collate_impl<_CharT>::_C_default_table = { 0, { 0, 0, 0, 0, 0, 0 } };

_RWSTD_NAMESPACE_END   // __rw


_RWSTD_NAMESPACE_BEGIN (std)

// --------------------------------------
// Facet collate<_CharT> member templates.
// --------------------------------------

template <class _CharT>
locale::id collate<_CharT>::id;

template <class _CharT>
int collate<_CharT>::
do_compare (const _CharT* __low1, const _CharT* __high1,
            const _CharT* __low2, const _CharT* __high2) const
{
    size_t __len1 = __high1 - __low1;
    size_t __len2 = __high2 - __low2;
    size_t __len  = __len1 < __len2 ? __len1 : __len2;

    for ( ; __len--; __low1++, __low2++) {

        int __comp =   this->_C_coll_order (*__low1)
                     - this->_C_coll_order (*__low2);
        
        if (__comp)
            return __comp < 0 ? -1 : 1;
    }

    return __len1 < __len2 ? -1 : __len2 < __len1 ? +1 : 0;
}


template <class _CharT>
_TYPENAME collate<_CharT>::string_type
collate<_CharT>::do_transform (const _CharT *__low, const _CharT *__high) const
{
    // use an extension: allocate unitialized string of (high - low) elements
    string_type __res ((_CharT*)0, __high - __low);

    _TYPENAME string_type::iterator __out = __res.begin ();

    for ( ; __low != __high; ++__low, ++__out)
        *__out = this->_C_coll_order (*__low);

    return __res;
}


template <class _CharT>
long collate<_CharT>::do_hash (const _CharT *__start, const _CharT *__end) const
{
    // We hash the result of do_transform, so that keys that transform equally
    // will hash equally, as per 22.2.4.1.2, p3.

    string_type __s = do_transform (__start, __end);
    __start         = __s.data ();
    __end           = __start + __s.length ();

    // Peter Weinberger's generic hashing algorithm, adapted by Andrew Binstock
    // from a version by Allen Holub (see Andrew Binstock, "Hashing Revisited",
    // Dr. Dobb's Journal, April 1996) and templatized by Rogue Wave.

    const int  __long_bits     = CHAR_BIT * sizeof (long);
    const int  __one_eighth    = __long_bits / 8;
    const int  __three_fourths = __long_bits * 3 / 4;
    const long __high_bits     = ~0L << __long_bits - __one_eighth;

    long __res = 0;

    for ( ; __start != __end; ++__start) {
        __res = (__res << __one_eighth) + *__start;

        long __tmp = __res & __high_bits;
        if (__tmp)
            __res = (__res ^ (__tmp >> __three_fourths)) & ~__high_bits;
    }

    return __res;
}


_RWSTD_NAMESPACE_END   // std
