/* 重複定義を避けるために関数定義の際にstaticをつける必要がある。 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef DEF_H
#define DEF_H

union data_32bit {
  struct {
    unsigned int frac : 23;
    unsigned int exp  : 8;
    unsigned int sign : 1;
  };
  float fl32;
  uint32_t uint32;
};


// 下からnbitのORをとる 返り値0or1 
static int or_nbit(unsigned int num, int n) {
  unsigned int all = (1 << n) - 1;
  unsigned int result = num & all;
  if (result > 0) 
    return 1;
  else
    return 0;
}

//最近接偶数への丸め(round to the nearest even) -- 注：27bit -> 24bit
static unsigned int round_even(unsigned int num) {
  int right4;
  right4 = num & 15;
  if ((4 < right4 && right4 < 8) || 11 < right4) 
    num = (num >> 3) + 1;
  else
    num = num >> 3;
  return (num);
}

//最近接偶数丸めにより仮数部がオーバーフローしてしまうときに'1'を返す。 注意: 27bit
//"111111111111111111111111100" ～ "111111111111111111111111111" のとき。
static int round_even_carry(unsigned int num) {
  if (0x7fffffcu <= num && num <= 0x7ffffffu)
    return 1;
  else
    return 0;
}

#endif
