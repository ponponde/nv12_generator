#include <stdio.h>

#include <limits.h>

#define BLOCK_WIDTH 512
#define BLOCK_HEIGHT 512 /* should be 2X */
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

	int y_arr[9] = { 0, 21, 49, 88, 139, 190, 223, 247, 255 };

        for (i = 0; i < 1536; i++) {
            for (j = 0; j < 1536; j++) {
                y[i * 1536 + j] = y_arr[i / 512 * 3 + j / 512];
            }
        }
	for (i = 0; i < NUM_C_PIXEL; i++) {
                u[i] = i % 256;
	}
	for (i = 0; i < NUM_C_PIXEL; i++) {
                v[i] = (i / 768) % 256;
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

                // if (i < 100)
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
	FILE *file;

	file = fopen(argv[1], "wb");
	if (file == NULL) {
		perror("Error opening file");
		return 1;
	}
	fwrite(output, sizeof(char), NUM_TOTAL_PIXEL, file);
	fclose(file);

	printf("\nGood Bye\n");

	return 0;
}
