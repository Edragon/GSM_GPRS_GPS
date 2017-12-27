/***************************************************************************
 *
 * _mutex.h - Definitions of classes and inline functions providing MT safety
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
 *
 * $Id: _mutex.h 91721 2005-08-08 18:30:07Z drodgman $
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
 ***************************************************************************
 *
 * There are two mutex classes:
 *
 * __rw::__rw_mutex_base - a POD-type class with no ctor or dtor (POSIX
 * and Solaris threads only) suitable for mutex objects with static storage
 * duration. On POSIX threads, these objects are fully constructed at static
 * initialization time and initialized via assignment by 
 * PTHREAD_MUTEX_INITIALIZER, while on Solaris threads they do not need to be
 * initialized at all. On platforms such as Win32/64 that do not support static
 * initialization this class does define a ctor that initializes the object
 * by the appropriate thread library call.
 *
 * __rw::__rw_mutex - a non-POD-type class derived from __rw::__rw_mutex_base
 * with explicitly defined ctor and dtor suitable for mutex member variables.
 * On platforms such as Win32/64 that do not support static initialization this
 * class does not define its own ctor and dtor and simply defers the
 * initialization and destruction to its base.
 *
 * __rw::__rw_static_mutex<> - a POD-type class template, defined on
 * platforms such as POSIX or Solaris threads that support static
 * initialization of mutexes, containing a single public static data member
 * of the __rw::__rw_mutex_base class. The static member object is returned
 * from the factory function template __rw::__rw_get_static_mutex<>().
 *
 * On platforms such as Win32/64 that do not support static initialization
 * of mutex objects the factory template function
 * __rw::__rw_get_static_mutex<>() defines and dynamically initializes
 * a static local reference to an object of the __rw::__rw_mutex_base class.
 * The dynamic initialization is done in an MT-safe way (i.e., such that
 * exactly one initialization of the mutex object is guaranteed). On Win32/64
 * this is accomplished by making use of the InterlockedIncrement() API call,
 * everywhere else there exists a small potential for a race condition and
 * the risk of the object being initialized multiple times. The reference is
 * initialized to refer to a properly aligned static data buffer to prevent
 * the destruction of the actual object at program termination, and the object
 * itself is constructed in this buffer space via a call to placement new.
 *
 ***************************************************************************/

#ifndef _RWSTD_MUTEX_H_INCLUDED
#define _RWSTD_MUTEX_H_INCLUDED

#include <rw/_defs.h>

#include _RWSTD_CSTRING

#ifdef _RWSTD_MULTI_THREAD


#include <rw/_exception.h>


#if defined (_RWSTD_SOLARIS_THREADS)  // assuming Solaris 2.1 or greater

// SunOS 5.7 Threads Library:
//   "A statically  allocated  mutex does  not  need to be explicitly
//   initialized; by default, a statically allocated mutex is initialized
//   with  all  zeros and its scope is set to be within the calling
//   process."

#  include <synch.h>
#  include <thread.h>

#  define _RWSTD_MUTEX_INIT(mutex)      mutex_init (&mutex, USYNC_THREAD, 0)
#  define _RWSTD_MUTEX_DESTROY(mutex)   mutex_destroy (&mutex)
#  define _RWSTD_MUTEX_LOCK(mutex)      mutex_lock (&mutex)
#  define _RWSTD_MUTEX_UNLOCK(mutex)    mutex_unlock (&mutex)
#  define _RWSTD_MUTEX_T                mutex_t

#elif defined (_RWSTD_POSIX_D10_THREADS)

// LinuxThreads man page:
//   "Variables of type pthread_mutex_t can also be initialized
//    statically, using the constants  PTHREAD_MUTEX_INITIALIZER
//    (for fast mutexes), PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
//    (for recursive mutexes), and PTHREAD_ERRORCHECK_MUTEX_INI-
//    TIALIZER_NP (for error checking mutexes)."
//    ...
//    "Attempting to initialize an already initialized mutex results
//    in undefined behavior."

#  include <pthread.h>

#  define _RWSTD_MUTEX_INIT(mutex)      pthread_mutex_init (&mutex, 0)
#  define _RWSTD_MUTEX_DESTROY(mutex)   pthread_mutex_destroy (&mutex)
#  define _RWSTD_MUTEX_LOCK(mutex)      pthread_mutex_lock (&mutex)
#  define _RWSTD_MUTEX_UNLOCK(mutex)    pthread_mutex_unlock (&mutex)
#  define _RWSTD_MUTEX_T                pthread_mutex_t

