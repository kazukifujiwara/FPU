#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "futil.h"

/* デバッグ用　ビット列を表示 */
void print_32bit(uint32_t n);

void print_data(union data_32bit data);

char *delete_space(char *str);

uint32_t str_to_uint32(char *str);

uint32_t fadd(uint32_t a, uint32_t b) {

  union data_32bit a_32bit, b_32bit, sum;
  union data_32bit big, small;
  int diff;
  int i;
  unsigned s_bit;  //sticky bit
  unsigned int temp;
  unsigned int big_mant, small_mant, sum_mant;
  int compare_flag = 1;    // aとbの絶対値を比較
  // a == b --> 0, a != b --> 1

  a_32bit.uint32 = a;
  b_32bit.uint32 = b;

  if ((a_32bit.exp == 255 && a_32bit.frac != 0) ||
      (b_32bit.exp == 255 && b_32bit.frac != 0)) { // いずれかがNaNの場合
    sum.sign = 0;
    sum.exp  = 255;
    sum.frac = FRAC_MAX;   // NaN
  } else if (a_32bit.exp == 255 && b_32bit.exp == 255) {
    if (a_32bit.sign != b_32bit.sign) {
      sum.sign = 0;
      sum.exp  = 255;
      sum.frac = FRAC_MAX; // NaN
    } else {
      if (a_32bit.sign == 0)
	sum.uint32 = INF;
      else
	sum.uint32 = NINF; // inf or -inf
    }
  } else if (a_32bit.exp == 255) {
    if (a_32bit.sign == 0)
      sum.uint32 = INF;
    else
      sum.uint32 = NINF; // inf or -inf
  } else if (b_32bit.exp == 255) {
    if (b_32bit.sign == 0)
      sum.uint32 = INF;
    else
      sum.uint32 = NINF; // inf or -inf
  } 
  /*
  else if (a_32bit.uint32 == NZERO && b_32bit.uint32 == NZERO)
    sum.uint32 = NZERO;
  else if (a_32bit.uint32 == ZERO && b_32bit.uint32 == NZERO)
    sum.uint32 = ZERO;
  else if (a_32bit.uint32 == NZERO && b_32bit.uint32 == ZERO)
    sum.uint32 = ZERO;
  else if (a_32bit.uint32 == ZERO || a_32bit.uint32 == NZERO)
    sum.uint32 = b_32bit.uint32;
  else if (b_32bit.uint32 == ZERO || b_32bit.uint32 == NZERO)
    sum.uint32 = a_32bit.uint32;
  */
  
  /* 非正規化数を0として計算 */
  else if (a_32bit.exp == 0 && a_32bit.frac == 0) {
    if (b_32bit.exp == 0 && b_32bit.frac == 0) {
      sum.uint32 = 0;
      if (a_32bit.sign == 1 && b_32bit.sign == 1)
	sum.sign = 1;
    } else
      sum.uint32 = b_32bit.uint32;
  } else if (a_32bit.exp == 0)
    sum.uint32 = b_32bit.uint32;
  else if (b_32bit.exp == 0)
    sum.uint32 = a_32bit.uint32;
  
  else {
    
    if (a_32bit.exp > b_32bit.exp) {
      big.uint32   = a_32bit.uint32;
      small.uint32 = b_32bit.uint32;
    } else if (a_32bit.exp < b_32bit.exp) {
      big.uint32   = b_32bit.uint32;
      small.uint32 = a_32bit.uint32;
    } else if (a_32bit.exp == b_32bit.exp) {
      if (a_32bit.frac > b_32bit.frac) {
	big.uint32   = a_32bit.uint32;
	small.uint32 = b_32bit.uint32;
      } else if (a_32bit.frac < b_32bit.frac) {
	big.uint32   = b_32bit.uint32;
	small.uint32 = a_32bit.uint32;
      } else if (a_32bit.frac == b_32bit.frac) { // 絶対値が一致した場合
	big.uint32   = a_32bit.uint32;
	small.uint32 = b_32bit.uint32;
	compare_flag = 0;
      }
    }
    
    diff = big.exp - small.exp;

    if (diff > 25)
      sum.uint32 = big.uint32;

    else {
      
      	big_mant   = big.frac | (1 << 23);
	small_mant = small.frac | (1 << 23);
  	
	big_mant = big_mant << 3;
	if (diff < 4) {
	  small_mant = small_mant << (3 - diff);
	} else {
	  s_bit = or_nbit(small_mant, diff - 2);
	  small_mant = small_mant >> (diff - 2);
	  small_mant = (small_mant << 1) | s_bit;
	}
	
	if (a_32bit.sign == b_32bit.sign) {   // 同符号の場合
	
	  sum.sign = big.sign;
	  sum_mant = big_mant + small_mant;

	  if ((sum_mant >> 27) > 0) { // 繰り上がりあり
	    sum.exp = big.exp + 1;
	    s_bit = or_nbit(sum_mant, 2);
	    sum_mant = sum_mant >> 2;
	    sum_mant = (sum_mant << 1) | s_bit;
	    sum.frac = round_even(sum_mant) & FRAC_MAX;
	    if (sum.exp >= 255) {
	      sum.exp = 255;
	      sum.frac = 0;
	    }
	  } else { // 繰り上がり無し
	    sum.exp = big.exp;
	    if ((round_even(sum_mant) >> 24) > 0) {
	      sum.exp++;
	      if (sum.exp == 255)
		sum.frac = 0;
	    } else
	      sum.frac = round_even(sum_mant) & FRAC_MAX;
	  }

	} else {                              // 異符号の場合

	  if (compare_flag == 0) {            // 絶対値が等しい場合
	    sum.sign = 0;
	    sum.exp  = 0;
	    sum.frac = 0;
	    compare_flag = 0;
	  } else {                            // 絶対値が異なる場合
	    
	    sum.sign = big.sign;

	    temp = big_mant - small_mant;

	    //上から何bit目に初めて1が現れるか(0~26)
	    i = 0;
	    while ((temp >> (26 - i)) == 0) {
	      i++;
	      if (i == 27)
		break; // i == 27 の場合、絶対値が等しいので処理済み
	    }
	
	    if (big.exp > i) {
	      if (i < 27) {
		sum.exp = big.exp - i;
		if (i < 4) { 
		  sum.frac = round_even(temp << i) & FRAC_MAX;
		  if (round_even_carry(temp << i) == 1) {
		    sum.exp++; //丸めによる仮数部の桁溢れの処理
		  }
		} else  
		  sum.frac = (temp & (FRAC_MAX)) << (i - 3) & FRAC_MAX;	
	      }
	    } else {
	      sum.exp = 0;
	      sum.frac = 0;
	    }
	  }
	}
    }
  }
  return (sum.uint32);
}
	         
