// -*- C++ -*-
/***************************************************************************
 *
 * _iosbase.h - Declarations for the Standard Library basic stream I/O
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _iosbase.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_IOSBASE_H_INCLUDED
#define _RWSTD_IOSBASE_H_INCLUDED

#include <iosfwd>

#include <rw/_locale.h>
#include <rw/_mutex.h>
#include <rw/_defs.h>
#include <rw/_traits.h>

//#include <fcntl.h> Rogue Wave Bug

//#ifndef _RWSTD_NO_EXCEPTIONS Rogue Wave Bug
#  include <rw/_exception.h>
//#endif // _RWSTD_NO_EXCEPTIONS Rogue Wave Bug


_RWSTD_NAMESPACE_BEGIN (std)


class _RWSTD_EXPORT ios_base: public _RW::__rw_synchronized
{
public:

    // 27.4.2.1.1
    class _RWSTD_EXPORT failure : public __rw_exception {
    public:
        _EXPLICIT failure (const string& __what):
            __rw_exception (__what) { }

        // extension
        _EXPLICIT failure (const char *__what = 0):
            __rw_exception (__what) { }
    };


#ifndef _RWSTD_NO_EXT_FAILURE

    // extensions
    class badbit_set : public failure { };

    class eofbit_set : public failure { };

    class failbit_set : public failure { };

#endif   // _RWSTD_NO_EXT_FAILURE

    // 27.4.2.1.2
    typedef int fmtflags;

    // insert and extract bool type in alphabetic format 
    _RWSTD_STATIC_CONST (fmtflags, boolalpha   = _RWSTD_IOS_BOOLALPHA);

    // converts integer input or generates integer output in decimal base
    _RWSTD_STATIC_CONST (fmtflags, dec         = _RWSTD_IOS_DEC);

    // generate floating-point output in fixed-point notation
    _RWSTD_STATIC_CONST (fmtflags, fixed       = _RWSTD_IOS_FIXED);

    // converts integer input or generates integer output in hexadecimal base
    _RWSTD_STATIC_CONST (fmtflags, hex         = _RWSTD_IOS_HEX);

    // adds fill characters at a designated internal point in certain generated
    // output, or identical to right if no such point is designated
    _RWSTD_STATIC_CONST (fmtflags, internal    = _RWSTD_IOS_INTERNAL);

    // adds fill characters on the right of certain generated output
    _RWSTD_STATIC_CONST (fmtflags, left        = _RWSTD_IOS_LEFT);

    // converts integer input or generates integer output in octal base
    _RWSTD_STATIC_CONST (fmtflags, oct         = _RWSTD_IOS_OCT);

    // adds fill characters on the left of certain generated output
    _RWSTD_STATIC_CONST (fmtflags, right       = _RWSTD_IOS_RIGHT);

    // generates floating-point output in scientific notation
    _RWSTD_STATIC_CONST (fmtflags, scientific  = _RWSTD_IOS_SCIENTIFIC);

    // generates a prefix indicating the numeric base of generated integer
    // output (bin - none, oct - "0", dec - none, hex - "0x")
    _RWSTD_STATIC_CONST (fmtflags, showbase    = _RWSTD_IOS_SHOWBASE);

    // generates a decimal-point character unconditionally in generated
    // floating-point output
    _RWSTD_STATIC_CONST (fmtflags, showpoint   = _RWSTD_IOS_SHOWPOINT);

    // generates a + sign in non-negative generated numeric output
    _RWSTD_STATIC_CONST (fmtflags, showpos     = _RWSTD_IOS_SHOWPOS);

    // skips leading white space before certain input operations
    _RWSTD_STATIC_CONST (fmtflags, skipws      = _RWSTD_IOS_SKIPWS);

    // flushes output after each output operation
    _RWSTD_STATIC_CONST (fmtflags, unitbuf     = _RWSTD_IOS_UNITBUF);

    // replaces certain lowercase letters with their uppercase equivalents
    // in generated output
    _RWSTD_STATIC_CONST (fmtflags, uppercase   = _RWSTD_IOS_UPPERCASE);

#ifndef _RWSTD_NO_EXT_BIN_IO

    // extension - converts integer input or generates integer output
    // in binary base
    _RWSTD_STATIC_CONST (fmtflags, bin         = _RWSTD_IOS_BIN);

#endif   // _RWSTD_NO_EXT_BIN_IO

    _RWSTD_STATIC_CONST (fmtflags, basefield   = _RWSTD_IOS_BASEFIELD);
    _RWSTD_STATIC_CONST (fmtflags, adjustfield = _RWSTD_IOS_ADJUSTFIELD);
    _RWSTD_STATIC_CONST (fmtflags, floatfield  = _RWSTD_IOS_FLOATFIELD);

#ifndef _RWSTD_NO_EXT_REENTRANT_IO

    // extension: never locks the object in MT environments
    _RWSTD_STATIC_CONST (fmtflags, nolock      = _RWSTD_IOS_NOLOCK);

    // extension: never locks stream buffer in MT environments
    _RWSTD_STATIC_CONST (fmtflags, nolockbuf   = _RWSTD_IOS_NOLOCKBUF);

#endif   // _RWSTD_NO_EXT_REENTRANT_IO

    // 27.4.2.1.3
    typedef int iostate;

    _RWSTD_STATIC_CONST (iostate, goodbit = _RWSTD_IOS_GOODBIT);

    // indicates a loss of integrity in an input or output sequence
    // (such as an irrecoverable read error from a file); 
    _RWSTD_STATIC_CONST (iostate, badbit  = _RWSTD_IOS_BADBIT);

    // indicates that an input operation reached the end of an input sequence
    _RWSTD_STATIC_CONST (iostate, eofbit  = _RWSTD_IOS_EOFBIT);

    // indicates that an input operation failed to read the expected
    // characters, or that an output operation failed to generate the
    // desired characters
    _RWSTD_STATIC_CONST (iostate, failbit = _RWSTD_IOS_FAILBIT);

    // 27.4.2.1.4
    typedef int openmode;

    // seek to end before each write 
    _RWSTD_STATIC_CONST (openmode, app    = _RWSTD_IOS_APP);

    // perform input and output in binary mode (as opposed to text mode)
    _RWSTD_STATIC_CONST (openmode, binary = _RWSTD_IOS_BINARY);

    // open for input
    _RWSTD_STATIC_CONST (openmode, in     = _RWSTD_IOS_IN);

    // open for output
    _RWSTD_STATIC_CONST (openmode, out    = _RWSTD_IOS_OUT);

    // truncate an existing stream when opening
    _RWSTD_STATIC_CONST (openmode, trunc  = _RWSTD_IOS_TRUNC);

    // open and seek to end immediately after opening 
    _RWSTD_STATIC_CONST (openmode, ate    = _RWSTD_IOS_ATE);

#ifndef _RWSTD_STRICT_ANSI

    // do not create a (file) stream if it doesn't exist
    _RWSTD_STATIC_CONST (openmode, nocreate    = _RWSTD_IOS_NOCREATE);

    // do not eplace an existing (file) stream
    _RWSTD_STATIC_CONST (openmode, noreplace   = _RWSTD_IOS_NOREPLACE);

#endif   // _RWSTD_STRICT_ANSI

    enum _C_seekdir {
        _C_beg = SEEK_SET,
        _C_cur = SEEK_CUR,
        _C_end = SEEK_END
    };

#ifndef _RWSTD_NO_STATIC_CONST_MEMBER_INIT

    typedef _C_seekdir seekdir;

#else

    typedef int seekdir;

#endif   // _RWSTD_NO_STATIC_CONST_MEMBER_INIT

    // 27.4.2.1.5, p1
    _RWSTD_STATIC_CONST (seekdir, beg = _C_beg);
    _RWSTD_STATIC_CONST (seekdir, cur = _C_cur);
    _RWSTD_STATIC_CONST (seekdir, end = _C_end);

#ifndef _RWSTD_NO_DEPRECATED    

    // D.6, p1
    typedef seekdir         seek_dir;
    typedef openmode        open_mode;
    typedef iostate         io_state;
    typedef _STD::streamoff streamoff;
    typedef _STD::streampos streampos;

#endif // _RWSTD_NO_DEPRECATED    
    
    
    // 27.4.2.1.6
    class _RWSTD_EXPORT Init {
        static int _C_initcnt;   // initialization counter
    public:
        Init ();

        ~Init ();
    };

    // 27.4.2.2, p1
    fmtflags flags () const {
        return _C_fmtfl;
    }

    // 27.4.2.2, p2
    fmtflags flags (fmtflags __f) {
        return _RWSTD_ATOMIC_IO_SWAP (_C_fmtfl, __f, _C_mutex);
    }

    // 27.4.2.2, p4
    fmtflags setf (fmtflags __f) {
        return flags (flags () | __f);
    }

    // 27.4.2.2, p6
    fmtflags setf (fmtflags __f, fmtflags __mask) {
        return flags (flags () & ~__mask | __f & __mask);
    }

    // 27.4.2.2, p8
    void unsetf (fmtflags __f) {
        flags (flags () & ~__f);
    }

    // 27.4.2.2, p9
    streamsize precision () const {
          return _C_prec;
    }

    // 27.4.2.2, p10
    streamsize precision (streamsize __p) {
        return _RWSTD_ATOMIC_IO_SWAP (_C_prec, __p, _C_mutex);
    }

    // 27.4.2.2, p12
    streamsize width () const {
        return _C_wide;
    }

    // 27.4.2.2, p13
    streamsize width (streamsize __w) {
        return _RWSTD_ATOMIC_IO_SWAP (_C_wide, __w, _C_mutex);
    }

    // 27.4.2.3, p1
    locale imbue (const locale&);

    // extension: avoids reference counting in MT builds (may result in
    // a speedup of up to 50%); this is an alternative to caching a reference
    // (pointer) to a facet in each stream and stream buffer object
    locale& getloc () {
        return _C_loc;
    }

    // 27.4.2.3, p4
    locale getloc () const {
        return _C_loc;
    }

    // 27.4.2.5, p1
    static int xalloc () {
        return _RWSTD_ATOMIC_PREINCREMENT (_C_index, false) - 1;
    }

    // 27.4.2.5, p2
    long&  iword (int);

    // 27.4.2.5, p4
    void*& pword (int);

    enum event { erase_event, imbue_event, copyfmt_event };

    typedef void (*event_callback)(event, ios_base&, int);

    // 27.5.2.6, p1
    void register_callback (event_callback, int);
    
    static bool _C_is_sync () {
        return _C_sync_with_stdio;
    }

    // 27.4.2.4, p1
    static bool sync_with_stdio (bool __sync = true) {
        // always atomic in MT environments since _C_sync is static
        return _RWSTD_ATOMIC_SWAP (_C_sync_with_stdio, __sync, false);
    }

    // 27.5.2.7, p2
    virtual ~ios_base ();    

    // returns a numeric base as per 22.2.2.1.2, p 4
    int _C_base () const {
        fmtflags __flags = flags () & basefield;
        return   oct == __flags ?  8
               : dec == __flags ? 10
               : hex == __flags ? 16

#ifndef _RWSTD_NO_EXT_BIN_IO

               : bin == __flags ?  2    // extension

#endif   // _RWSTD_NO_EXT_BIN_IO

               : 0;                     // anything else...
    }

protected:

    // will be reentrant if __reentrant is true, non-reentrant otherwise
    void _C_fire_event (event, bool __reentrant);

    // unsafe members - not synchronized in MT environments
    void _C_copyfmt (const ios_base&);

    // may throw (second argument is a bitmap of exceptions considered)
    // returns exceptions that would have been thrown had they not been masked
    iostate _C_unsafe_clear (iostate = goodbit, iostate = ~0);

    // called from basic_ios<>::imbue ()
    locale _C_unsafe_imbue (const locale&);

    // 27.5.2.7, p1
    ios_base ();

    fmtflags    _C_fmtfl;   // formatting flags
    streamsize  _C_prec;    // current precision
    streamsize  _C_wide;    // current width
    iostate     _C_state;   // stream state
    iostate     _C_except;  // active exceptions
    
private:

    ios_base (const ios_base&);            //not defined
    ios_base& operator= (const ios_base&); //not defined

    struct _C_event_cb {
        int             _C_index;
        event_callback  _C_fn;
    };

    locale       _C_loc;      // locale associated with stream

    long        *_C_iarray;   // user long data
    void*       *_C_parray;   // user void* data

    size_t       _C_isize;    // size of _C_iarray in elements
    size_t       _C_psize;    // size of _C_parray in elements

    _C_event_cb *_C_cbarray;  // array of user-registered callbacks
    size_t       _C_cbsize;   // size of _C_cbarray in elems

    static int   _C_index;    // used by xalloc to obtain a unique index
    static bool  _C_sync_with_stdio;
};


inline ios_base::ios_base ()
    : _C_iarray (0),
      _C_parray (0),
      _C_isize (0),
      _C_psize (0),
      _C_cbarray (0),
      _C_cbsize (0)
{
}


inline ios_base::~ios_base()
{
    // MT guard or reentrancy not necessary in a dtor
    _C_fire_event (erase_event, false /* not reentrant */);

    delete [] _C_iarray;
    delete [] _C_parray;
    delete [] _C_cbarray;
}