#elif defined (_RWSTD_DCE_THREADS)

#  if defined (_RWSTD_NO_DCE_PTHREAD_H)
#    include <pthread.h>
#  else
#    include <dce/pthread.h>
#  endif

#  define _RWSTD_MUTEX_INIT(mutex) \
          pthread_mutex_init (&mutex, pthread_mutexattr_default)
#  define _RWSTD_MUTEX_DESTROY(mutex)   pthread_mutex_destroy (&mutex)
#  define _RWSTD_MUTEX_LOCK(mutex)      pthread_mutex_lock (&mutex)
#  define _RWSTD_MUTEX_UNLOCK(mutex)    pthread_mutex_unlock (&mutex)
#  define _RWSTD_MUTEX_T                pthread_mutex_t

// DCE doesn't seem to support static mutex initialization
#  include <new>

#elif defined (_WIN32) || defined (_WIN64)

#  ifdef _RWSTD_NO_FWD_DECLARATIONS

#    include <windows.h>
#    define _RWSTD_MUTEX_T _RTL_CRITICAL_SECTION

#  else   // if defined (_RWSTD_NO_FWD_DECLARATIONS)

   // avoid #including this header...
   // #  include <windows.h>

extern "C" {

// but rather declare these globals here
struct _RTL_CRITICAL_SECTION;

__declspec (dllimport) void __stdcall
InitializeCriticalSection (_RTL_CRITICAL_SECTION*);

__declspec (dllimport) void __stdcall
EnterCriticalSection (_RTL_CRITICAL_SECTION*);

__declspec (dllimport) void __stdcall
LeaveCriticalSection (_RTL_CRITICAL_SECTION*);

__declspec (dllimport) void __stdcall
DeleteCriticalSection (_RTL_CRITICAL_SECTION*);

__declspec (dllimport) long __stdcall
InterlockedIncrement (long*);

__declspec (dllimport) long __stdcall
InterlockedDecrement (long*);

__declspec (dllimport) long __stdcall
InterlockedExchange (long*, long);

}   // extern "C"

_RWSTD_NAMESPACE_BEGIN (__rw)

// fake critical section type
union __rw_critical_section {
    long _C_pad;   // force alignment
    char _C_buf [24 /* == sizeof (_RTL_CRITICAL_SECTION) */];
};

#    define _RWSTD_MUTEX_T _RW::__rw_critical_section

_RWSTD_NAMESPACE_END   // __rw


#  endif   // _RWSTD_NO_FWD_DECLARATIONS


_RWSTD_NAMESPACE_BEGIN (__rw)

// Win32/64 throws non-C++ exceptions rather than returning error status
// from some system calls like most other operating systems do

inline int __rw_mutex_init (_RTL_CRITICAL_SECTION *__mutex)
{
    __try {
        InitializeCriticalSection (__mutex);
    }
    __except (1) {
        return -1;
    }
    return 0;
}

inline int __rw_mutex_destroy (_RTL_CRITICAL_SECTION *__mutex)
{
    __try {
        DeleteCriticalSection (__mutex);
    }
    __except (1) {
        return -1;
    }
    return 0;
}

inline int __rw_mutex_lock (_RTL_CRITICAL_SECTION *__mutex)
{
    __try {
        EnterCriticalSection (__mutex);
    }
    __except (1) {
        return -1;
    }
    return 0;
}

inline int __rw_mutex_unlock (_RTL_CRITICAL_SECTION *__mutex)
{
    __try {
        LeaveCriticalSection (__mutex);
    }
    __except (1) {
        return -1;
    }
    return 0;
}

#  define _RWSTD_MUTEX_INIT(mutex)      \
   __rw_mutex_init (_RWSTD_REINTERPRET_CAST (_RTL_CRITICAL_SECTION*, &mutex))
#  define _RWSTD_MUTEX_DESTROY(mutex)   \
   __rw_mutex_destroy (_RWSTD_REINTERPRET_CAST (_RTL_CRITICAL_SECTION*, &mutex))