#if 0
int main(void) {
  
  union data_32bit a,b;
  union data_32bit sum;

  int select_flag;
  char select_buf[10];
  printf("select import form :\n");
  printf("float -> 0\n");
  printf("32bit -> 1 (others)\n");
  gets(select_buf);
  sscanf(select_buf, "%d\n", &select_flag);

  if (select_flag == 0) {
    printf("a.fl32 : "); scanf("%f", &a.fl32);
    printf("b.fl32 : "); scanf("%f", &b.fl32);
  } else {
    char a_str[35], b_str[35];
    printf("詰めて入力しても可\n");
    printf("- --exp--- -------fraction--------\n");
    printf("a(32bit) :\n");
    gets(a_str);
    printf("b(32bit) :\n");
    gets(b_str);
    a.uint32 = str_to_uint32(delete_space(a_str));
    b.uint32 = str_to_uint32(delete_space(b_str));
  }
    
  printf("\n");

  printf("-- a --\n");
  print_data(a);
  
  printf("\n");

  printf("-- b --\n");
  print_data(b);

  printf("\n");

  sum.uint32 = fadd(a.uint32, b.uint32);
  
  printf("-- sum --\n");
  print_data(sum);

  printf("\n");

  union data_32bit test;

  printf(" -- correct answer --\n");
  test.fl32 = a.fl32 + b.fl32;
  print_data(test);

  return(0);
}

#endif
