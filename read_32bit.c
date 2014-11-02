#include <stdio.h>
#include <string.h>
#include <stdint.h>

unsigned int str_to_uint32(char *str) {
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


/*
int main(void) {

  char a[33], b[33];
  memset(a, '\0', 33);
  memset(b, '\0', 33);
  
  int i;
  for (i = 0; i < 32; i++) {
    putchar('-');
  }
  putchar('\n');

  while(scanf("%s", a) != EOF) {
    scanf("%s", b);
    printf("a:"); str_to_uint32(a); putchar('\n');
    printf("b:"); str_to_uint32(b); putchar('\n');
    memset(a, '\0', 33);
    memset(b, '\0', 33);
  }
  
  return 0;
}
*/