#  define _RWSTD_MUTEX_LOCK(mutex)      \
   __rw_mutex_lock (_RWSTD_REINTERPRET_CAST (_RTL_CRITICAL_SECTION*, &mutex))
#  define _RWSTD_MUTEX_UNLOCK(mutex)    \
   __rw_mutex_unlock (_RWSTD_REINTERPRET_CAST (_RTL_CRITICAL_SECTION*, &mutex))

_RWSTD_NAMESPACE_END   // __rw

#elif defined (__OS2__)

#  define INCL_DOSSEMAPHORES

#  include <os2.h>

#  define _RWSTD_MUTEX_INIT(mutex) \
          DosCreateMutexSem (0, &mutex, DC_SEM_SHARED,FALSE)
#  define _RWSTD_MUTEX_DESTROY(mutex)   DosCloseMutexSem (mutex)
#  define _RWSTD_MUTEX_LOCK(mutex) \
          DosRequestMutexSem (mutex, SEM_INDEFINITE_WAIT)
#  define _RWSTD_MUTEX_UNLOCK(mutex)    DosReleaseMutexSem (mutex)
#  define _RWSTD_MUTEX_T                HMTX

#else
#  error unknown thread environment
#endif


#if defined (__DECCXX)
   // get declarations of __ATOMIC_XXX intrinsics

#  include <machine/builtins.h>

#endif   // __DECXX


#if defined (__GNUG__) && defined (__osf__)
   // prevent g++ warnings about missing initializers
   // see <pthread.h> for explanation of _PTHREAD_NOMETER_STATIC
#  ifndef _PTHREAD_NOMETER_STATIC
#    define _RWSTD_PTHREAD_MUTEX_INITIALIZER \
            { _PTHREAD_MSTATE_SLOW, _PTHREAD_MVALID | _PTHREAD_MVF_STA, \
              0, 0, 0, 0, 0, 0 }
#  else   // if defined (_PTHREAD_NOMETER_STATIC)
#    define _RWSTD_PTHREAD_MUTEX_INITIALIZER
            { 0, _PTHREAD_MVALID | _PTHREAD_MVF_STA, 0, 0, 0, 0, 0, 0 }
#  endif   // _PTHREAD_NOMETER_STATIC
#elif defined (__GNUG__) && defined (__sgi__)
   // prevent g++ warnings about a partly bracketed initializer
#  define _RWSTD_PTHREAD_MUTEX_INITIALIZER { PTHREAD_MUTEX_INITIALIZER }
#else
#  define _RWSTD_PTHREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#endif


_RWSTD_NAMESPACE_BEGIN (__rw)

// a using declaration (rather than a directive)
// used to work around yet another MSVC 6.0 bug
_USING (std::exception);

class __rw_thread_error : public exception
{
public:
    __rw_thread_error () _THROWS (())
    : exception () { }

    virtual const char* what () const _THROWS (()) {
        return "thread synchronization error";
    }
};                            


// POD type with no user-defined ctor or dtor facilitates static
// initialization of mutex objects with static storage duration
// (important during library initialziation time)
class _RWSTD_EXPORT __rw_mutex_base
{
public:

    void _C_acquire () {
#if !defined (__HP_aCC) || __HP_aCC > 32700
        if (0 != _RWSTD_MUTEX_LOCK (_C_mutex))
            _RW::__rw_throw (_RWSTD_ERROR_RUNTIME_ERROR,
                             "synchronization error");
#else
        // working around an HP aCC 3.27 bug JAGac88738
        _RWSTD_MUTEX_LOCK (_C_mutex);
#endif   // !defined (__HP_aCC) || __HP_aCC > 32700
    }

    void _C_release ();

#ifdef _RWSTD_NO_STATIC_MUTEX_INIT

    // static initialization not an option, define ctor and dtor
    // and make member mutex private

    __rw_mutex_base ();

    ~__rw_mutex_base ();

private:

    // not defined
    __rw_mutex_base (const __rw_mutex_base&);
    __rw_mutex_base& operator= (const __rw_mutex_base&);

#endif   // _RWSTD_NO_STATIC_MUTEX_INIT

    _RWSTD_MUTEX_T _C_mutex;   // the real thing
};


inline void __rw_mutex_base::_C_release ()
{
    // we should NOT throw from here as _C_release will typically be called
    // during the destruction of local objects such as __rw_guard (perhaps
    // due to another exception)
    _RWSTD_MUTEX_UNLOCK (_C_mutex);
}