inline locale ios_base::_C_unsafe_imbue (const locale& __loc)
{
    locale __tmp = _C_loc;
    _C_loc = __loc;

    _C_fire_event (imbue_event, true /* reentrant */);

    return __tmp;  
}


inline locale ios_base::imbue (const locale& __loc)
{
    _RWSTD_MT_GUARD (flags () & _RWSTD_IOS_NOLOCK ? 0 : &_C_mutex);

    return _C_unsafe_imbue (__loc);
}


// 27.4.5.1 - fmtflags manipulators

// 27.4.5.1, p25
inline ios_base& unitbuf (ios_base& __strm)
{
    __strm.setf (ios_base::unitbuf);
    return __strm;
}


// 27.4.5.1, p27
inline ios_base& nounitbuf (ios_base& __strm)
{
    __strm.unsetf (ios_base::unitbuf);
    return __strm;
}


// 27.4.5.1, p1
inline ios_base& boolalpha (ios_base& __strm)
{
    __strm.setf (ios_base::boolalpha);
    return __strm;
}


// 27.4.5.1, p3
inline ios_base& noboolalpha (ios_base& __strm)
{
    __strm.unsetf (ios_base::boolalpha);
    return __strm;
}


// 27.4.5.1, p5
inline ios_base& showbase (ios_base& __strm)
{
    __strm.setf (ios_base::showbase);
    return __strm;
}


