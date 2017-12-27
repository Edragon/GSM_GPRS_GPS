/***************************************************************************
 *
 * _file.h - Wrapper definitions for platform independent file I/O
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _file.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_FILE_H_INCLUDED
#define _RWSTD_FILE_H_INCLUDED

#include <rw/_defs.h>
#include _RWSTD_CSTDIO

#if (defined (_WIN32) || defined (_WIN64)) && !defined (__CYGWIN__)

#  include <io.h>

#  ifndef STDIN_FILENO
#    define STDIN_FILENO  0
#    define STDOUT_FILENO 1
#    define STDERR_FILENO 2
#  endif   // STDIN_FILENO

#else
#ifndef _RWSTD_NO_FILENO
#    include <unistd.h>
#else 
#    include <rw/_file_support.h>
#endif //_RWSTD_NO_FILENO
#endif   // _WIN32 || _WIN64

#ifndef _RWSTD_NO_FILENO
#include <fcntl.h>
#endif //_RWSTD_NO_FILENO

#if defined (_WIN32) || defined (_WIN64)
#  define _BINARY _O_BINARY
#else 
#  define _BINARY 0
#endif 


#if defined (__sun__) || defined (__sun) || defined (sun)

// fileno isn't available (e.g., for strict ANSI C conformance)
// this may result in a (hopefully) benign redeclaration
extern "C" int fileno (FILE*);

#endif   // __sun__

_RWSTD_NAMESPACE_BEGIN (__rw)

_USING (namespace std);


// __rw_file_t implements a file abstraction used by std::basic_filebuf<>
// the size of the object is guaranteed to be the same regardless
// of whether FILE* or int (for file descriptor) is used - switching
// between one and the other (for efficiency) is controlled via the
// macro _RWSTD_NO_NATIVE_IO

union _RWSTD_EXPORT __rw_file_t
{
private:

    static bool _C_is_open (FILE *__fp) {
        return  !(__fp == (FILE*)_RWSTD_INVALID_FILE_PTR);
    }

    static int _C_fileno (FILE *__fp) {
        return _C_is_open (__fp)
            ? fileno(__fp)
            : _RWSTD_INVALID_FILE_DESC;
    }

public:
    static int _C_get_mode (int);

    static int _C_get_mode (FILE *__fp);

#ifndef _RWSTD_NO_NATIVE_IO

    __rw_file_t ( ) : _C_fdsc (_RWSTD_INVALID_FILE_DESC)
        { }

    __rw_file_t (FILE *__fp) : _C_fdsc (_C_fileno (__fp))
        { }
    __rw_file_t (int __fd) : _C_fdsc (__fd)
        { }
    __rw_file_t (const __rw_file_t &__f) : _C_fdsc (__f._C_fdsc)
        { }

    __rw_file_t & operator= (const __rw_file_t &__f) {
        _C_fdsc = __f._C_fdsc;
        return *this;
    }
    
    int _C_get_mode () const;

    int _C_get_mode_arg (int) const;

    int _C_get_fd () const {
        return _C_fdsc;
    }

    bool _C_is_open () const {
        return  _C_fdsc != _RWSTD_INVALID_FILE_DESC;
    }

    bool _C_is_stdin () const {
        return STDIN_FILENO == _C_fdsc;
    }

    bool _C_is_stdout () const {
        return STDOUT_FILENO == _C_fdsc;
    }

    bool _C_is_stderr () const {
        return STDERR_FILENO == _C_fdsc;
    }

    bool _C_is_std () const {
        return _C_is_stdin () || _C_is_stdout () || _C_is_stderr ();
    }

    __rw_file_t& _C_open (const char *__name, int __mode, long __prot);

    bool _C_close () {
        return 0 == close (_C_fdsc);
    }

    size_t _C_read (void *__buf, size_t __size, size_t __count) {
        _RWSTD_SSIZE_T __nbytes = read (_C_fdsc, __buf, __size * __count);
        return __nbytes < 0 ? 0 : _RWSTD_STATIC_CAST (size_t, __nbytes);
    }

    bool _C_write (const void* __buf, size_t __size, size_t __count) {
        return (    (size_t)write (_C_fdsc, __buf, __size * __count)
                 == __size * __count);
    }

    long _C_seek (long __offset, int __origin) {
        return lseek (_C_fdsc, __offset, __origin);
    }

    // For non-native i/o facilitation only.
    int _C_flush ( ) {
        return 0;
    }

#else // if defined (_RWSTD_NO_NATIVE_IO)
    
    const char* _C_get_mode_arg (int) const;

    __rw_file_t ( ) : _C_fptr (0)
        { }

    __rw_file_t (FILE *__fp) : _C_fptr (__fp)
        { }

    __rw_file_t (int);

    __rw_file_t (const __rw_file_t &__f) : _C_fptr (__f._C_fptr)
        { }

    __rw_file_t& operator= (const __rw_file_t &__f) {
        _C_fptr = __f._C_fptr;
        return *this;
    }

    int _C_get_mode () const;

    int _C_get_fd () const {
        return _C_fileno (_C_fptr);
    }

    bool _C_is_open () const {
        return  !!_C_fptr;
    }

    bool _C_is_stdin () const {
        return STDOUT_FILENO ==_C_fileno (_C_fptr);
    }

    bool _C_is_stdout () const {
        return STDOUT_FILENO ==_C_fileno (_C_fptr);
    }

    bool _C_is_stderr () const {
        return STDERR_FILENO ==_C_fileno (_C_fptr);
    }
    
    bool _C_is_std () const {
        return _C_is_stdin () || _C_is_stdout () || _C_is_stderr ();
    }

    __rw_file_t &
    _C_open (const char *__name, int __mode, long);

    bool _C_close () {
        return !_C_is_open () ? true : 0 == fclose (_C_fptr);
    }

    size_t _C_read (void *__buf, size_t __size, size_t __count) {
#define __ARM_USE_FREAD_BYTES_AVAIL
#ifdef __ARM_USE_FREAD_BYTES_AVAIL
        size_t __bytes_read = __fread_bytes_avail(__buf, __size * __count, _C_fptr);
        /* fread can set a sticky EOF, clear it if there has been no error */
        if (!ferror(_C_fptr)) clearerr(_C_fptr);
        return __bytes_read;
