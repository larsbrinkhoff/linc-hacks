#include <stdio.h>
#include <stdlib.h>

unsigned divide_12_bits_by_10(unsigned x)
{
   unsigned a, shift = 11, z;
   a = x * (((1 << shift) + 9) / 10);
   z = a & 03777;
   a >>= shift;
   z -= x >> 3;
   if (z & 04000)
     a--;
   return a;
}

int main(int argc, char **argv)
{
   unsigned x[] = { 0, 9, 10, 99, 100, 999, 1000, 2029, 2030, 2039, 2040, 2047 };
   int i;
   for (i = 0; i < sizeof x / sizeof x[0]; i++)
      printf("%u / 10 = %u\n", x[i], divide_12_bits_by_10(x[i]));
   return 0;
}