// non-POD type, always initializes mutex data member via a function call
class  _RWSTD_EXPORT __rw_mutex: public __rw_mutex_base
{
public:

#ifndef _RWSTD_NO_STATIC_MUTEX_INIT

    __rw_mutex ();

    ~__rw_mutex ();

private:

    // not defined
    __rw_mutex (const __rw_mutex&);
    __rw_mutex& operator= (const __rw_mutex&);

#endif   // _RWSTD_NO_STATIC_MUTEX_INIT

};


#ifndef _RWSTD_NO_STATIC_MUTEX_INIT

// helper factory class - static member is guranteed to be constructed
// during static initialization; objects of this POD class are not
// destroyed during program termination (important to allow them
// to be used in dtors of other objects with static storage duration)
template <class _TypeT>
struct __rw_static_mutex
{
    static __rw_mutex_base _C_mutex;
};


template <class _TypeT>
__rw_mutex_base __rw_static_mutex<_TypeT>::_C_mutex

#ifdef _RWSTD_POSIX_D10_THREADS

    = { _RWSTD_PTHREAD_MUTEX_INITIALIZER }

#endif   // _RWSTD_POSIX_D10_THREADS
        ;

// explicitly instantiated to work around a g++ 2.95.2 bug on COFF systems
// (such as IBM AIX or DEC OSF1) where it "forgets" to do so implicitly for
// explicitly initialized static data members
_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_static_mutex<int>);
_RWSTD_INSTANTIATE_1 (class _RWSTD_EXPORT __rw_static_mutex<size_t>);


inline __rw_mutex::__rw_mutex ()

#else   // if defined (_RWSTD_NO_STATIC_MUTEX_INIT)

inline __rw_mutex_base::__rw_mutex_base ()

#endif   // _RWSTD_NO_STATIC_MUTEX_INIT
{ 
    if (0 != _RWSTD_MUTEX_INIT (_C_mutex))
        _RW::__rw_throw (_RWSTD_ERROR_RUNTIME_ERROR, "synchronization error");
}

#ifndef _RWSTD_NO_STATIC_MUTEX_INIT

inline __rw_mutex::~__rw_mutex () 

#else   // if defined (_RWSTD_NO_STATIC_MUTEX_INIT)

inline __rw_mutex_base::~__rw_mutex_base () 

#endif   // _RWSTD_NO_STATIC_MUTEX_INIT
{
    _RWSTD_MUTEX_DESTROY (_C_mutex);
}


// mutex factory function avoids problematic initialization dependencies
// introduced by definitions of mutexes with static storage duration
// instead of defining a static mutex member that may not be initialized
// before first use, a non-local class may either declare a unique type
// (enum or class) or use itself as this type, and call this template
// function with a ptr to that type to obtain an initialized mutex object
// that is the same for each unique type passed to the function

#ifndef _RWSTD_NO_STATIC_MUTEX_INIT

template <class _TypeT>
inline
__rw_mutex_base& __rw_get_static_mutex (_TypeT*)
{
    // POD mutex already initialized during static initialization
    return __rw_static_mutex<_TypeT>::_C_mutex;
}

#else   // if defined (_RWSTD_NO_STATIC_MUTEX_INIT)

#ifdef _INLINE_WITH_STATICS

template <class _TypeT>
_INLINE_WITH_STATICS
__rw_mutex_base& __rw_get_static_mutex (_TypeT*)

#else    // if !defined (_INLINE_WITH_STATICS)

template <class _TypeT>
__rw_mutex_base& __rw_get_static_mutex (_TypeT*)

#endif   // _INLINE_WITH_STATICS

