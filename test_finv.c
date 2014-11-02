/*
  コンパイル時コマンド
  旧: gcc test_finv.c print.c -lm
  新: gcc test_finv.c fmul.c fadd.c print.c def.c -lm
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "def.h"
#include "finv.c"

void print_data(union data_32bit data);

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


//一致ならば1,不一致ならば0,1bitずれならば2を返す
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
      if (diff == 1 || diff == -1)
	return 2; //誤差の範囲が1bit以内の場合
      else
	return 0;
    }
  }
}

int main(void)
{
  FILE *fp;
  union data_32bit a, n, result, correct;
  uint32_t a_uint32;
  int count_mistake = 0; //誤答数をカウント
  int count_1bit_diff = 0; //1bitずれをカウント

  if ((fp = fopen("testcase.txt", "r")) == NULL) {
    printf("file open error.\n");
    exit(EXIT_FAILURE);
  }

  while(fscanf(fp, "%8x", &a.uint32) != EOF) {
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
  }

  printf("total 1bit_diff : %d\n", count_1bit_diff);
  printf("total mistakes  : %d\n", count_mistake);

  fclose(fp);
  
  return 0;
}
