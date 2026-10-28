#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

static char dsc[6][34];
static unsigned data[34];
static FILE *output;

static const char *hello[] =
{
  "#  # #### #    #    ####",
  "#  # #    #    #    #  #",
  "#### #    #    #    #  #",
  "#  # ###  #    #    #  #",
  "#  # #    #  # #  # #  #",
  "#  # #### #### #### ####"
};

static void spaces(int n)
{
  while (n-- > 0)
    putchar(' ');
}

static void wave(double x)
{
  int i, j, k;
  memset(dsc, ' ', sizeof dsc);
  for (i = 0; i < 6; i++) {
    k = round(5 + 5*sin(x + 0.1*i));
    for (j = 0; j < 24; j++)
      dsc[i][j + k] = hello[i][j];
  }
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 34; j++)
      printf("%c ", dsc[i][j]);
    putchar('\n');
  }
  memset(data, 0, sizeof data);
  for (j = 0; j < 34; j++) {
    for (i = 0; i < 6; i++) {
      data[j] <<= 1;
      data[j] |= dsc[i][j] & 1;
    }
  }
  for (j = 0; j < 34; j += 2)
    fprintf(output, "%02o%02o\n", data[j + 1], data[j]);
  fflush(stdout);
}

void main(void)
{
  const struct timespec ts = { 0, 20000000 };
  int x = 0;
  int i;

  output = fopen("DSC DATA", "w");

  for (;;) {
    printf("\e[2J\e[H%03d\n", x);
    wave(2 * M_PI * x / 80.0);
    if (++x >= 80) {
      x = 0;
      fclose(output);
      output = fopen("/dev/null", "w");
    }
    nanosleep(&ts, NULL);
  }
}
