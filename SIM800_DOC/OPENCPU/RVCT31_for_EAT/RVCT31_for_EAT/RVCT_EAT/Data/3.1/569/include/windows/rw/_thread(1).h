/***************************************************************************
 *
 * _thread.h - Class encapsulating portable thread primitives
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _thread.h 91721 2005-08-08 18:30:07Z drodgman $
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

#ifndef _RWSTD_THREAD_H_INCLUDED
#define _RWSTD_THREAD_H_INCLUDED

#include <rw/_defs.h>

#if defined (_RWSTD_SOLARIS_THREADS)
#  include <thread.h>
#  include <unistd.h>   // for sleep()

#  define _RWSTD_THREAD_ID           thread_t
#  define _RWSTD_THREAD_PROC(name)   void* name (void *arg)
#  define _RWSTD_THREAD_CREATE(tid, proc, arg) \
          thr_create (0, 0, proc, arg, 0, &tid)
#  define _RWSTD_THREAD_JOIN(tid)    thr_join (tid, 0, 0)
#  define _RWSTD_SLEEP(nsec)         sleep (nsec)
#  define _RWSTD_THREAD_GETID(tid)   tid

#elif defined (_RWSTD_POSIX_D10_THREADS)

#  include <pthread.h>
#  include <unistd.h>   // for sleep()

#  define _RWSTD_THREAD_ID           pthread_t
#  define _RWSTD_THREAD_PROC(name)   void* name (void *arg)
#  define _RWSTD_THREAD_CREATE(tid, proc, arg) \
          pthread_create (&tid, 0, proc, arg)
#  define _RWSTD_THREAD_JOIN(tid)    pthread_join (tid, 0)
#  define _RWSTD_SLEEP(nsec)         sleep (nsec)
#  define _RWSTD_THREAD_GETID(tid)   tid

#elif defined (_RWSTD_DCE_THREADS)

#  if defined (_RWSTD_NO_DCE_PTHREAD_H)
#    include <pthread.h>
#  else
#    include <dce/pthread.h>
#  endif
#  include <unistd.h>

#  define _RWSTD_THREAD_ID pthread_t
#  define _RWSTD_THREAD_PROC(name) void* name (void *arg)
#  define _RWSTD_THREAD_CREATE(tid, proc, arg) \
          pthread_create (&tid, pthread_attr_default, proc, arg)
#  define _RWSTD_THREAD_JOIN(tid) pthread_join (tid, 0)
#  define _RWSTD_SLEEP(nsec)      sleep (nsec)
#  define _RWSTD_THREAD_GETID(tid)      pthread_getunique_np(&tid)

#elif defined(_WIN32)

#  include <windows.h>

#  define _RWSTD_THREAD_ID HANDLE
#  define _RWSTD_THREAD_PROC(name) DWORD name (void *arg)
#  define _RWSTD_THREAD_CREATE(tid, proc, arg) \
          ((tid = CreateThread (0, 0, (DWORD(__stdcall*)(void*))proc, \
                                arg, 0, 0)) ? 0 : -1)
#  define _RWSTD_THREAD_JOIN(tid) WaitForSingleObject (tid, INFINITE)
#  define _RWSTD_SLEEP(nsec)      Sleep (nsec)
#  define _RWSTD_THREAD_GETID(tid)      tid

#else
#  error "unknown thread environment"
#endif


_RWSTD_NAMESPACE_BEGIN (__rw)


extern "C" {

#if    (   !defined (__sgi)                                          \
        || !defined (_COMPILER_VERSION) || _COMPILER_VERSION > 730)  \
    && (!defined (__HP_aCC) || __HP_aCC > 32700)

// working around a bug in HP aCC +w (bogus future error 503)
// working around a bug in MIPSpro 7.3.1.1m
inline

#endif   // !__sgi || !_COMPILER_VERSION || _COMPILER_VERSION > 730

void* __rw_run_thread (void*);

}


class __rw_thread
{
public:

    enum _C_state_t {
        _C_created, _C_error, _C_suspended, _C_running, _C_exited
    };

    enum _C_attr_t { _C_attr_default, _C_attr_detached };

    __rw_thread (int __attr = _C_attr_default)
#if defined(_RWSTD_DCE_THREADS)
        : _C_state (_C_created),
          _C_attr (__attr) { }
#else
        : _C_tid (0),
          _C_state (_C_created),
          _C_attr (__attr) { }
#endif

    unsigned long _C_get_tid () const {
        typedef unsigned long ulong;
        return ulong(_RWSTD_THREAD_GETID(_C_tid));
    }

    _C_state_t _C_get_state () const {
        return _C_state;
    }

    void _C_run () {
        _C_state = _RWSTD_THREAD_CREATE (_C_tid, __rw_run_thread, this)
            ? _C_error : _C_running;
    }

    void _C_join () {
        if (_C_running == _C_state && !(_C_attr & _C_attr_detached))
            _C_state = _RWSTD_THREAD_JOIN (_C_tid) ? _C_error : _C_exited;
    }

    static void _C_sleep (unsigned __nsec) {
      _RWSTD_SLEEP (__nsec);
    }

#if !defined (__EDG_VERSION__) || __EDG_VERSION__ > 240

// edg (and derivatives) gets confused after seeing the friend
// declaration and forgets about the C linkage of the function

protected:

    friend void* __rw_run_thread (void*);

#else   // if defined (__EDG_VERSION__) && __EDG_VERSION__ <= 240

public:

#endif   // !defined (__EDG_VERSION__) || __EDG_VERSION__ > 240

    virtual void _C_do_run () = 0;

protected:

    operator void* () {
        return _C_join (), _C_result;
    }

    _RWSTD_THREAD_ID  _C_tid;      // thread id
    _C_state_t        _C_state;    // thread state
    int               _C_attr;     // thread attributes
    void             *_C_result;   // result of thread function
};


extern "C"  {

void* __rw_run_thread (void *__arg)
{
#if defined (sun) || defined (__sun) || defined (__sun__)
    // necessary to allow other threads to finish creation
    // (otherwise all threads will run in serially)
    sleep (1);
#endif   //  defined (sun) || defined (__sun) || defined (__sun__)

    ((__rw_thread*)__arg)->_C_do_run ();
    return 0;
}

}

_RWSTD_NAMESPACE_END   // __rw


#endif   // _RWSTD_THREAD_H_INCLUDED
