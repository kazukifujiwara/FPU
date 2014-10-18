#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define FRAC_MAX 8388607 // 2^23
#define ZERO     0u
#define NZERO    2147483648u
#define INF      2139095040u
#define NINF     4286578688u


union data_32bit {
  struct {
    unsigned int frac : 23;
    unsigned int exp  : 8;
    unsigned int sign : 1;
  };
  float fl32;
  uint32_t uint32;
};

/* デバッグ用　long long int を指定されたbit数の列で表示。越える場合は越えて表示 */
void llitobit(long long int num, int max) {
  int i = 0;
  int j;
  long long int temp;
  temp = num;
  
  if (num != 0) {

    //最上位の桁を決定
    while (temp > 0) {
      temp = temp >> 1;
      i++;
    }
    
    while (max > i) {
      putchar('0');
      max--;
    }

    /* 1のままだと32bitになってしまうので、途中でループしてしまう可能性あり。
       1LLと表記しなければならない。*/
    for (j = 0; j < i; j++) {
      if ((num & ((1LL << (i - j - 1)))) == (1LL << (i - j - 1))) {
	putchar('1');
      } else {
	putchar('0');
      }
    }
  } else {
    for (i = 0; i < max; i++) {
      putchar('0');
    }
  }
  putchar('\n');
}


/* デバッグ用　ビット列を表示 */
void print_32bit(uint32_t n) {
  int i;
  int temp = n;
  for (i = 0; i < 32; i++) {
    printf("%u", (temp >> (31 - i)) & 1);
    if (i == 0 || i == 8) {
      printf(" "); // 1 + 8 + 23;
    }
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

// 下からnbitのORをとる 返り値0or1 
int or_nbit(unsigned int num, int n) {
  unsigned int all = (1 << n) - 1;
  unsigned int result = num & all;
  if (result > 0) 
    return 1;
  else
    return 0;
}

//最近接偶数への丸め(round to the nearest even) -- 注：27bit -> 24bit
unsigned int round_even(unsigned int num) {
  int right4;
  right4 = num & 15;
  if ((4 < right4 && right4 < 8) || 11 < right4) 
    num = (num >> 3) + 1;
  else
    num = num >> 3;
  return (num);
}

//最近接偶数丸めにより仮数部がオーバーフローしてしまうときに'1'を返す。
//"11111111111111111111100" ～ "11111111111111111111111111" のとき。
int round_even_carry(unsigned int num) {
  if (0x7fffffcu <= num && num <= 0x7ffffffu)
    return 1;
  else
    return 0;
}



uint32_t fmul(uint32_t a, uint32_t b) {

  union data_32bit a_32bit, b_32bit, result;
  long long int a_mant, b_mant;
  long long int a_hmant, a_lmant, b_hmant, b_lmant;
  long long int product; //仮数部の積
  int s_bit;   //sticky bit
  int exp;     //場合によっては途中で255を上回ることがあるので、最後にresult.expに代入する


  a_32bit.uint32 = a;
  b_32bit.uint32 = b;

  if ((a_32bit.exp == 255 && a_32bit.frac != 0) ||
      (b_32bit.exp == 255 && b_32bit.frac != 0)) {  //いずれかがNaNの場合
    result.sign = 0;
    result.exp  = 255;
    result.frac = FRAC_MAX;  // NaN
  } else if ((a_32bit.exp == 255) || (b_32bit.exp == 255)) {
    result.sign = a_32bit.sign ^ b_32bit.sign;
    result.exp  = 255;
    result.frac = 0; // inf or -inf
  } else if ((a_32bit.exp == 0) || (b_32bit.exp == 0)) {
    result.sign = a_32bit.sign ^ b_32bit.sign;
    result.exp  = 0;
    result.frac = 0; // どちらか一方が０ならば全て０に。(非正規化数も全て０に。)
  } else {
    
    result.sign = a_32bit.sign ^ b_32bit.sign;
    exp  = a_32bit.exp + b_32bit.exp - 127; // + 129 ??

    a_mant = (1 << 23) | a_32bit.frac;
    b_mant = (1 << 23) | b_32bit.frac;
    a_hmant = a_mant >> 10; //上位14桁を取り出す
    b_hmant = b_mant >> 10;
    a_lmant = a_mant & ((1 << 10) - 1); //下位10桁を取り出す
    b_lmant = b_mant & ((1 << 10) - 1);  
  
    /*
    printf("a_mant  : "); printf("%10llu ", a_mant);  llitobit(a_mant, 25); 
    printf("b_mant  : "); printf("%10llu ", b_mant);  llitobit(b_mant, 25);
    printf("a_hmant : "); printf("%10llu ", a_hmant); llitobit(a_hmant, 14);
    printf("a_lmant : "); printf("%10llu ", a_lmant); llitobit(a_lmant, 10);
    printf("b_hmant : "); printf("%10llu ", b_hmant); llitobit(b_hmant, 14);
    printf("b_lmant : "); printf("%10llu ", b_lmant); llitobit(b_lmant, 10);
    */

    product = (a_hmant * b_hmant) + ((a_hmant * b_lmant) >> 10)
      + ((a_lmant * b_hmant) >> 10); 
    /*
    printf("a_hmant * b_hmant : %10llu\n", a_hmant * b_hmant);
    printf("a_hmant * b_lmant : %10llu\n", a_hmant * b_lmant);
    printf("a_lmant * b_hmant : %10llu\n", a_lmant * b_hmant);
    printf("product : "); printf("%10llu \n", product); 
    llitobit(product, 28);
    */
  
    //仮数部の積のMSBは下から27bit目or28bit目(1,2,..,25,26)
    if ((product & (1 << 27)) == (1 << 27)) { //繰り上がりあり
      //llitobit(FRAC_MAX << 4, 28);
      //printf("--- 1 ---\n"); //debug

      s_bit = or_nbit(product, 2);
      product = product >> 2;
      product = (product << 1) | s_bit;
      result.frac = round_even(product) & FRAC_MAX;
      if (round_even_carry(product) == 1) {
	exp++;
      }
      //result.frac = (product & (FRAC_MAX << 4)) >> 4;
      exp++;
    } else { //繰り上がり無し
      //llitobit(FRAC_MAX << 3, 28);
      //printf("--- 0 ---\n"); //debug

      result.frac = round_even(product) & FRAC_MAX;
      if (round_even_carry(product) == 1) {
	exp++;
      }
      //result.frac = (product & (FRAC_MAX << 3)) >> 3;
    }
    if (exp > 254) {
      result.exp  = 255;
      result.frac = 0;   // inf or -inf
    } else if (exp <= 0) {
      result.exp = 0;
    } else {
      result.exp = exp;
    }
  }
  return (result.uint32);
}

char *delete_space(char *str) {
  char *new = calloc(33, sizeof(char));
  int i = 0;
  int j = 0;
  while (str[i] != '\0') {
    if (str[i] != ' ') {
      new[j] = str[i];
      i++;
      j++;
    } else
      i++;
  }
  j++;
  new[j] = '\0';

  return (new);
}

uint32_t str_to_uint32(char *str) {
  int i;
  uint32_t sum = 0;
  for (i = 0; i < 32; i++) {
    if (str[i] == '1')
      sum = sum + (1 << (31 - i));
  }
  return (sum);
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

  sum.uint32 = fmul(a.uint32, b.uint32);
  
  printf("-- product --\n");
  print_data(sum);

  printf("\n");

  union data_32bit test;

  printf(" -- correct answer --\n");
  test.fl32 = a.fl32 * b.fl32;
  print_data(test);

  return(0);
}

#endif
