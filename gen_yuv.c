#include <stdio.h>

#include <limits.h>

/*
 * Generate a YUV Image in NV12 format
 *
 * For a 4x4 NV12 image:
 * Y0  Y1  Y2  Y3   U0 U2   V0 V2
 * Y4  Y5  Y6  Y7   U8 U10  V8 V10
 * Y8  Y9  Y10 Y11
 * Y12 Y13 Y14 Y15
 *
 * Will be stored as follow:
 * Y0  Y1  Y2  Y3
 * Y4  Y5  Y6  Y7
 * Y8  Y9  Y10 Y11
 * Y12 Y13 Y14 Y15
 * U0  V0  U2  V2
 * U8  V8  U10 V10
 */

#define WIDTH 128
#define HEIGHT 128 /* should be 2X */

int main(int argc, char *argv[]) {
	int num_y_pixel = WIDTH * HEIGHT;
	int num_c_pixel = WIDTH * HEIGHT / 4;
	int num_total_pixel = num_y_pixel + 2 * num_c_pixel;

	unsigned char y[num_y_pixel];
	unsigned char u[num_c_pixel];
	unsigned char v[num_c_pixel];
	unsigned char output[num_total_pixel];

	int i, j;
	int h_step = UCHAR_MAX / (WIDTH / 2 - 1);
	int v_step = UCHAR_MAX / (HEIGHT / 2 - 1);

	FILE *file;

	for (i = 0; i < num_y_pixel; i++) {
		// y[i] = ((i * (UCHAR_MAX + 1)) / num_y_pixel) % (UCHAR_MAX + 1);
		y[i] = 0;
	}
	// for (i = 0; i < num_c_pixel; i++) {
		// u[i] = ((i * (UCHAR_MAX + 1)) / num_c_pixel) % (UCHAR_MAX + 1);
		// v[i] = ((i * (UCHAR_MAX + 1)) / num_c_pixel) % (UCHAR_MAX + 1);
		// printf("u[%d]:%d v[%d]:%d\n", i, u[i], i, v[i]);
	// }

	/* U */
	for (i = 0; i < num_c_pixel; i++) {
		u[i] = ((i % (WIDTH / 2)) * h_step) % (UCHAR_MAX + 1);
	}
	for (i = 0; i < num_c_pixel; i++) {
		v[i] = ((i / (WIDTH / 2)) * v_step) % (UCHAR_MAX + 1);
	}

	for (i = 0; i < num_y_pixel; i++) {
		output[i] = y[i];
	}
	for (i = 0; i < 2 * num_c_pixel; i++) {
		if (i % 2 == 0) {
			output[i + num_y_pixel] = u[i / 2];
		} else {
			output[i + num_y_pixel] = v[i / 2];
		}
		// printf("output[%d]:%d\n", i + num_y_pixel, output[i + num_y_pixel]);
	}
	printf("output[256]:%d\n", output[256]);

	for (i = 0; i < num_total_pixel; i++) {
		if (i % WIDTH == 0) {
			printf("\n");
		}
		printf("%3d ", output[i]);
	}

	file = fopen(argv[1], "w");
	if (file == NULL) {
		perror("Error opening file");
		return 1;
	}
	fwrite(output, sizeof(char), num_total_pixel, file);
	fclose(file);

	printf("\nGood Bye\n");

	return 0;
}
