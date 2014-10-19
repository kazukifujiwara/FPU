#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
fadd.c, fmul.cなど他の関数と一緒にコンパイルする。
main関数を使用しない場合は不要。
*/


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
