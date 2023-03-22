
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Include that allows to print result as an image
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Default size of image
#define X 1280
#define Y 720
#define MAX_ITER 10000

#define MAX_BRIGHTNESS 255
#define BAILOUT_VALUE (2 * 2)

void calc_mandelbrot(uint8_t image[Y][X]) {
  for (uint i = 0; i < Y; i++) {
    for (uint j = 0; j < X; j++) {
      double cx = (((2 * (double)j) / X) - 1) * (X / Y),
             cy = ((2 * (double)i) / Y) - 1;
      uint iteration = 0;
      double x = 0, y = 0;

      for (; (iteration < MAX_ITER) && ((x * x + y * y) <= BAILOUT_VALUE);
           iteration++) {
        double x_tmp = x * x - y * y + cx;
        y = 2 * x * y + cy;
        x = x_tmp;
      }

      image[i][j] = (iteration * MAX_BRIGHTNESS) / MAX_ITER;
    }
  }
}

int main() {
  uint8_t image[Y][X];

  calc_mandelbrot(image);

  const int channel_nr = 1, stride_bytes = 0;
  stbi_write_png("mandelbrot.png", X, Y, channel_nr, image, stride_bytes);
  return EXIT_SUCCESS;
}
