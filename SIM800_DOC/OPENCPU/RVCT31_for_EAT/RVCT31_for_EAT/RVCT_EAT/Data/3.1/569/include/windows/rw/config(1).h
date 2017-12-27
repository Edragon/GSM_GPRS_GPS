/* Copyright (c) 1994-2001 Rogue Wave Software, Inc.  All Rights Reserved. */
/* Copyright (c) 2002 ARM Ltd. All Rights Reserved.                        */
#ifndef __RW_CONFIG_H_INCLUDED
#define __RW_CONFIG_H_INCLUDED

#define _RWSTD_NO_NATIVE_IO // Rogue Wave global config options
#define _RWSTD_NO_CONST_FLT_ROUNDS // auto config
#ifndef __EXCEPTIONS
#define _RWSTD_NO_EXCEPTIONS // auto config
#else
#endif
#define _RWSTD_NO_FGETWC // auto config
#define _RWSTD_NO_FGETWS // auto config
#define _RWSTD_NO_FILENO // auto config
#define _RWSTD_NO_FPUTWC // auto config
#define _RWSTD_NO_FPUTWS // auto config
#ifndef __EXCEPTIONS
#define _RWSTD_NO_FUNCTION_TRY_BLOCK // auto config
#endif
#define _RWSTD_NO_FWIDE // auto config
#define _RWSTD_NO_FWPRINTF // auto config
#define _RWSTD_NO_FWSCANF // auto config
#define _RWSTD_NO_GETWC // auto config
#define _RWSTD_NO_GETWCHAR // auto config
#define _RWSTD_NO_IEEEFP_H_REQUIRED // auto config
#define _RWSTD_LLONG_PRINTF_PREFIX "ll"
#define _RWSTD_LONG_LONG __int64
#define _RWSTD_NO_LRAND48 // auto config
#define _RWSTD_NO_MATH_EXCEPTION // auto config
#define _RWSTD_NO_MBSTATE_COMPARE // auto config
#ifndef __EXCEPTIONS
#define _RWSTD_NO_NEW_THROWS // auto config
#else
#endif
#define _RWSTD_NO_NL_TYPES_H // auto config
#define _RWSTD_NO_PUTWC // auto config
#define _RWSTD_NO_PUTWCHAR // auto config
#define _RWSTD_NO_SSIZE_T // auto config
#ifndef __EXCEPTIONS
#define _RWSTD_NO_UNCAUGHT_EXCEPTION // auto config
#else
#endif
#define _RWSTD_NO_UNGETWC // auto config
#define _RWSTD_NO_VFWPRINTF // auto config
#define _RWSTD_NO_VWPRINTF // auto config
#define _RWSTD_NO_VWSCANF // auto config
#define _RWSTD_WINT_T int
#define _RWSTD_NO_WPRINTF // auto config
#define _RWSTD_NO_WSCANF // auto config

#endif /* __RW_CONFIG_H_INCLUDED */
