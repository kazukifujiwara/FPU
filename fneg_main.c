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
void print_32bit(uint32_t n);

void print_data(union data_32bit data);

char *delete_space(char *str);

uint32_t str_to_uint32(char *str);

uint32_t fneg(uint32_t a);

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
