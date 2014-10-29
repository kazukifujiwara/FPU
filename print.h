#include <stdint.h>

#ifndef PRINT_H
#define PRINT_H

/* デバッグ用　long long int を指定されたbit数の列で表示。越える場合は越えて表示 */
void llitobit(long long int num, int max);

/* デバッグ用　ビット列を表示 */
void print_32bit(uint32_t n);

void print_data(union data_32bit data);

char *delete_space(char *str);

uint32_t str_to_uint32(char *str);

#endif
