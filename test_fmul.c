#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

union data_32bit {
  struct {
    unsigned int frac : 23;
    unsigned int exp  : 8;
    unsigned int sign : 1;
  };
  float fl32;
  uint32_t uint32;
};

//test
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

uint32_t fmul(uint32_t a, uint32_t b);

void print_data(union data_32bit data);

void show_testcase(union data_32bit a, union data_32bit b,
		   union data_32bit result, union data_32bit correct) {
  printf("-- a --\n");
  print_data(a);
  putchar('\n');
  printf("-- b --\n");
  print_data(b);
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
  union data_32bit a, b, result, correct;
  uint32_t a_uint32, b_uint32;
  char a_str[33], b_str[33];
  int count_mistake = 0; //誤答数をカウント
  int count_1bit_diff = 0; //1bitずれをカウント

  memset(a_str, '\0', 33);
  memset(b_str, '\0', 33);
  
  if ((fp = fopen("testcase.txt", "r")) == NULL) {
    printf("file open error.\n");
    exit(EXIT_FAILURE);
  }

  while(fscanf(fp, "%s", a_str) != EOF) {
    fscanf(fp, "%s", b_str);
    a_uint32 = str_to_uint32t(a_str);
    b_uint32 = str_to_uint32t(b_str);
    a.uint32 = normalize(a_uint32);
    b.uint32 = normalize(b_uint32);
    //printf("a: %10u  ", a.uint32);
    //printf("b: %10u\n", b.uint32);
    result.uint32 = fmul(a.uint32, b.uint32);
    correct.fl32 = a.fl32 * b.fl32;
    //printf("result  : %10u\n", result.uint32);
    //printf("correct : %10u\n", correct.uint32);
    if (equal(result.uint32, correct.uint32) == 1) {
    } else if (equal(result.uint32, correct.uint32) == 2) {
      count_1bit_diff++;
    } else {
      //printf("0\n");  //一致しなければ0
      count_mistake++;
      show_testcase(a, b, result, correct);
    }
    memset(a_str, '\0', 33);
    memset(b_str, '\0', 33);
  }

  printf("total 1bit_diff : %d\n", count_1bit_diff);
  printf("total mistakes  : %d\n", count_mistake);

  fclose(fp);
  
  return 0;
}