{
    // allocate properly aligned memory for static mutex (necessary
    // to prevent static local mutex from being destroyed during
    // program termination)
    union __mutex_buf_t {
        long double __pad;   // force alignment (must be first)
        char        __buf [sizeof (__rw_mutex_base)];
    };

    // using a named union to work around a bug in HP aCC 3.14.10 (JAGad03246)
    static __mutex_buf_t __mutex_buf;

    // initialize mutex reference to refer to the static buffer space
    __rw_mutex_base &__mutex =
        _RWSTD_REINTERPRET_CAST (__rw_mutex_base&, __mutex_buf);

    // keep track of number of mutex initialization attempts
    // although `init' may reach a value greater than 1, `mutex'
    // will (should) never be multiply initialized

    // implicit initialization used to prevent a g++ 2.95.2 warning on Tru64
    // sorry: semantics of inline function static data are wrong (you'll wind
    // up with multiple copies)
    static volatile long __cntr /* = 0 */;   // initialization counter

#if defined (_WIN32) || defined (_WIN64)
    // MT safe
    if (0 == __cntr && 1 == InterlockedIncrement ((long*)&__cntr))
#else
    // not so safe (volatile should help)
    if (0 == __cntr && 1 == ++__cntr)
#endif   // _WIN32

    {
        // manually initialize `mutex' via a call to placement new
        new (&__mutex) __rw_mutex_base ();

        // indicate that `mutex' has been fully initialized
        // (unlikely that we'll have more than 1000 threads)
        __cntr += 1000;
    }
    else
        // busywait until `mutex' has been completely initialized
        while (__cntr < 1000);

    return __mutex;
}

#endif   //_RWSTD_NO_STATIC_MUTEX_INIT


// clean up
#undef _RWSTD_MUTEX_LOCK
#undef _RWSTD_MUTEX_UNLOCK
#undef _RWSTD_MUTEX_T


// allows safe use of a mutex in the presence of exceptions
class __rw_guard
{
    __rw_mutex_base *_C_mutex;

    // undefined
    __rw_guard& operator= (const __rw_guard&);
    
public:

    __rw_guard (__rw_guard &__rhs)
        : _C_mutex (__rhs._C_mutex) {
        __rhs._C_mutex = 0;
    }

    __rw_guard (__rw_mutex_base &__mutex): _C_mutex (&__mutex) {
        _C_mutex->_C_acquire ();
    }

    __rw_guard (__rw_mutex_base *__mutex): _C_mutex (__mutex) {
        if (_C_mutex)
            _C_mutex->_C_acquire ();
    }

    ~__rw_guard () {
        if (_C_mutex)
            _C_mutex->_C_release ();
    }

    __rw_mutex_base* _C_set (__rw_mutex_base *__mutex) {
        __rw_mutex_base *__tmp = _C_mutex;
        return _C_mutex = __mutex, __tmp;
    }
};


// base class for obects required to guarantee some degree of MT safety
struct _RWSTD_EXPORT __rw_synchronized
{
    __rw_mutex _C_mutex;

    void _C_lock () {
        _C_mutex._C_acquire ();
    }

    void _C_unlock () {
        _C_mutex._C_release ();
    }

    __rw_guard _C_guard () {
        __rw_guard __guard (_C_mutex);
        return __guard;
    }
};


// helper functions for atomic value [in|de]crement and exchange
// the functions are atomic with respect to each other as long as
// they are passed the same mutex by the callers
template <class _TypeT>
inline
_TypeT __rw_atomic_preincrement (_TypeT &__t, __rw_mutex_base &__mutex)
{
    _RWSTD_MT_GUARD (__mutex);

    return ++__t;
}


template <class _TypeT>
inline
_TypeT __rw_atomic_predecrement (_TypeT &__t, __rw_mutex_base &__mutex)
{
    _RWSTD_MT_GUARD (__mutex);

    return --__t;
}


template <class _TypeT, class _TypeU>
inline
_TypeT __rw_atomic_exchange (_TypeT &__t, const _TypeU &__u,
                             __rw_mutex_base &__mutex)
{
    _RWSTD_MT_GUARD (__mutex);

    _TypeT __tmp = __t;
    __t = __u;
    return __tmp;
}


// for use on class statics or on namespace-scope variables
// the unused argument is only here so that all functions can be
// called from the same set of macros

template <class _TypeT>
inline
_TypeT __rw_atomic_preincrement (_TypeT &__t, bool)
{
    return __rw_atomic_preincrement (__t,
                                     __rw_get_static_mutex ((_TypeT*)0));
}


template <class _TypeT>
inline
_TypeT __rw_atomic_predecrement (_TypeT &__t, bool)
{
    return __rw_atomic_predecrement (__t,
                                     __rw_get_static_mutex ((_TypeT*)0));
}


