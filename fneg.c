#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

union data_32bit {
  struct {
    unsigned int frac : 23;
    unsigned int exp  : 8;
    unsigned int sign : 1;
  };
  float fl32;
  uint32_t uint32;
};

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

uint32_t fneg(uint32_t a) {
  union data_32bit a_32bit;
  a_32bit.uint32 = a;
  a_32bit.sign = ~(a_32bit.sign);
  return (a_32bit.uint32);
}

#if 0
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

int main(void) {
  
  union data_32bit a;
  union data_32bit neg;

  int select_flag;
  char select_buf[10];
  printf("select import form :\n");
  printf("float -> 0\n");
  printf("32bit -> 1 (others)\n");
  gets(select_buf);
  sscanf(select_buf, "%d\n", &select_flag);

  if (select_flag == 0) {
    printf("a.fl32 : "); scanf("%f", &a.fl32);
  } else {
    char a_str[35];
    printf("詰めて入力しても可\n");
    printf("- --exp--- -------fraction--------\n");
    printf("a(32bit) :\n");
    gets(a_str);
    a.uint32 = str_to_uint32(delete_space(a_str));
  }
    
  printf("\n");

  printf("-- a --\n");
  print_data(a);
  
  printf("\n");

  neg.uint32 = fneg(a.uint32);
  
  printf("-- neg --\n");
  print_data(neg);

  printf("\n");

  union data_32bit test;

  printf(" -- correct answer --\n");
  test.fl32 = a.fl32 * (-1);
  print_data(test);

  return(0);
}

#endif
