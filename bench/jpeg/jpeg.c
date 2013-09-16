#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datatype.h"
#include "config.h"
#include "prototype.h"

#include "rgbimage.h"

//#include "ptlcalls.h"
//#include "npu.h"

#define OUT_BUFFER_SIZE 262144 // in bytes

int main (int argc, const char* argv[]) {
	char inputFileName[64], outputFileName[64];
	UINT32 qualityFactor;
	UINT32 imageFormat;

	UINT8 *outputBuffer;
	FILE *fp;

	UINT8 *outputBufferPtr;

	strcpy(inputFileName, "./data/peppers.rgb");
	strcpy(outputFileName, "./data/peppers.rgb.jpg");

	printf("inputFileName: %s\n", inputFileName);

	qualityFactor = 1024;
	imageFormat = GRAY;

	RgbImage srcImage;
	initRgbImage(&srcImage);
	if (loadRgbImage(inputFileName, &srcImage) == 0) {
		printf("Error! Oops: Cannot load the input image: %s!\n", inputFileName);

		//HADI: ptlcall_kill();
		return -1;
	}

	makeGrayscale(&srcImage);

	outputBuffer = (UINT8 *) malloc(OUT_BUFFER_SIZE * sizeof(UINT8));

	/* Start the simulation */
	//HADI: ptlcall_switch_to_sim();
	//HADI: ptlcall_single_flush("-snapshot-now");
	//HADI: printf("Starting the simulation ...\n");
	//HADI: printf("The first magic instruction!\n");

	//HADI: MAGIC_INST_START;

	outputBufferPtr = outputBuffer;
	outputBufferPtr = encodeImage(
		&srcImage, outputBufferPtr, qualityFactor, imageFormat
	);

	//HADI: MAGIC_INST_STOP;
	//HADI: printf("The last magic instruction!\n");
	//HADI: printf("Stopping the simulation!\n");
	//HADI: ptlcall_single_flush("-snapshot-now");
	//HADI: ptlcall_single_flush("-stop");
	/* Stop the simulation */

	freeRgbImage(&srcImage);

	fp = fopen(outputFileName, "wb");
	if (fp != NULL) {
		fwrite(outputBuffer, 1, outputBufferPtr - outputBuffer, fp);
		fclose(fp);
	}
	free(outputBuffer);

	//HADI: ptlcall_kill();
	return 0;
}

