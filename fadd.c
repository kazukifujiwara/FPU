#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define FRAC_MAX 8388607

union data_32bit {
  struct {
    unsigned int frac : 23;
    unsigned int exp  : 8;
    unsigned int sign : 1;
  };
  float fl32;
  uint32_t uint32;
};


/* デバッグ用 ビット列を回転 */
uint32_t lrotate(uint32_t n) {
  return ((n << 1)+(n >> 31));
}

/* デバッグ用　ビット列を表示 */
void print_32bit(uint32_t n) { 
  int i;
  int temp = n;
  //uint32_t one = 1;          //下の変更で異常がなければ不要
  for (i = 0; i < 32; i++) {
    temp = lrotate(temp);
    printf("%u", temp & 1);  //変更
    if (i == 0 || i == 8) printf(" ");
  }
  printf("\n");
}


void print_data(union data_32bit data) {
  printf("float  : %f\n", data.fl32);
  printf("uint32 : %u\n", data.uint32);
  printf("sign   : %u\n", data.sign);
  printf("exp    : %u\n", data.exp);
  printf("frac   : %u\n", data.frac);
  printf("32bit  : "); print_32bit(data.uint32);
}


uint32_t fadd(uint32_t a, uint32_t b) {

  union data_32bit a_32bit, b_32bit, sum;
  union data_32bit big, small;
  int diff;
  int i;

  unsigned int temp;
  unsigned int sum_frac;
  unsigned int big_frac, small_frac;
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
	sum.sign = 0;
      else
	sum.sign = 1;
      sum.exp  = 255;
      sum.frac = 0;     // inf or -inf
    }
  } else if (a_32bit.exp == 255) {
    sum.sign = a_32bit.sign;
    sum.exp  = 255;
    sum.frac = 0;       // inf or -inf
  } else if (b_32bit.exp == 255) {
    sum.sign = b_32bit.sign;
    sum.exp  = 255;
    sum.frac = 0;       // inf or -inf
  } else if (a_32bit.exp == 0 && a_32bit.frac == 0) {
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
      if (a_32bit.sign == b_32bit.sign) {   // 同符号の場合
      
	sum.uint32 = big.uint32;   // ?
       
	big_frac   = big.frac | (1 << 23);
	small_frac = small.frac | (1 << 23);
	small_frac = small_frac >> diff;
	sum_frac = big_frac + small_frac;

	// (sum_frac >> 24) > 0  : 下から25bit目が1であるとき
	if (big.exp == 254 && (sum_frac >> 24) > 0) {
	  sum.exp  = 255;
	  sum.frac = 0;     // 繰り上がりによるinf
	} else {
	  if ((sum_frac >> 24) > 0) {  // 繰り上がりのある場合
	    sum.exp = big.exp + 1;
	    sum.frac = (sum_frac >> 1) & FRAC_MAX;
	  } else {     // 繰り上がりのない場合
	    sum.exp = big.exp;
	    sum.frac = sum_frac & FRAC_MAX;
	  }
	}
      } else {                              // 異符号の場合

	if (compare_flag == 0) {            // 絶対値が等しい場合
	  sum.sign = 0;
	  sum.exp  = 0;
	  sum.frac = 0;
	  compare_flag = 0;
	} else {                            // 絶対値が異なる場合
	
	  sum.sign = big.sign;
	  diff = big.exp - small.exp;
		  
	  big_frac   = big.frac | (1 << 23);
	  small_frac = small.frac | (1 << 23);

	  big_frac   = big_frac << 2;
	  small_frac = small_frac << 2;

	  small_frac = small_frac >> diff;
	  temp = big_frac - small_frac;
	  //int left = ((small.frac | (1 << 23)) & ((1 << diff) - 1));
	  // smallの(1.frac)のうち無視される部分(diff桁分)
	
	  i = 0;
	  while ((temp >> (25 - i)) == 0) { //
	    i++;
	    if (i == 26) //
	      break;
	  }
	  // i == 24ならばここまで着ていないはず。
	
	  if (big.exp > i) {
	    if (i < 26) { //
	      sum.exp = big.exp - i;
	      if (i < 2)
	        sum.frac = temp >> (2 - i);
	      else
	        sum.frac = (temp & FRAC_MAX) << (i - 2); //
	    }
	  } else {
	    sum.uint32 = 0;
	  }
	}
      }
    }
  }
  return (sum.uint32);
}
      

uint32_t power(int n) {
  uint32_t s = 1;
  while (n > 0) {
    s = s * 2;
    n--;
  }
  return (s);
}

uint32_t str_to_uint32(char *str) {
  int i;
  uint32_t sum = 0;
  for (i = 0; i < 32; i++) {
    if (str[i] == '1')
      sum = sum + power(31-i);
  }
  return (sum);
}
	         
int main(void) {
  
  union data_32bit a,b,c;
  union data_32bit sum;

  int select_flag;
  printf("select import form :\n");
  printf("float -> 0\n");
  printf("32bit -> 1 (others)\n");
  scanf("%d", &select_flag);

  if (select_flag == 0) {
    printf("a.fl32 : "); scanf("%f", &a.fl32);
    printf("b.fl32 : "); scanf("%f", &b.fl32);
  } else {
    char a_str[33], b_str[33];
    printf("--------------------------------\n");
    printf("a(32bit) :\n"); scanf("%s", a_str);
    printf("b(32bit) :\n"); scanf("%s", b_str);
    a.uint32 = str_to_uint32(a_str);
    b.uint32 = str_to_uint32(b_str);
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


/* bit列入力用 */
/*


int main(void) {
  union data_32bit a, b;
  union data_32bit sum;
  char a_str[33], b_str[33];
 
  printf("--------------------------------\n");
  printf("a(32bit) :\n"); scanf("%s", a_str);
  printf("b(32bit) :\n"); scanf("%s", b_str);
  
  a.uint32 = str_to_uint32(a_str);
  b.uint32 = str_to_uint32(b_str);

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

  return 0;
}
*/
