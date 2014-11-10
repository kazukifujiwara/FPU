/*
  コンパイル時コマンド
  旧: gcc test_finv.c print.c -lm
  新: gcc test_finv.c finv.c fmul.c fadd.c print.c def.c -lm
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "print.h"

#define PERMIT 4  //誤差何ulpまで許容か

uint32_t finv(uint32_t org);

uint32_t str_to_uint32t(char *str) {
  int i;
  uint32_t result = 0;
  
  for (i = 0; i < 32; i++) {
    if (str[i] == '1') {
      result += (1 << (31 - i));
    }
  }
  //printf("%u\n", result); //debug
  return (result);
}

void show_testcase(union data_32bit a,
		   union data_32bit result, union data_32bit correct) {
  printf("-- a --\n");
  print_data(a);
  putchar('\n');
  printf("-- result --\n");
  print_data(result);
  putchar('\n');
  printf("-- correct --\n");
  print_data(correct);
  putchar('\n');
  printf("-------------------------------------------------\n");
}

//非正規化数を全て０にする
uint32_t normalize(uint32_t a) {
  union data_32bit temp;
  temp.uint32 = a;
  if (temp.exp == 0) {
    temp.frac = 0;
  }
  return (temp.uint32);
}


//一致ならば1,不一致ならば0,2bitずれ以内ならば2を返す
int equal(uint32_t a_uint32, uint32_t b_uint32) {
  union data_32bit a, b;
  int diff;
  a.uint32 = a_uint32;
  b.uint32 = b_uint32;
  if ((a.exp == 255) && (a.frac != 0)) { // aがNaNの場合
    if ((b.exp == 255) && (b.frac != 0)) {
      return 1; //NaNは全て同一視
    } else {
      return 0;
    }
  } else if (a.exp == 0) {
    if (b.exp == 0) {
      if (a.sign == b.sign) {
	return 1;
      } else {
	return 0;
      }
    } else {
      return 0;
    }
  } else {
    if (a.uint32 == b.uint32) {
      return 1;
    } else {
      diff = a.uint32 - b.uint32;
      if ((PERMIT * (-1)) <= diff && diff <= PERMIT)
	return 2; //誤差の範囲が(PERMIT)bit以内の場合
      else
	return 0;
    }
  }
}

int main(void)
{
  FILE *fp;
  union data_32bit a, n, result, correct;
  char a_str[33];
  int count_mistake = 0; //誤答数をカウント
  int count_1bit_diff = 0; //1bitずれをカウント

  memset(a_str, '\0', 33);

  if ((fp = fopen("testcase.txt", "r")) == NULL) {
    printf("file open error.\n");
    exit(EXIT_FAILURE);
  }

  while(fscanf(fp, "%s", a_str) != EOF) {
    a.uint32 = str_to_uint32t(a_str);
    n.uint32 = normalize(a.uint32);
    result.uint32 = finv(a.uint32);
    correct.fl32 = 1.0 / n.fl32;  //非正規仮数を0に潰す

    if (n.uint32 != a.uint32 && count_mistake <= 10) {
      //printf("normalized!!!!!\n"); //debug
    }

    if (equal(result.uint32, correct.uint32) == 1) {
      //何もしない
    } else if (equal(result.uint32, correct.uint32) == 2) {
      count_1bit_diff++;
      //show_testcase(a, result, correct);
    } else {
      //printf("0\n");  //一致しなければ0
      count_mistake++;
      if (count_mistake <= 5) {
	show_testcase(a, result, correct);
      }
      if (count_mistake == 5) {
	printf("more than 5 mistakes.\n");
      }
    }
    memset(a_str, '\0', 33);
  }

  printf("total ~%dbit_diff : %d\n", PERMIT, count_1bit_diff);
  printf("total mistakes  : %d\n", count_mistake);

  fclose(fp);
  
  return 0;
}
