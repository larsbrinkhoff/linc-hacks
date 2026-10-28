#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* Print to stdout a sine table with n 12-bit words. */

static void usage(void)
{
  fprintf(stderr, "Usage: n amplitude\nNumbers in octal.\n");
  exit(1);
}

int main(int argc, char **argv)
{
  unsigned long n, amplitude;
  char *end;
  int i;

  if (argc != 3)
    usage();

  n = strtoul(argv[1], &end, 8);
  if (*end != 0)
    usage();

  amplitude = strtoul(argv[2], &end, 8);
  if (*end != 0)
    usage();

  for (i = 0; i < n; i++) {
    double x = 2 * M_PI * i / n;
    double y = (amplitude - .1) * sin(x);
    int z = round(y);
    if (z < 0)
      z = -z ^ 07777;
    printf("%04o\n", z);
  }

  return 0;
}
