#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MT.h"

#define MAX 4294967295 // 2^32-1
#define FILE_NAME "testcase.txt"

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

//NaNを生成
uint32_t make_nan() {
  union data_32bit temp;
  temp.uint32 = genrand_int32();
  temp.exp = 255; //指数部を255に書き換え
  return (temp.uint32);
}

//非正規化数を生成
uint32_t make_denormal() {
  union data_32bit temp;
  temp.uint32 = genrand_int32();
  temp.exp = 0; //指数部を0に書き換え
  return (temp.uint32);
}

//一方の引数を指定した場合のテストケースを生成＆ファイル出力
void fix_one(uint32_t a, FILE *fp) {
  uint32_t b;
  int i;
  b = ZERO;   fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
  b = NZERO;  fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
  b = INF;    fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
  b = NINF;   fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
  for (i = 0; i < 10; i++) {
    b = make_nan();
    fprintf(fp, "%08x %08x\n", a, b);
    fprintf(fp, "%08x %08x\n", b, a);
  }
  for (i = 0; i < 10; i++) {
    b = make_denormal();
    fprintf(fp, "%08x %08x\n", a, b);
    fprintf(fp, "%08x %08x\n", b, a);
  }
  for (i = 0; i < 500; i++) {
    b = genrand_int32();
    fprintf(fp, "%08x %08x\n", a, b);
    fprintf(fp, "%08x %08x\n", b, a);
  }
}


int main(void) {
  int i;
  uint32_t a, b;
  FILE *fp;

  if ((fp = fopen(FILE_NAME, "w")) == NULL) {
    printf("file open error.\n");
    exit(EXIT_FAILURE);
  }

  fix_one(ZERO, fp);
  fix_one(NZERO, fp);
  fix_one(INF, fp);
  fix_one(NINF, fp);
  
  //一方がNaNの場合
  for (i = 0; i < 50; i++) {
    a = make_nan();
    b = ZERO;   fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
    b = NZERO;  fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
    b = INF;    fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
    b = NINF;   fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
    for (i = 0; i < 10; i++) {
      b = make_nan();
      fprintf(fp, "%08x %08x\n", a, b);
      fprintf(fp, "%08x %08x\n", b, a);
    }
    for (i = 0; i < 10; i++) {
      b = make_denormal();
      fprintf(fp, "%08x %08x\n", a, b);
      fprintf(fp, "%08x %08x\n", b, a);
    } 
    for (i = 0; i < 50; i++) {
      b = genrand_int32();
      fprintf(fp, "%08x %08x\n", a, b);
      fprintf(fp, "%08x %08x\n", b, a);
    }
  }

  //一方が非正規仮数の場合
  for (i = 0; i < 50; i++) {
    a = make_denormal();
    b = ZERO;   fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
    b = NZERO;  fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
    b = INF;    fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
    b = NINF;   fprintf(fp, "%08x %08x\n", a, b); fprintf(fp, "%08x %08x\n", b, a);
    for (i = 0; i < 10; i++) {
      b = make_nan();
      fprintf(fp, "%08x %08x\n", a, b);
      fprintf(fp, "%08x %08x\n", b, a);
    }
    for (i = 0; i < 10; i++) {
      b = make_denormal();
      fprintf(fp, "%08x %08x\n", a, b);
      fprintf(fp, "%08x %08x\n", b, a);
    } 
    for (i = 0; i < 50; i++) {
      b = genrand_int32();
      fprintf(fp, "%08x %08x\n", a, b);
      fprintf(fp, "%08x %08x\n", b, a);
    }
  } 

  for (i = 0; i < 50000; i++) {
    a = genrand_int32();
    b = genrand_int32();
    fprintf(fp, "%08x %08x\n", a, b);
  };

  printf("success (> %s)\n", FILE_NAME);

  return 0;
}
