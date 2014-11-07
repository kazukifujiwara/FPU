#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "def.h"
#include "print.h"

uint32_t itof(uint32_t a);

int main(void) {
  
  union data_32bit a;
  union data_32bit result;

  int select_flag;
  char select_buf[10];
  int a_int;
  
  printf("int : "); scanf("%d", &a_int);
    
  printf("\n");

  a.uint32 = a_int;
  
  printf("-- int --\n");
  print_data(a);
  
  printf("\n");

  result.uint32 = itof(a_int);
  
  printf("-- itof --\n");
  print_data(result);

  printf("\n");

  union data_32bit test;

  printf("-- correct answer --\n");
  test.fl32 = (float)a_int;
  print_data(test);

  return(0);
}
