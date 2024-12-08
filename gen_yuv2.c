#include <stdio.h>

#include <limits.h>

#define BLOCK_WIDTH 256
#define BLOCK_HEIGHT 256 /* should be 2X */
#define BLOCK_NUM_H 3
#define BLOCK_NUM_V 3

#define TOTAL_WIDTH (BLOCK_WIDTH * BLOCK_NUM_H)
#define TOTAL_HEIGHT (BLOCK_HEIGHT * BLOCK_NUM_V)

#define NUM_Y_PIXEL (TOTAL_WIDTH * TOTAL_HEIGHT)
#define NUM_C_PIXEL (TOTAL_WIDTH * TOTAL_HEIGHT / 4)
#define NUM_TOTAL_PIXEL (NUM_Y_PIXEL + 2 * NUM_C_PIXEL)

unsigned char y[NUM_Y_PIXEL];
unsigned char u[NUM_C_PIXEL];
unsigned char v[NUM_C_PIXEL];
unsigned char output[NUM_TOTAL_PIXEL];

int main(int argc, char *argv[]) {
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
	for (i = 0; i < NUM_C_PIXEL; i++) {
		double temp = UCHAR_MAX * (i % (BLOCK_WIDTH / 2)) / (BLOCK_WIDTH / 2);
		u[i] = temp;
		// u[i] = ((i % (BLOCK_WIDTH / 2)) * h_step) % (UCHAR_MAX + 1);
		// u[i] = ((i % (BLOCK_WIDTH / 2)) * h_step) % (UCHAR_MAX + 1);
		// printf("u[%d] = %d\n", i, u[i]);
	}
        /* V */
	for (i = 0; i < NUM_C_PIXEL; i++) {
		double temp = UCHAR_MAX * ((i / (TOTAL_WIDTH / 2)) % (BLOCK_HEIGHT / 2)) / (BLOCK_HEIGHT / 2);
		v[i] = temp;
		// v[i] = (((i / (TOTAL_WIDTH / 2)) % (BLOCK_WIDTH / 2)) * v_step) % (UCHAR_MAX + 1);
		// v[i] = u[i];
	}

	for (i = 0; i < NUM_Y_PIXEL; i++) {
		output[i] = y[i];
	}
	for (i = 0; i < 2 * NUM_C_PIXEL; i++) {
		if (i % 2 == 0) {
			output[i + NUM_Y_PIXEL] = u[i / 2];
		} else {
			output[i + NUM_Y_PIXEL] = v[i / 2];
		}
		// printf("output[%d]:%d\n", i + NUM_Y_PIXEL, output[i + NUM_Y_PIXEL]);
	}

#if 0
	for (i = 0; i < NUM_TOTAL_PIXEL; i++) {
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
	fwrite(output, sizeof(char), NUM_TOTAL_PIXEL, file);
	fclose(file);

	printf("\nGood Bye\n");

	return 0;
}