#else
        return __size * fread (__buf, __size, __count, _C_fptr);
#endif
    }

    bool _C_write (const void* __buf, size_t __size, size_t __count) {
        return fwrite (__buf, __size, __count, _C_fptr) == __count;
    }

    long _C_seek (long __offset, int __origin) {
        int __rc = fseek (_C_fptr, __offset, __origin);
        if (__rc != 0)
            return -1;
        return ftell (_C_fptr);
    }

    int _C_flush () {
        return _C_is_open () ? fflush(_C_fptr) : EOF;
    }

#endif   //_RWSTD_NO_NATIVE_IO

private:

    FILE *_C_fptr;   // file pointer - used if _RWSTD_NO_NATIVE_IO is undef'd
    int   _C_fdsc;   // file descriptor - used otherwise (default)
};



inline int __rw_file_t::_C_get_mode (int __fd)
{
// FIXME -- need to have equivalent of fcntl() on win32.
#if defined (_RWSTD_NO_NATIVE_IO) 
    return __fd == STDIN_FILENO
               ? _RWSTD_IOS_IN
               : __fd == STDOUT_FILENO || __fd == STDERR_FILENO
                   ? _RWSTD_IOS_OUT : _RWSTD_IOS_OUT | _RWSTD_IOS_IN;
#else    
   int __m = fcntl (__fd, F_GETFL);

   if (__m == -1)
       return _RWSTD_INVALID_OPENMODE;

   int __mode = __m & O_APPEND ? _RWSTD_IOS_APP : 0;

   switch (__m & O_ACCMODE) {
       case O_RDONLY: __mode |= _RWSTD_IOS_IN; break;
       case O_WRONLY: __mode |= _RWSTD_IOS_OUT; break;
       case O_RDWR:   __mode |= _RWSTD_IOS_IN | _RWSTD_IOS_OUT; break;
   }

   return __mode;
#endif
}


