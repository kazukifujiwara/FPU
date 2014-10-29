#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "def.h"

uint32_t fadd(uint32_t a, uint32_t b);

uint32_t fneg(uint32_t a);

uint32_t fsub(uint32_t a, uint32_t b) {
  return fadd(a, fneg(b));
}
