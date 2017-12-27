/* tgmath.h: ISO/IEC 9899:1999 (C99), section 7.22 */

/* Copyright (C) ARM Ltd., 2006
 * All rights reserved
 * RCS $Revision: 102466 $
 * Checkin $Date: 2006-08-23 11:37:17 +0100 (Wed, 23 Aug 2006) $
 * Revising $Author: statham $
 */

#ifndef __tgmath_h
#define __tgmath_h

#include <math.h>

#define _ARM_GENERIC1(F,x) __generic(x,,, F, F##f, F##l,,,)(x)
#define _ARM_GENERIC2(F,x,y) __generic(x,y,, F, F##f, F##l,,,)(x, y)
#define _ARM_GENERIC1I(F,x,i) __generic(x,,, F, F##f, F##l,,,)(x, i)
#define _ARM_GENERIC3(F,x,y,z) __generic(x,y,z, F, F##f, F##l,,,)(x, y, z)
#define _ARM_GENERIC2I(F,x,y,i) __generic(x,y,, F, F##f, F##l,,,)(x, y, i)

#define acos(x) _ARM_GENERIC1(acos, x)
#define asin(x) _ARM_GENERIC1(asin, x)
#define atan(x) _ARM_GENERIC1(atan, x)
#define acosh(x) _ARM_GENERIC1(acosh, x)
#define asinh(x) _ARM_GENERIC1(asinh, x)
#define atanh(x) _ARM_GENERIC1(atanh, x)
#define cos(x) _ARM_GENERIC1(cos, x)
#define sin(x) _ARM_GENERIC1(sin, x)
#define tan(x) _ARM_GENERIC1(tan, x)
#define cosh(x) _ARM_GENERIC1(cosh, x)
#define sinh(x) _ARM_GENERIC1(sinh, x)
#define tanh(x) _ARM_GENERIC1(tanh, x)
#define exp(x) _ARM_GENERIC1(exp, x)
#define log(x) _ARM_GENERIC1(log, x)
#define pow(x, y) _ARM_GENERIC2(pow, x, y)
#define sqrt(x) _ARM_GENERIC1(sqrt, x)
#define fabs(x) _ARM_GENERIC1(fabs, x)

/* 7.22#5: Functions without complex variants */
#define atan2(x, y) _ARM_GENERIC2(atan2, x, y)
#define log10(x) _ARM_GENERIC1(log10, x)
#define cbrt(x) _ARM_GENERIC1(cbrt, x)
#define ceil(x) _ARM_GENERIC1(ceil, x)
#define copysign(x, y) _ARM_GENERIC2(copysign, x, y)
#define erf(x) _ARM_GENERIC1(erf, x)
#define erfc(x) _ARM_GENERIC1(erfc, x)
#define exp2(x) _ARM_GENERIC1(exp2, x)
#define expm1(x) _ARM_GENERIC1(expm1, x)
#define fdim(x, y) _ARM_GENERIC2(fdim, x, y)
#define floor(x) _ARM_GENERIC1(floor, x)
#define fma(x, y, z) _ARM_GENERIC3(fma, x, y, z)
#define fmax(x, y) _ARM_GENERIC2(fmax, x, y)
#define fmin(x, y) _ARM_GENERIC2(fmin, x, y)
#define fmod(x, y) _ARM_GENERIC2(fmod, x, y)
#define frexp(x, i) _ARM_GENERIC1I(frexp, x, i)
#define hypot(x, y) _ARM_GENERIC2(hypot, x, y)
#define ilogb(x) _ARM_GENERIC1(ilogb, x)
#define ldexp(x, i) _ARM_GENERIC1I(ldexp, x, i)
#define lgamma(x) _ARM_GENERIC1(lgamma, x)
#define llrint(x) _ARM_GENERIC1(llrint, x)
#define llround(x) _ARM_GENERIC1(llround, x)
#define log10(x) _ARM_GENERIC1(log10, x)
#define log1p(x) _ARM_GENERIC1(log1p, x)
#define log2(x) _ARM_GENERIC1(log2, x)
#define logb(x) _ARM_GENERIC1(logb, x)
#define lrint(x) _ARM_GENERIC1(lrint, x)
#define lround(x) _ARM_GENERIC1(lround, x)
#define nearbyint(x) _ARM_GENERIC1(nearbyint, x)
#define nextafter(x, y) _ARM_GENERIC2(nextafter, x, y)
#define nexttoward(x, y) _ARM_GENERIC1I(nexttoward, x, y)
#define remainder(x, y) _ARM_GENERIC2(remainder, x, y)
#define remquo(x, y, p) _ARM_GENERIC2I(remquo, x, y, p)
#define rint(x) _ARM_GENERIC1(rint, x)
#define round(x) _ARM_GENERIC1(round, x)
#define scalbn(x, i) _ARM_GENERIC1I(scalbn, x, i)
#define scalbln(x, i) _ARM_GENERIC1I(scalbln, x, i)
#define tgamma(x) _ARM_GENERIC1(tgamma, x)
#define trunc(x) _ARM_GENERIC1(trunc, x)

#endif /* included */

/* end of tgmath.h */
