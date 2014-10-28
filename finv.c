/* コンパイル時　gcc finv.c -lm */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "fadd.c"
#include "fmul.c"

#define MAX 2048
#define MASK11 8384512  //((1 << 11) - 1) << 12
#define FRAC_MAX 8388607 // 2^23
#define ZERO     0u
#define NZERO    2147483648u
#define INF      2139095040u
#define NINF     4286578688u

double make_a(double t, double c) {
  double a;
  a = 1 / (t * (t+c));
  return a;
}

double make_b(double t, double c) {
  double b, temp;
  temp = sqrt(1/t) + sqrt(1/(t+c));
  b = (temp * temp) / 2;
  return b;
}

uint32_t finv(uint32_t org) {
  float a_fl,b_fl;
  double a_db,b_db,c,t;
  unsigned int index;
  union data_32bit a,b,fraction,result,original;
  int d;
  
  original.uint32 = org;

  if (original.exp == 255 && original.frac != 0) {
    result.sign = 0;
    result.exp  = 255;
    result.frac = FRAC_MAX; // NaN -> NaN
  } else if (original.uint32 == ZERO) {
    result.uint32 = INF;
  } else if (original.uint32 == NZERO) {
    result.uint32 = NINF;
  } else if (original.uint32 == INF) {
    result.uint32 = ZERO;
  } else if (original.uint32 == NINF) {
    result.uint32 = NZERO;
  } else {


    if (original.frac == 0) {
      result.uint32 = org;
      if (original.exp >= 127) {
	d = original.exp - 127;
	result.exp = 127 - d;
      } else {
	d = 127 - original.exp;
	result.exp = 127 + d;
      }
    } else {
      fraction.uint32 = org;
      fraction.exp = 127;

      c = 1.0 / MAX;
      index = (fraction.frac & MASK11) >> 12;
      t = 1 + c * index;
      a_db = make_a(t,c) * (-1);
      b_db = make_b(t,c);
      a_fl = (float)a_db;
      b_fl = (float)b_db;
      a.fl32 = a_fl;
      b.fl32 = b_fl;
      fraction.uint32 = fadd(fmul(a.uint32, fraction.uint32), b.uint32);
    
      result.sign = original.sign;
      if (original.exp >= 127) {
	d = original.exp - 127;
	result.exp = 127 - d - 1;
      } else {
	d = 127 - original.exp;
	result.exp = 127 + d - 1;
      }
      result.frac = fraction.frac;
    }
  }
  return (result.uint32);
}
