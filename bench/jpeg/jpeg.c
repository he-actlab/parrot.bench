#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datatype.h"
#include "config.h"
#include "prototype.h"

#include "rgbimage.h"

#include "ptlcalls.h"
#include "npu.h"

#define OUT_BUFFER_SIZE 262144 // in bytes

int main (int argc, const char* argv[]) {
	char inputFileName[32], outputFileName[32];
	UINT32 qualityFactor;
	UINT32 imageFormat;

	UINT8 *outputBuffer;
	FILE *fp;

	UINT8 *outputBufferPtr;

	if (argc > 1) {
		strcpy(inputFileName, "./data/baboon-220px.rgb");
		strcpy(outputFileName, "./data/baboon-220px.rgb.jpg");
	} else {
		strcpy(inputFileName, "/root/data/baboon-220px.rgb");
		strcpy(outputFileName, "/root/data/baboon-220px.rgb.jpg");
	}

	qualityFactor = 1024;
	imageFormat = GRAY;

	RgbImage srcImage;
	initRgbImage(&srcImage);
	if (loadRgbImage(inputFileName, &srcImage) == 0) {
		printf("Error! Oops: Cannot load the input image!\n");

		ptlcall_kill();
		return -1;
	}

	makeGrayscale(&srcImage);

	outputBuffer = (UINT8 *) malloc(OUT_BUFFER_SIZE * sizeof(UINT8));

	/* Start the simulation */
	ptlcall_switch_to_sim();
	ptlcall_single_flush("-snapshot-now");
	printf("Starting the simulation ...\n");
	printf("The first magic instruction!\n");

	MAGIC_INST_START;

	outputBufferPtr = outputBuffer;
	outputBufferPtr = encodeImage(
		&srcImage, outputBufferPtr, qualityFactor, imageFormat
	);

	MAGIC_INST_STOP;
	printf("The last magic instruction!\n");
	printf("Stopping the simulation!\n");
	ptlcall_single_flush("-snapshot-now");
	ptlcall_single_flush("-stop");
	/* Stop the simulation */

	freeRgbImage(&srcImage);

	fp = fopen(outputFileName, "wb");
	if (fp != NULL) {
		fwrite(outputBuffer, 1, outputBufferPtr - outputBuffer, fp);
		fclose(fp);
	}
	free(outputBuffer);

	ptlcall_kill();
	return 0;
}

