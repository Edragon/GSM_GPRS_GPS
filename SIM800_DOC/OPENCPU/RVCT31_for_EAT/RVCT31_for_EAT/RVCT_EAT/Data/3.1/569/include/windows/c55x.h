/* c55x.h - Emulation of selected TI C55x intrinsics */
/* Copyright (C) 2006 ARM Limited. All rights reserved. */

/*
 * RCS $Revision: 121159 $
 * Checkin $Date: 2006-11-30 17:00:11 +0000 (Thu, 30 Nov 2006) $
 * Revising $Author: agrant $
 */

#ifndef __arm_c55x_h
#define __arm_c55x_h

#include <stdint.h>

__inline int16_t _sadd(int16_t src1, int16_t src2)
{
  return __qadd(src1<<16, src2<<16) >> 16;
}

__inline int32_t _lsadd(int32_t src1, int32_t src2)
{
  return __qadd(src1, src2);
}

__inline int16_t _ssub(int16_t src1, int16_t src2)
{
  return __qsub(src1<<16, src2<<16) >> 16;
}

__inline int32_t _lssub(int32_t src1, int32_t src2)
{
  return __qsub(src1, src2);
}

__inline int16_t _sneg(int16_t src)
{
  return _ssub(0, src);
}

__inline int32_t _lsneg(int32_t src)
{ 
  return __qsub(0, src);
}

__inline int16_t _abss(int16_t src)
{
  return src < 0 ? _sneg(src) : src;
}

__inline int32_t _labss(int32_t src)
{
  return src < 0 ? _lsneg(src) : src;
}

__inline int16_t _smpy(int16_t src1, int16_t src2)
{
  return (int16_t)(__qdbl(src1*src2) >> 16);
}

__inline int32_t _lsmpy(int16_t src1, int16_t src2)
{
  return __qdbl(src1*src2);
}

__inline int32_t _smac(int32_t src1, int16_t src2, int16_t src3)
{
  return __qadd(src1, __qdbl(src2*src3));
}

__inline int32_t _smas(int32_t src1, int16_t src2, int16_t src3)
{
  return __qsub(src1, __qdbl(src2*src3));
}

__inline int16_t _sshl(int16_t src1, int16_t src2)
{
  if (src2 < 0) {
    return src1 >> -src2;
  } else {
    int16_t sh = src1 << src2;
    if ((sh >> src2) != src1) {
      sh = (src1 < 0) ? -0x8000 : 0x7FFF;
    }
    return sh;
  }
}

__inline int32_t _lsshl(int32_t src1, int16_t src2)
{
  if (src2 < 0) {
    return src1 >> -src2;
  } else {
    int32_t sh = src1 << src2;
    if ((sh >> src2) != src1) {
      sh = (src1 < 0) ? -0x80000000 : 0x7FFFFFFF;
    }
    return sh;
  }
}

__inline int16_t _shrs(int16_t src1, int16_t src2)
{
  if (src2 < 0) {
    return _sshl(src1, -src2);
  } else {
    return src1 >> src2;
  }
}

__inline int32_t _lshrs(int32_t src1, int16_t src2)
{
  if (src2 < 0) {
    return _lsshl(src1, -src2);
  } else {
    return src1 >> src2;
  }
}

__inline int16_t _shl(int16_t src1, int16_t src2)
{
  if (src2 < 0) {
    return src1 >> -src2;
  } else {
    return src1 << src2;
  }
}

__inline int32_t _lshl(int32_t src1, int16_t src2)
{
  if (src2 < 0) {
    return src1 >> -src2;
  } else {
    return src1 << src2;
  }
}

__inline int32_t _round(int32_t src)
{
  return (src + 0x8000) & 0xFFFF0000;
}

__inline int32_t _sround(int32_t src)
{
  return __qadd(src, 0x8000) & 0xFFFF0000;
}

__inline int32_t _rnd(int32_t src)
{
  return _sround(src);
}

__inline int32_t _roundn(int32_t src)
{
  if ((src & 0x17FFF) != 0) {
    src += (src & 0x8000);
  }
  return src & 0xFFFF0000;
}

__inline int32_t _sroundn(int32_t src)
{
 if ((src & 0x17FFF) != 0) {
    src = __qadd(src, (src & 0x8000));
  }
  return src & 0xFFFF0000;
}

__inline int16_t _norm(int16_t src)
{
  return __clz(src ^ ((int32_t)src << 17)) & 15;
}

__inline int16_t _lnorm(int32_t src)
{
  return __clz(src ^ (src << 1)) & 31;
}

__inline int16_t _count(uint64_t src1, uint64_t src2)
{
  return (int16_t)__builtin_popcountll(src1 & src2);
}

__inline int16_t _max(int16_t src1, int16_t src2)
{
  return (src1 > src2) ? src1 : src2;
}

__inline int32_t _lmax(int32_t src1, int32_t src2)
{
  return (src1 > src2) ? src1 : src2;
}

__inline int16_t _min(int16_t src1, int16_t src2)
{
  return (src1 < src2) ? src1 : src2;
}

__inline int32_t _lmin(int32_t src1, int32_t src2)
{
  return (src1 < src2) ? src1 : src2;
}

__inline int16_t _divs(int16_t src1, int16_t src2)
{
  if (src1 == -0x8000 && src2 == -1) {
    return 0x7FFF;
  }
  return src1 / src2;
}

#endif /* ndef __arm_c55x_h */

/* end of c55x.h */