inline int __rw_file_t::_C_get_mode (FILE *__fp)
{
    return _C_get_mode (_C_fileno (__fp));
}


#ifndef _RWSTD_NO_NATIVE_IO

#ifdef _INLINE_WITH_STATICS

_INLINE_WITH_STATICS int
__rw_file_t::_C_get_mode_arg (int __index) const
{
#if    !(defined (__osf__) && defined (__GNUG__) \
    && __GNUG__ == 2 && __GNUG_MINOR__ < 96)
    // static not used to prevent a g++ 2.95.2 warning on Tru64
    // sorry: semantics of inline function static data are wrong
    // (you'll wind up with multiple copies)
    static
#endif
    const int __modetbl [] = {
        -1,                                        // no flags error
        O_WRONLY | O_APPEND | O_CREAT,             // app
        -1 | _BINARY,                              // binary
        O_WRONLY | _BINARY |  O_APPEND | O_CREAT,  // binary|app
        O_RDONLY,                                  // in
        O_RDONLY | O_APPEND,                       // in|app
        O_RDONLY | _BINARY,                        // in|binary
        O_RDONLY | _BINARY | O_APPEND | O_CREAT,   // in|binary|app
        O_TRUNC  | O_WRONLY | O_CREAT,             // out
        O_WRONLY | O_APPEND | O_CREAT,             // out|app
        O_TRUNC | O_WRONLY | O_CREAT | _BINARY,    // out|binary
        O_WRONLY | _BINARY | O_APPEND | O_CREAT,   // out|binary|app
        O_RDWR,                                    // out|in
        O_RDWR | O_APPEND | O_CREAT,               // out|in|app
        O_RDWR | _BINARY,                          // out|in|binary
        O_RDWR | _BINARY  | O_APPEND | O_CREAT,    // out|in|binary|app
        O_TRUNC,                                   // trunc
        O_TRUNC | O_APPEND,                        // trunc|app
        O_TRUNC | _BINARY,                         // trunc|binary
        O_TRUNC | O_APPEND | _BINARY,              // trunc|binary|app
        O_TRUNC | O_RDONLY,                        // trunc|in
        O_TRUNC | O_RDONLY | O_APPEND,             // trunc|in|app
        O_TRUNC | O_RDONLY | _BINARY,              // trunc|in|binary
        O_TRUNC | O_RDONLY | _BINARY  | O_APPEND,  // trunc|in|binary|app
        O_TRUNC | O_WRONLY | O_CREAT,              // trunc|out
        O_TRUNC | O_WRONLY | O_APPEND | O_CREAT,   // trunc|out|app
        O_TRUNC | O_WRONLY | _BINARY  | O_CREAT,   // trunc|out|binary
        O_TRUNC | O_WRONLY | _BINARY  | O_APPEND| O_CREAT,// trunc|out|binary|app
        O_TRUNC | O_RDWR | O_CREAT,                // trunc|out|in
        O_TRUNC | O_RDWR | O_APPEND | O_CREAT,     // trunc|out|in|app
        O_TRUNC | O_RDWR | O_CREAT  | _BINARY,     // trunc|out|in|binary
        O_TRUNC | O_RDWR | _BINARY  | O_APPEND | O_CREAT  // trunc|out|in|binary|app
    };

    if (   0 > __index
        || int (sizeof __modetbl / sizeof *__modetbl) <= __index)
        return _RWSTD_INVALID_OPENMODE;

    return __modetbl[__index];
}

#endif   // _INLINE_WITH_STATICS