template <class _TypeT, class _TypeU>
inline
_TypeT __rw_atomic_exchange (_TypeT &__t, const _TypeU &__u, bool)
{
    return __rw_atomic_exchange (__t, __u,
                                 __rw_get_static_mutex ((_TypeT*)0));
}

/********************** DEC CXX **************************************/

#if defined (__DECCXX) && !defined (_RWSTD_NO_ATOMIC_OPERATIONS)

// __ATOMIC_[DE|IN]CREMENT_[LONG|QUAD] and __ATOMIC_EXCH_[LONG|QUAD] are
// intrinsic functions declared in <machine/builtins.h> that atomically
// modify their argument and return its original value (__ATOMIC_XXX_LONG
// is misnamed -- it actually operates on an int, not a long)

inline
int __rw_atomic_preincrement (int &__x, bool)
{
    return 1 + __ATOMIC_INCREMENT_LONG (&__x);
}


inline
unsigned __rw_atomic_preincrement (unsigned &__x, bool)
{
    return __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (int&, __x),
                                     false);
}


inline
long __rw_atomic_preincrement (long &__x, bool)
{
    return 1 + __ATOMIC_INCREMENT_QUAD (&__x);
}


inline
unsigned long __rw_atomic_preincrement (unsigned long &__x, bool)
{
    return __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                     false);
}


template <class _TypeT>
inline _TypeT* __rw_atomic_preincrement (_TypeT* &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return _RWSTD_REINTERPRET_CAST (_TypeT*,
             __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                       false));
}


inline
int __rw_atomic_predecrement (int &__x, bool)
{
    return __ATOMIC_DECREMENT_LONG (&__x) - 1;
}

inline
unsigned __rw_atomic_predecrement (unsigned &__x, bool)
{
    return __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (int&, __x),
                                     false);
}


inline
long __rw_atomic_predecrement (long &__x, bool)
{
    return __ATOMIC_DECREMENT_QUAD (&__x) - 1;
}


inline
unsigned long __rw_atomic_predecrement (unsigned long &__x, bool)
{
    return __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                     false);
}


template <class _TypeT>
inline _TypeT* __rw_atomic_predecrement (_TypeT* &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return _RWSTD_REINTERPRET_CAST (_TypeT*,
             __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                       false));
}


inline
int __rw_atomic_exchange (int &__x, int __y, bool)
{
    return __ATOMIC_EXCH_LONG (&__x, __y);
}


inline
unsigned __rw_atomic_exchange (unsigned &__x, unsigned __y, bool)
{
    return __rw_atomic_exchange (_RWSTD_REINTERPRET_CAST (unsigned&, __x),
                                 _RWSTD_STATIC_CAST (int, __y), false);
}


inline
long __rw_atomic_exchange (long &__x, long __y, bool)
{
    return __ATOMIC_EXCH_QUAD (&__x, __y);
}


inline
unsigned long __rw_atomic_exchange (unsigned long &__x, unsigned long __y, bool)
{
    return __rw_atomic_exchange (_RWSTD_REINTERPRET_CAST (unsigned long&, __x),
                                 _RWSTD_STATIC_CAST (long, __y), false);
}


template <class _TypeT>
inline _TypeT* __rw_atomic_exchange (_TypeT* &__x, const _TypeT* __y, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return _RWSTD_REINTERPRET_CAST (_TypeT*,
             __rw_atomic_exchange (_RWSTD_REINTERPRET_CAST (long&, __x),
                                   _RWSTD_REINTERPRET_CAST (long,  __y),
                                   false));
}

/********************** gcc/i86 **************************************/

#elif defined (__i386__) && defined (__GNUG__)

inline
int __rw_atomic_preincrement (int &__x, bool)
{
    __asm__ __volatile__ ("lock; addl $1,%0" : : "m" (__x));

    return __x;
}


inline
unsigned __rw_atomic_preincrement (unsigned &__x, bool)
{
    return __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (int&, __x),
                                     false);
}


inline
long __rw_atomic_preincrement (long &__x, bool)
{
    return __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (int&, __x),
                                     false);
}


inline
unsigned long __rw_atomic_preincrement (unsigned long &__x, bool)
{
    return __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (int&, __x),
                                     false);
}


template <class _TypeT>
inline _TypeT* __rw_atomic_preincrement (_TypeT* &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return _RWSTD_REINTERPRET_CAST (_TypeT*,
             __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                       false));
}