// 27.4.5.1, p7
inline ios_base& noshowbase (ios_base& __strm)
{
    __strm.unsetf (ios_base::showbase);
    return __strm;
}


// 27.4.5.1, p9
inline ios_base& showpoint (ios_base& __strm)
{ 
    __strm.setf (ios_base::showpoint);
    return __strm;
}


// 27.4.5.1, p11
inline ios_base& noshowpoint (ios_base& __strm)
{
    __strm.unsetf (ios_base::showpoint);
    return __strm;
}


// 27.4.5.1, p13
inline ios_base& showpos (ios_base& __strm)
{
    __strm.setf (ios_base::showpos);
    return __strm;
}


// 27.4.5.1, p15
inline ios_base& noshowpos (ios_base& __strm)
{
    __strm.unsetf (ios_base::showpos);
    return __strm;
}


// 27.4.5.1, p17
inline ios_base& skipws (ios_base& __strm)
{
    __strm.setf (ios_base::skipws);
    return __strm;
}


// 27.4.5.1, p19
inline ios_base& noskipws (ios_base& __strm)
{
    __strm.unsetf (ios_base::skipws);
    return __strm;
}


// 27.4.5.1, p21
inline ios_base& uppercase (ios_base& __strm)
{
    __strm.setf (ios_base::uppercase);
    return __strm;
}


