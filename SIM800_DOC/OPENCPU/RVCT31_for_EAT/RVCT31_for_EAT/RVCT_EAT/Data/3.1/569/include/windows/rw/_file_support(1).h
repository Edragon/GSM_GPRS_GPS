/* added file to implement some of the fileno behaviour */

#ifndef _RWSTD_FILE_SUPPORT_H_INCLUDED
#define _RWSTD_FILE_SUPPORT_H_INCLUDED

#include <rw/_defs.h>
#include _RWSTD_CSTDIO

_RWSTD_NAMESPACE_BEGIN (__rw)

_USING (namespace std);

#ifdef _RWSTD_NO_FILENO

#ifndef STDIN_FILENO 
#  define STDIN_FILENO  0
#  define STDOUT_FILENO 1
#  define STDERR_FILENO 2
#endif //STDIN_FILENO 

inline int fileno (FILE* _C_fptr) // very simple and not complete implementation of fileno
// not extern "C", which introduces additional complications by masquerading as a 'real' fileno
{
    
   if (_C_fptr == &std::__stdin) return STDIN_FILENO;
   if (_C_fptr == &std::__stdout) return STDOUT_FILENO;
   if (_C_fptr == &std::__stderr) return STDERR_FILENO;
   return (int)_C_fptr; // should be unique, but fdopen will not work
}
#endif //STDIN_FILENO

_RWSTD_NAMESPACE_END   // __rw

#endif //_RWSTD_FILE_SUPPORT_H_INCLUDED


