#include <stdio.h>

#include <limits.h>

/* there are 2x2 blocks */
#define BLOCK_WIDTH 256
#define BLOCK_HEIGHT 256 /* should be 2X */
#define BLOCK_NUM_H 3
#define BLOCK_NUM_V 3
#define TOTAL_WIDTH (BLOCK_WIDTH * BLOCK_NUM_H)
#define TOTAL_HEIGHT (BLOCK_HEIGHT * BLOCK_NUM_V)

int main(int argc, char *argv[]) {
	int num_y_pixel = TOTAL_WIDTH * TOTAL_HEIGHT;
	int num_c_pixel = num_y_pixel / 4;
	int num_total_pixel = num_y_pixel + 2 * num_c_pixel;

	unsigned char y[num_y_pixel];
	unsigned char u[num_c_pixel];
	unsigned char v[num_c_pixel];
	unsigned char output[num_total_pixel];

	int i, j;
	// int y_arr[4] = { 0, 63, 142, 255 };
	int y_arr[9] = { 0, 21, 49, 88, 139, 190, 223, 247, 255 };

	for (i = 0; i < TOTAL_HEIGHT; i++) {
		for (j = 0; j < TOTAL_WIDTH; j++) {
			y[i * TOTAL_WIDTH + j] = y_arr[i / BLOCK_HEIGHT * BLOCK_NUM_H + j / BLOCK_WIDTH];
			// printf("y[%d] = %d\n", i * TOTAL_WIDTH + j, y[i * TOTAL_WIDTH + j]);
		}
	}

	int h_step = UCHAR_MAX / (BLOCK_WIDTH / 2 - 1);
	int v_step = UCHAR_MAX / (BLOCK_HEIGHT / 2 - 1);

	FILE *file;

	/* U */
	for (i = 0; i < num_c_pixel; i++) {
		double temp = UCHAR_MAX * (i % (BLOCK_WIDTH / 2)) / (BLOCK_WIDTH / 2);
		u[i] = temp;
		// u[i] = ((i % (BLOCK_WIDTH / 2)) * h_step) % (UCHAR_MAX + 1);
		// u[i] = ((i % (BLOCK_WIDTH / 2)) * h_step) % (UCHAR_MAX + 1);
		// printf("u[%d] = %d\n", i, u[i]);
	}

	for (i = 0; i < num_c_pixel; i++) {
		double temp = UCHAR_MAX * ((i / (TOTAL_WIDTH / 2)) % (BLOCK_HEIGHT / 2)) / (BLOCK_HEIGHT / 2);
		v[i] = temp;
		// v[i] = (((i / (TOTAL_WIDTH / 2)) % (BLOCK_WIDTH / 2)) * v_step) % (UCHAR_MAX + 1);
		// v[i] = u[i];
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

#if 0
	for (i = 0; i < num_total_pixel; i++) {
		if (i % TOTAL_WIDTH == 0) {
			printf("\n");
		}
		printf("%3d ", output[i]);
	}
#endif

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