// 27.4.5.1, p23
inline ios_base& nouppercase (ios_base& __strm)
{
    __strm.unsetf (ios_base::uppercase);
    return __strm;
}


// 27.4.5.2 - adjustfield manipulators

// 27.4.5.2, p1
inline ios_base& internal (ios_base& __strm)
{
    __strm.setf (ios_base::internal, ios_base::adjustfield);
    return __strm;
}


// 27.4.5.2, p3
inline ios_base& left (ios_base& __strm)
{
    __strm.setf (ios_base::left, ios_base::adjustfield);
    return __strm;
}


// 27.4.5.2, p5
inline ios_base& right (ios_base& __strm)
{
    __strm.setf (ios_base::right, ios_base::adjustfield);
    return __strm;
}


// 27.4.5.3 - basefield manipulators

// 27.4.5.3, p1
inline ios_base& dec (ios_base& __strm)
{
    __strm.setf (ios_base::dec, ios_base::basefield);
    return __strm;
}


// 27.4.5.3, p3
inline ios_base& hex (ios_base& __strm)
{
    __strm.setf (ios_base::hex, ios_base::basefield);
    return __strm;
}


// 27.4.5.3, p5
inline ios_base& oct (ios_base& __strm)
{
    __strm.setf (ios_base::oct, ios_base::basefield);
    return __strm;
}


#ifndef _RWSTD_NO_EXT_BIN_IO

// extension
inline ios_base& bin (ios_base& __strm)
{
    __strm.setf (ios_base::bin, ios_base::basefield);
    return __strm;
}

#endif   // _RWSTD_NO_EXT_BIN_IO


// 27.4.5.4 - floatfield manipulators

// 27.4.5.4, p1
inline ios_base& fixed (ios_base& __strm)
{
    __strm.setf (ios_base::fixed, ios_base::floatfield);
    return __strm;
}


// 27.4.5.4, p3
inline ios_base& scientific (ios_base& __strm)
{
    __strm.setf (ios_base::scientific, ios_base::floatfield);
    return __strm;
}


_RWSTD_NAMESPACE_END   // std


#endif   // _RWSTD_IOSBASE_H_INCLUDED