inline
int __rw_atomic_predecrement (int &__x, bool)
{
    __asm__ __volatile__ ("lock; subl $1,%0" : : "m" (__x));

    return __x;
}

inline
unsigned __rw_atomic_predecrement (unsigned &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (int));

    return __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (int&, __x),
                                     false);
}


inline
long __rw_atomic_predecrement (long &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (int));

    return __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (int&, __x),
                                     false);
}


inline
unsigned long __rw_atomic_predecrement (unsigned long &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (int));

    return __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (int&, __x),
                                     false);
}


template <class _TypeT>
inline _TypeT* __rw_atomic_predecrement (_TypeT* &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return _RWSTD_REINTERPRET_CAST (_TypeT*,
             __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                       false));
}

/********************** WIN 32/64 ************************************/

#elif defined (_WIN32) && !defined (_RWSTD_NO_ATOMIC_OPERATIONS)

// Interlocked[In|De]crement functions atomically modify their argument
// and return the new value

// InterlockedExchange atomically sets the value pointed to by the first
// argument to that of the second argument and returns the original value

inline
long __rw_atomic_preincrement (long &__x, bool)
{
    return InterlockedIncrement (&__x);
}


inline
unsigned long __rw_atomic_preincrement (unsigned long &__x, bool)
{
    return __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                     false);
}


inline
int __rw_atomic_preincrement (int &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                     false);
}


inline
unsigned int __rw_atomic_preincrement (unsigned int &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                     false);
}


#if !defined (_WIN64)

template <class _TypeT>
inline _TypeT* __rw_atomic_preincrement (_TypeT* &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return _RWSTD_REINTERPRET_CAST (_TypeT*,
             __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                       false));
}

#endif   // _WIN64

inline
long __rw_atomic_predecrement (long &__x, bool)
{
    return InterlockedDecrement (&__x);
}


inline
unsigned long __rw_atomic_predecrement (unsigned long &__x, bool)
{
    return __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                     false);
}


inline
int __rw_atomic_predecrement (int &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                     false);
}


inline
unsigned int __rw_atomic_predecrement (unsigned int &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                     false);
}


#if !defined (_WIN64)

template <class _TypeT>
inline _TypeT* __rw_atomic_predecrement (_TypeT* &__x, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return _RWSTD_REINTERPRET_CAST (_TypeT*,
             __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                       false));
}

#endif   // _WIN64

inline
long __rw_atomic_exchange (long &__x, long __y, bool)
{
    return InterlockedExchange (&__x, __y);
}


inline
unsigned long __rw_atomic_exchange (unsigned long &__x, unsigned long __y,
                                    bool)
{
    return __rw_atomic_exchange (_RWSTD_REINTERPRET_CAST (long&, __x),
                                 _RWSTD_STATIC_CAST (long, __y), false);
}


inline
int __rw_atomic_exchange (int &__x, int __y, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return __rw_atomic_exchange (_RWSTD_REINTERPRET_CAST (long&, __x),
                                 _RWSTD_STATIC_CAST (long, __y), false);
}


inline
unsigned int __rw_atomic_exchange (unsigned int &__x, unsigned int __y, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return __rw_atomic_exchange (_RWSTD_REINTERPRET_CAST (long&, __x),
                                 _RWSTD_STATIC_CAST (long, __y), false);
}


#if !defined (_WIN64)

template <class _TypeT>
inline _TypeT* __rw_atomic_exchange (_TypeT* &__x, const _TypeT* __y, bool)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return _RWSTD_REINTERPRET_CAST (_TypeT*,
             __rw_atomic_exchange (_RWSTD_REINTERPRET_CAST (long&, __x),
                                   _RWSTD_REINTERPRET_CAST (long,  __y),
                                   false));
}

#endif   // _WIN64


inline
long __rw_atomic_preincrement (long &__x, __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_preincrement (__x, false);
}


inline
unsigned long __rw_atomic_preincrement (unsigned long   &__x,
                                        __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_preincrement (__x, false);
}


inline
int __rw_atomic_preincrement (int &__x, __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_preincrement (__x, false);
}


inline
unsigned int __rw_atomic_preincrement (unsigned int    &__x,
                                       __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_preincrement (__x, false);
}


#if !defined (_WIN64)

