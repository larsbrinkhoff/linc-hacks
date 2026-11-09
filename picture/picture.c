#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h"

#define MAX_WIDTH (512/4)
#define MAX_HEIGHT (512/4)

static unsigned width, height;

static int pixel(unsigned char *data, int x, int y)
{
  return data[4 * (width * y + x)] != 0;
}

static void convert_dsc(unsigned char *data)
{
  unsigned word = 0, mask = 1;
  int i, j;
  for (j = 0; j < 2; j++) {
    for (i = 5; i >= 0; i--, mask <<= 1) {
      if (pixel(data, j, i))
        word |= mask;
    }
  }
  printf("%04o\n", word);
}

static void convert_row(unsigned char *data)
{
  int i;
  for (i = 0; i < width; i += 2)
    convert_dsc(data + i * 4);
}

int main(int argc, char *argv[])
{
  unsigned error;
  unsigned char* image = 0;
  int failed = 0;
  int y;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <file.png>\n", argv[0]);
    exit(1);
  }

  error = lodepng_decode32_file(&image, &width, &height, argv[1]);
  if (error) {
    fprintf(stderr, "error %u: %s\n", error, lodepng_error_text(error));
    exit(1);
  }

  if (width % 2) {
    fprintf(stderr, "Picture is %u pixels wide, but must be an even number of pixels.\n", width);
    failed = 1;
  }
  if (width > MAX_WIDTH) {
    fprintf(stderr, "Picture is %u pixels wide, but must be no more than %d pixels.\n", width, MAX_WIDTH);
    failed = 1;
  }
  if ((height % 8) != 6) {
    fprintf(stderr, "Picture is %u pixels high, but number of pixels modulo 8 must be 6.\n", height);
    failed = 1;
  }
  if (height > MAX_HEIGHT) {
    fprintf(stderr, "Picture is %u pixels high, but must be no more than %d pixels high.\n", height, MAX_HEIGHT);
    failed = 1;
  }
  if (failed)
    exit(1);

  printf("[%d WIDE\n", width / 2);
  printf("[%d HIGH\n", height / 8);

  for (y = 0; y < height; y += 8) {
    printf("[ROW %d\n", y / 8);
    convert_row(image + y * width * 4);
  }

  return 0;
}
