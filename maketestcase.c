#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MT.h"
#include "def.h"

#define MAX 4294967295 // 2^32-1
#define FILE_NAME "testcase.txt"

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

//2^32-1まで。32bitで返す
void output(uint32_t num, FILE *fp) {
  fprintf(fp, "%08x\n", num);
}

/*
void output(uint32_t num, FILE *fp) {
  int i;
  char *temp = calloc(33, sizeof(char));
  for (i = 0; i < 32; i++) {
    if (num % 2 == 1)
      temp[31-i] = '1';
    else
      temp[31-i] = '0';
    num = num / 2;
  }
  temp[32] = '\0';
  fprintf(fp, "%s\n", temp);
  free(temp);
}
*/

//一方の引数を指定した場合のテストケースを生成＆ファイル出力
void fix_one(uint32_t a, FILE *fp) {
  uint32_t b;
  int i;

  b = ZERO;   output(a,fp); output(b,fp); output(b,fp); output(a,fp);
  b = NZERO;  output(a,fp); output(b,fp); output(b,fp); output(a,fp);
  b = INF;    output(a,fp); output(b,fp); output(b,fp); output(a,fp);
  b = NINF;   output(a,fp); output(b,fp); output(b,fp); output(a,fp);
  for (i = 0; i < 10; i++) {
    b = make_nan();
    output(a,fp); output(b,fp); output(b,fp); output(a,fp);
  }
  for (i = 0; i < 10; i++) {
    b = make_denormal();
    output(a,fp); output(b,fp); output(b,fp); output(a,fp);
  }
  for (i = 0; i < 500; i++) {
    b = genrand_int32();
    output(a,fp); output(b,fp); output(b,fp); output(a,fp);
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
    b = ZERO;   output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    b = NZERO;  output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    b = INF;    output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    b = NINF;   output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    for (i = 0; i < 10; i++) {
      b = make_nan();
      output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    }
    for (i = 0; i < 10; i++) {
      b = make_denormal();
      output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    } 
    for (i = 0; i < 50; i++) {
      b = genrand_int32();
      output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    }
  }

  //一方が非正規仮数の場合
  for (i = 0; i < 50; i++) {
    a = make_denormal();
    b = ZERO;   output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    b = NZERO;  output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    b = INF;    output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    b = NINF;   output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    for (i = 0; i < 10; i++) {
      b = make_nan();
      output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    }
    for (i = 0; i < 10; i++) {
      b = make_denormal();
      output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    } 
    for (i = 0; i < 50; i++) {
      b = genrand_int32();
      output(a,fp); output(b,fp); output(b,fp); output(a,fp);
    }
  } 

  for (i = 0; i < 50000; i++) {
    a = genrand_int32();
    b = genrand_int32();
    output(a,fp); output(b,fp);
  };

  printf("success (> %s)\n", FILE_NAME);

  return 0;
}
