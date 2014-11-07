#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "def.h"

uint32_t
rand32()
{
	uint32_t x;

	x = rand() & 0xff;
	x |= (rand() & 0xff) << 8;
	x |= (rand() & 0xff) << 16;
	x |= (rand() & 0xff) << 24;

	return x;
}

int
main()
{
	union data_32bit a, b, s;

	srand((unsigned) time(NULL));

	for (int i=0; i < 1<<20; i++)
	{
		a.uint32 = rand32();
		b.uint32 = rand32();
		s.uint32 = (a.fl32 > b.fl32)?1:0;

		printf("%08X %08X %08X\n", a.uint32, b.uint32, s.uint32);
	}

	return 0;
}
