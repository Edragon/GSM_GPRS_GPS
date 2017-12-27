/***************************************************************************
 *
 * _ioiter.h - Definitions of stream iterator templates
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _ioiter.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_IOITER_H_INCLUDED
#define _RWSTD_IOITER_H_INCLUDED

#include <iosfwd>
#include <rw/_iterbase.h>
#include <rw/_defs.h>


_RWSTD_NAMESPACE_BEGIN (std)


template <class _TypeT, class _CharT, class _Traits, class _Distance>
class istream_iterator;

template <class _TypeT, class _CharT, class _Traits, class _Distance>
bool operator== (const istream_iterator<_TypeT, _CharT, _Traits, _Distance>&,
                 const istream_iterator<_TypeT, _CharT, _Traits, _Distance>&);

// 24.5.1
#if !defined (_MSC_VER) || _MSC_VER > 1300

template <class _TypeT,
          class _CharT _RWSTD_SIMPLE_DEFAULT (char),
          class _Traits _RWSTD_COMPLEX_DEFAULT (char_traits<_CharT>),
          class _Distance _RWSTD_COMPLEX_DEFAULT (ptrdiff_t)>
#else

// prevent MSVC 6.0 ICE
template <class _TypeT, class _CharT, class _Traits, class _Distance>

#endif

class istream_iterator
    : public iterator<input_iterator_tag, _TypeT, _Distance,
                      const _TypeT*, const _TypeT&>
{
    friend bool _RWSTD_SPECIALIZED_FRIEND (operator==)
        (const istream_iterator&, const istream_iterator&);

    typedef iterator<input_iterator_tag, _TypeT, _Distance,
                     const _TypeT*, const _TypeT&> _C_base;

public: 

    typedef _CharT                                char_type;
    typedef _Traits                               traits_type;
    typedef basic_istream<char_type, traits_type> istream_type;

    // for convenience
    typedef _TYPENAME _C_base::value_type         value_type;

    // 24.5.1.1, p1
    istream_iterator (): _C_strm (0) { }

    // 24.5.1.1, p2
    istream_iterator (istream_type& __s)
        : _C_strm (&__s) {
        ++*this;
    }

    // 24.5.1.2, p1
    const value_type& operator* () const {
        return _C_val;
    }

    // 24.5.1.2, p2
    _RWSTD_OPERATOR_ARROW (const value_type* operator->() const);

    // 24.5.1.2, p3
    istream_iterator& operator++ () {
        return _C_strm && !!*_C_strm && (*_C_strm >> _C_val), *this;
    }

    // 24.5.1.2, p5
    istream_iterator operator++ (int) {
        istream_iterator __tmp = *this;
        return ++*this, __tmp;
    }
      
protected:

    istream_type *_C_strm;   // associated stream
    value_type    _C_val;    // last extracted value
};


// 24.5.1.2, p6
template <class _TypeT, class _CharT, class _Traits, class _Distance>
inline bool
operator== (const istream_iterator<_TypeT, _CharT, _Traits, _Distance>& __x,
            const istream_iterator<_TypeT, _CharT, _Traits, _Distance>& __y)
{
    return (__x._C_strm && !!*__x._C_strm) == (__y._C_strm && !!*__y._C_strm);
}


template <class _TypeT, class _CharT, class _Traits, class _Distance>
inline bool
operator!= (const istream_iterator<_TypeT, _CharT, _Traits, _Distance>& __x,
            const istream_iterator<_TypeT, _CharT, _Traits, _Distance>& __y)
{
    return !(__x == __y);
}




// 24.5.2
template <class _TypeT, 
          class _CharT _RWSTD_SIMPLE_DEFAULT (char), 
          class _Traits _RWSTD_COMPLEX_DEFAULT (char_traits<_CharT>) >
class ostream_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
{
public:
    typedef _CharT                                char_type;
    typedef _Traits                               traits_type;
    typedef basic_ostream<char_type, traits_type> ostream_type;

    // 24.5.2.1, p1
    ostream_iterator (ostream_type& __strm) 
      : _C_strm (&__strm), _C_str (0) { }

    // 24.5.2.1, p2
    ostream_iterator (ostream_type& __strm, const char_type* __s) 
      : _C_strm (&__strm), _C_str (__s) { }

    // 24.5.2.2, p1
    ostream_iterator& operator= (const _TypeT &__val) { 
        *_C_strm << __val;
        if (_C_str)
            *_C_strm << _C_str;
        return *this;
    }

    // 24.5.2.2, p2
    ostream_iterator& operator* () {
        return *this;
    }

    // 24.5.2.2, p3
    ostream_iterator& operator++ () {
        return *this;
    }
 
    ostream_iterator& operator++ (int) {
        return *this;
    }

private:

    ostream_type*    _C_strm;   // associated stream
    const char_type* _C_str;    // string to separate values with
};


// 24.5.3
template<class _CharT, class _Traits>
class istreambuf_iterator
    : public iterator<input_iterator_tag, _CharT, 
                      _TYPENAME _Traits::off_type, _CharT*, _CharT&>
{
public:

    typedef _CharT                                  char_type;
    typedef _Traits                                 traits_type;
    typedef _TYPENAME traits_type::int_type         int_type;
    typedef basic_streambuf<char_type, traits_type> streambuf_type;
    typedef basic_istream<char_type, traits_type>   istream_type;

    // 24.5.3.1
    class proxy {
        char_type       _C_keep;
        streambuf_type *_C_sb;

        proxy (char_type __c, streambuf_type *__sb)
            : _C_keep (__c), _C_sb (__sb) { }

    public:
        
        char_type operator* () const {
            return _C_keep;
        }

        friend class istreambuf_iterator<char_type, traits_type>;
    };

public:

    // 24.5.3.2, p1
    istreambuf_iterator (streambuf_type *__sb = 0) _THROWS (())
    : _C_sb (__sb) { }

    // 24.5.3.2, p2
    istreambuf_iterator (istream_type &__strm) _THROWS (())
    : _C_sb (__strm.rdbuf ()) { } 

    // 24.5.3.2, p3
    istreambuf_iterator (const proxy &__proxy) _THROWS (())
    : _C_sb (__proxy._C_sb) { }

    // 24.5.3.3, p1
    char_type operator*() const {
        return traits_type::to_char_type (_C_sb ? _C_sb->sgetc ()
                                          : traits_type::eof ()) ;
    }

    // 24.5.3.4, p1
    istreambuf_iterator& operator++ () {
        if (_C_sb)
            _C_sb->sbumpc ();
        return *this;
    }

    // 24.5.3.4, p3
    proxy operator++ (int);

    // 24.5.3.5, p1 - const follows the accepted resolution of DR 110
    bool equal (const istreambuf_iterator& __rhs) const {
        return    (_C_sb ? _C_sb->sgetc () : traits_type::eof ())
               == (__rhs._C_sb ? __rhs._C_sb->sgetc () : traits_type::eof ());
    }

private:
    streambuf_type *_C_sb;
};


template<class _CharT, class _Traits>
inline _TYPENAME istreambuf_iterator<_CharT, _Traits>::proxy 
istreambuf_iterator<_CharT, _Traits>::operator++(int)
{
    int_type __c = _C_sb ? _C_sb->sgetc () : traits_type::eof ();

    if (!traits_type::eq_int_type (__c, traits_type::eof ()))
        _C_sb->sbumpc ();

    return proxy (traits_type::to_char_type (__c), _C_sb);
}


// 24.5.3.6
template<class _CharT, class _Traits>
inline bool
operator== (const istreambuf_iterator<_CharT, _Traits>& __x,
            const istreambuf_iterator<_CharT, _Traits>& __y)
{
    return __x.equal (__y);
}


// 24.5.3.7
template<class _CharT, class _Traits>
inline bool
operator!= (const istreambuf_iterator<_CharT, _Traits>& __x, 
            const istreambuf_iterator<_CharT, _Traits>& __y)
{
    return !(__x == __y);
}


// 24.5.4
template<class _CharT, class _Traits>
class ostreambuf_iterator 
    : public iterator<output_iterator_tag, void, void, void, void>
{
public:

    typedef _CharT                                  char_type;
    typedef _Traits                                 traits_type;
    typedef basic_streambuf<char_type, traits_type> streambuf_type;
    typedef basic_ostream<char_type, traits_type>   ostream_type;

    // 24.5.4.1, p1
    ostreambuf_iterator (ostream_type &__strm) _THROWS (())
    : _C_sb (__strm.rdbuf ()) { }

    // 24.5.4.1, p2
    ostreambuf_iterator (streambuf_type *__sb) _THROWS (())
    : _C_sb (__sb) {
        _RWSTD_ASSERT (__sb != 0);
    }

    // 24.5.4.2, p3
    ostreambuf_iterator& operator* () {
        return *this;
    }

    // 24.5.4.2, p4
    ostreambuf_iterator& operator++ () {
        return *this;
    }

    // 24.5.4.2, p4
    ostreambuf_iterator& operator++ (int) {
        return *this;
    }

    // 24.5.4.2, p1
    ostreambuf_iterator& operator= (char_type __c) { 
        if (!failed () && traits_type::eq_int_type (_C_sb->sputc (__c),
                                                    traits_type::eof ()))
            _C_sb = 0;
        return *this;
    }

    // 24.5.4.2, p5
    bool failed () const _THROWS (()) {
        return !_C_sb;
    }

private:
    streambuf_type *_C_sb;
};


_RWSTD_NAMESPACE_END   // std


#endif   // _RWSTD_IOITER_H_INCLUDED
