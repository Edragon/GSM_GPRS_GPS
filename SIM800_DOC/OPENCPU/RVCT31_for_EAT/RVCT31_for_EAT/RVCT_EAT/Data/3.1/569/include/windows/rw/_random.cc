/***************************************************************************
 *
 * _random.cc - member function definitions for template class
 *              __rw_random_generator
 *
 * $Id: _random.cc 91768 2005-08-10 18:58:48Z drodgman $
 *
 ***************************************************************************
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
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


template <unsigned _SIZE>
void __rw_random_generator<_SIZE>::seed (unsigned long __seed)
{
    unsigned long __k = 1;

    _C_table [_C_SIZE - 1] = __seed;

    _RWSTD_C::size_t __i;

    for (__i = 0; __i != _C_SIZE - 1; ++__i) {
        _RWSTD_C::size_t __ii = 21 * __i % _C_SIZE;
        _C_table [__ii] = __k;
        __k = __seed - __k;
        __seed = _C_table [__ii];
    }

    for (__i = 0; __i != 4; ++__i) {
        for (_RWSTD_C::size_t __ii = 0; __ii != _C_SIZE; ++__ii)
            _C_table [__i] =
                _C_table [__i] - _C_table [(__i + 31) % _C_SIZE];
    }

    _C_index1 = 0U;
    _C_index2 = _C_SIZE * 2UL / 3UL - 1UL;
}


_RWSTD_NAMESPACE_END   // __rw
