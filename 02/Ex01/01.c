#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Include that allows to print result as an image
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Macro functions
#define SQR(x) ((x) * (x))

// Default size of image
#define X 1280
#define Y 720
#define MAX_ITER 10000
#define BAILOUT_VALUE SQR(2)

void calc_mandelbrot(uint8_t image[Y][X]) {
	for(uint32_t i_y = 0; i_y < Y; i_y++) {
		for(uint32_t i_x = 0; i_x < X; i_x++) {

			double x = 0, y = 0;
			double cx = i_x / (double)X * (1 + 2.5) - 2.5;
			double cy = i_y / (double)Y * (1 + 1.0) - 1;

			uint32_t iteration;
			for(iteration = 0; iteration < MAX_ITER && SQR(x) + SQR(y) <= BAILOUT_VALUE;
			    iteration++) {
				double x_temp = SQR(x) - SQR(y) + cx;
				y = 2 * x * y + cy;
				x = x_temp;
			}
			double norm_iteration = iteration / (double)MAX_ITER * 255;
			image[i_y][i_x] = norm_iteration;
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