template <class _TypeT>
inline
_TypeT* __rw_atomic_preincrement (_TypeT*         &__x,
                                  __rw_mutex_base &__mutex)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return _RWSTD_REINTERPRET_CAST (_TypeT*,
             __rw_atomic_preincrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                       __mutex));
}

#endif   // _WIN64

inline
long __rw_atomic_predecrement (long &__x, __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_predecrement (__x, false);
}


inline
unsigned long __rw_atomic_predecrement (unsigned long   &__x,
                                        __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_predecrement (__x, false);
}


inline
int __rw_atomic_predecrement (int &__x, __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_predecrement (__x, false);
}


inline
unsigned int __rw_atomic_predecrement (unsigned int    &__x,
                                       __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_predecrement (__x, false);
}


#if !defined (_WIN64)

template <class _TypeT>
inline
_TypeT* __rw_atomic_predecrement (_TypeT*         &__x,
                                  __rw_mutex_base &__mutex)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return _RWSTD_REINTERPRET_CAST (_TypeT*,
             __rw_atomic_predecrement (_RWSTD_REINTERPRET_CAST (long&, __x),
                                       __mutex));
}

#endif   // _WIN64

inline
long __rw_atomic_exchange (long &__x, long __y, __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_exchange (__x, __y, false);
}


inline
unsigned long __rw_atomic_exchange (unsigned long &__x, unsigned long __y,
                                    __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_exchange (__x, __y, false);
}


inline
int __rw_atomic_exchange (int &__x, int __y, __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_exchange (__x, __y, false);
}


inline
unsigned int __rw_atomic_exchange (unsigned int &__x, unsigned int __y,
                                   __rw_mutex_base &__mutex)
{
    _RWSTD_UNUSED (__mutex);

    return __rw_atomic_exchange (__x, __y, false);
}


#if !defined (_WIN64)

template <class _TypeT>
inline
_TypeT* __rw_atomic_exchange (_TypeT* &__x, const _TypeT* __y,
                              __rw_mutex_base &__mutex)
{
    _RWSTD_COMPILE_ASSERT (sizeof __x == sizeof (long));

    return _RWSTD_REINTERPRET_CAST (_TypeT*,
             __rw_atomic_exchange (_RWSTD_REINTERPRET_CAST (long&, __x),
                                   _RWSTD_REINTERPRET_CAST (long,  __y),
                                   __mutex));
}

#endif   // _WIN64

#endif   // _WIN32


_RWSTD_NAMESPACE_END   // __rw


#else   // if !defined (_RWSTD_MULTI_THREAD)


_RWSTD_NAMESPACE_BEGIN (__rw)

// atomic in a single-threaded environment
template <class _TypeT, class _TypeU>
inline
_TypeT __rw_atomic_exchange (_TypeT &__t, const _TypeU &__u, bool)
{
    _TypeT __tmp = __t;
    __t = __u;
    return __tmp;
}


// dummy classes used as a base class in single-threaded environments

struct __rw_mutex_base
{
    void _C_acquire () { }

    void _C_release () { }
};


struct _RWSTD_EXPORT __rw_mutex: public __rw_mutex_base
{
};


struct __rw_guard
{
    __rw_guard (__rw_mutex_base&) { }

    __rw_guard (__rw_mutex_base*) { }

    __rw_mutex_base* _C_set (__rw_mutex_base*) {
        return 0;
    }
};


struct __rw_synchronized
{
    // static so that it takes up no room
    static __rw_mutex _C_mutex;

    void _C_lock () { }

    void _C_unlock () { }

    __rw_guard _C_guard () {
        return __rw_guard (_C_mutex);
    }
};


_RWSTD_NAMESPACE_END   // __rw


#endif   // _RWSTD_MULTI_THREAD


_RWSTD_NAMESPACE_BEGIN (__rw)

// available in all environments (ST and MT), used along with
// __rw_atomic_exchange<>() from conditional expressions in iostreams
template <class _TypeT, class _TypeU>
inline
_TypeT __rw_ordinary_exchange (_TypeT &__t, const _TypeU &__u)
{
    _TypeT __tmp = __t;
    __t = __u;
    return __tmp;
}

_RWSTD_NAMESPACE_END   // __rw


#endif   // _RWSTD_MUTEX_H_INCLUDED