inline __rw_file_t&
__rw_file_t::_C_open (const char *__name, int __mode, long __prot)
{
    int __m = _C_get_mode_arg (  __mode
                               & ~(_RWSTD_IOS_NOCREATE | _RWSTD_IOS_NOREPLACE));
        
    if (__m == _RWSTD_INVALID_OPENMODE){
      _C_fdsc = _RWSTD_INVALID_FILE_DESC;
      return *this;
    }
    
    if (__mode & _RWSTD_IOS_OUT) {
        if (__mode & _RWSTD_IOS_NOCREATE)
            __m &= ~O_CREAT;
        if (__mode & _RWSTD_IOS_NOREPLACE)
            __m |= O_EXCL;
    }

    _C_fdsc = open (__name, __m, __prot); 

    return *this;
}


inline int __rw_file_t::_C_get_mode () const
{
    return _C_get_mode (_C_fdsc);
}


#else   // if defined (_RWSTD_NO_NATIVE_IO)

inline const char* __rw_file_t::_C_get_mode_arg (int __index) const
{
#if    !(defined (__osf__) && defined (__GNUG__) \
    && __GNUG__ == 2 && __GNUG_MINOR__ < 96)
    // static not used to prevent a g++ 2.95.2 warning on Tru64
    // sorry: semantics of inline function static data are wrong
    // (you'll wind up with multiple copies)
    static
#endif
    const char __modetbl [][4] = {
        "",             // no flags error            0
        "a",            // app                       1
        "",             // binary                    2
        "ab",           // binary|app                3
        "r",            // in                        4
        "a",            // in|app                    5
        "rb",           // in|binary                 6
        "ab",           // in|binary|app             7
        "w",            // out                       8
        "a",            // out|app                   9
        "wb",           // out|binary                10
        "ab",           // out|binary|app            11
        "r+",           // out|in                    12
        "a+",           // out|in|app                13
        "r+b",          // out|in|binary             14
        "a+b",          // out|in|binary|app         15
        "w",            // trunc                     16
        "w",            // trunc|app                 17
        "wb",           // trunc|binary              18
        "wb",           // trunc|binary|app          19
        "",             // trunc|in                  20
        "",             // trunc|in|app              21
        "",             // trunc|in|binary           22
        "",             // trunc|in|binary|app       23
        "w",            // trunc|out                 24
        "w",            // trunc|out|app             25
        "wb",           // trunc|out|binary          26
        "wb",           // trunc|out|binary|app      27  
        "w+",           // trunc|out|in              28
        "w+",           // trunc|out|in|app          29
        "w+b",          // trunc|out|in|binary       30
        "w+b"           // trunc|out|in|binary|app   31
    };

    if (  0 > __index
       || int (sizeof __modetbl / sizeof *__modetbl) <= __index)
        return 0;

    return __modetbl[__index];
}

#ifndef _RWSTD_NO_FILENO
inline __rw_file_t::__rw_file_t (int __fd)
{
    const char *__mode = _C_get_mode_arg (_C_get_mode (__fd));
    _C_fptr = __mode && __fd > -1 ? fdopen (__fd, __mode) : 0;
}
#else
inline __rw_file_t::__rw_file_t (int __fd)
{
    _C_fptr = (__fd == STDIN_FILENO) ? &std::__stdin :
            ((__fd == STDOUT_FILENO) ? &std::__stdout :
            ((__fd == STDERR_FILENO) ? &std::__stderr : 0));
    if (_C_fptr == 0)
        _RW::__rw_throw (_RWSTD_ERROR_RUNTIME_ERROR, "possix _C_get_mode not supported");
}
#endif //_RWSTD_NO_FILENO

inline __rw_file_t& __rw_file_t::_C_open (const char *__name, int __mode, long)
{
    const char *__m = _C_get_mode_arg (__mode);

    if (!__m) {
        _C_fptr = 0;
        return *this;
    }
    
    _C_fptr = fopen (__name, __m);

#ifndef __ARM_DONT_WANT_IONBF
    if (_C_is_open ( ))
        setvbuf (_C_fptr, 0, _IONBF, 0);
#endif

    return *this;
}


inline int __rw_file_t::_C_get_mode () const
{
    return _C_get_mode (_C_fptr);
}

#endif // _RWSTD_NO_NATIVE_IO


_RWSTD_NAMESPACE_END   // __rw

#endif // _RWSTD_FILE_H_INCLUDED
