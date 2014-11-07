#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "def.h"


//最近接偶数への丸め(round to the nearest even) -- 注：26bit -> 23bit
unsigned int round_even_26bit(unsigned int num) {
  int right4;
  right4 = num & 15;
  if ((4 < right4 && right4 < 8) || 11 < right4) 
    num = (num >> 3) + 1;
  else
    num = num >> 3;
  return (num);
}

//最近接偶数丸めにより仮数部がオーバーフローしてしまうときに'1'を返す。 注意: 26bit
//"11111111111111111111111100" ～ "11111111111111111111111111" のとき。
int round_even_carry_26bit(unsigned int num) {
  if (0x3fffffcu <= num && num <= 0x3ffffffu)
    return 1;
  else
    return 0;
}

uint32_t bit_invert(uint32_t a) {
  uint32_t result = 0;
  uint32_t temp;
  int flag; // 0 or 1;
  int nflag; // 反転させたもの
  int i;
  for (i = 0; i < 32; i++) {
    temp = a >> (31 - i);
    flag = temp & 1;
    nflag = 1 - flag;
    result = result + (nflag << (31 - i));
  }
  return (result);
}

uint32_t itof(uint32_t a) {
  
  union data_32bit a_32bit, result;
  int flag; // 符号部
  int i; // 上からiビット目で始めて'1'が登場する
  unsigned int frac;
  unsigned int frac_grs;
  int s_bit; //sticky bit
  uint32_t temp;

  a_32bit.uint32 = a;
  flag = a_32bit.sign;
  
  if (a == 0) {
    result.uint32 = ZERO;
  } else {
    
    if (flag == 0) {
      temp = a;
    } else {
      temp = bit_invert(a-1);
    }

    i = 30;
    while (((temp >> i) & 1) == 0) {
      i--;
    } // i = 0~30 となるはず
    
    result.sign = flag;
    result.exp  = 127 + i;
    
    if (i < 24) {
      frac = temp & FRAC_MAX;
      frac = frac << (23 - i);  // 23 - (i - 1) ... shitr(0埋め)
      result.frac = frac;
    } else {
      if (i == 24) {
	frac_grs = (temp & FRAC_MAX) << 3;
      } else if (i == 25) {
	frac_grs = (temp & FRAC_MAX) << 2;
      } else if (i == 26) {
	frac_grs = (temp & FRAC_MAX) << 1;
      } else if (i == 27) {
	frac_grs = temp & FRAC_MAX;
      } else {
	s_bit = or_nbit(temp, (i-25));
	frac_grs = (temp << (32 - i)) >> 6;
	frac_grs = frac_grs + s_bit;
      }
      result.frac = round_even_26bit(frac_grs);
      if (round_even_carry_26bit(frac_grs) == 1) {
	result.exp++;   // int -> float なので inf (or -inf) になる心配はない
      }
    }
  }
  return (result.uint32);
}
