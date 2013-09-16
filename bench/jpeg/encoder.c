#include "datatype.h"
#include "config.h"
#include "prototype.h"

#include "rgbimage.h"

//Hadi: #include "npu.h"
#include <stdio.h>

UINT8	Lqt [BLOCK_SIZE];
UINT8	Cqt [BLOCK_SIZE];
UINT16	ILqt [BLOCK_SIZE];
UINT16	ICqt [BLOCK_SIZE];

INT16	Y1 [BLOCK_SIZE];
INT16	Y2 [BLOCK_SIZE];
INT16	Y3 [BLOCK_SIZE];
INT16	Y4 [BLOCK_SIZE];
INT16	CB [BLOCK_SIZE];
INT16	CR [BLOCK_SIZE];
INT16	Temp [BLOCK_SIZE];
UINT32 lcode = 0;
UINT16 bitindex = 0;

INT16 global_ldc1;
INT16 global_ldc2;
INT16 global_ldc3;

UINT8* encodeImage(
	RgbImage* srcImage,
	UINT8 *outputBuffer,
	UINT32 qualityFactor,
	UINT32 imageFormat
) {
	UINT16 i, j;

	global_ldc1 = 0;
	global_ldc2 = 0;
	global_ldc3 = 0;

	/** Quantization Table Initialization */
	initQuantizationTables(qualityFactor);

	/* Writing Marker Data */
	outputBuffer = writeMarkers(outputBuffer, imageFormat, srcImage->w, srcImage->h);

	for (i = 0; i < srcImage->h; i += 8) {
		for (j = 0; j < srcImage->w; j += 8) {
			readMcuFromRgbImage(srcImage, j, i, Y1);

			/* Encode the data in MCU */
			outputBuffer = encodeMcu(imageFormat, outputBuffer);
		}
	}

	/* Close Routine */
	closeBitstream(outputBuffer);

	return outputBuffer;
}

UINT8* encodeMcu(
	UINT32 imageFormat,
	UINT8 *outputBuffer
) {

	//HADI: MAGIC_INST;
	int iii;
	for (iii = 0; iii < BLOCK_SIZE - 1; ++iii) {
		printf("%f ", Y1[iii] / 255.);
	}
	printf("%f\n", Y1[iii] / 255.);

	levelShift(Y1);
	dct(Y1);
	quantization(Y1, ILqt);

	for (iii = 0; iii < BLOCK_SIZE - 1; ++iii) {
		printf("%f ", Temp[iii] / 128.);
	}
	printf("%f\n", Temp[iii] / 128.);

	outputBuffer = huffman(1, outputBuffer);
	//HADI: MAGIC_INST;

#if 0
	if (imageFormat != GRAY) {
		levelShift(CB);
		dct(CB);
		quantization(CB, ICqt);

		outputBuffer = huffman(2, outputBuffer);

		levelShift(CR);
		dct(CR);
		quantization(CR, ICqt);

		outputBuffer = huffman(3, outputBuffer);
	}
#endif //0

	return outputBuffer;
}
