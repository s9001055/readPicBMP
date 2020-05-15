/*	7108056053				*/
/*	Multimedia System			*/
/*	HomeWork 1	Histogram for an image	*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define sizeOfHeader 	14
#define sizeOfInfo	40
#define BMPName "Apartment_float_o15C.bmp"
#define outputFileName "Apartment_float_o15C_Histogram.csv" 

typedef struct bmp_t{
	//file header 14bytes
	uint16_t type;
	uint32_t fileSize;
	uint32_t reserved0;
	uint32_t dataOffset;
	//info header 40bytes 
	uint32_t headerSize;
	uint32_t width;			//bmp width
	uint32_t height;		//bmp height
	uint16_t planes;
	uint16_t bitsPerPixel;
	uint32_t compression;
	uint32_t imageSize;
	uint32_t xPixelsPerMeter;
	uint32_t yPixelsPerMeter;
	uint32_t colorUsed;
	uint32_t colorImportant;
}__attribute__((packed,aligned(1))) bmp_t;

typedef struct rgb_t{
	uint8_t b;
	uint8_t g;
	uint8_t r;
}rgb_t;

typedef struct recordRGB_t{
	uint32_t countB;
	uint32_t countG;
	uint32_t countR;
}recordRGB_t;

void outputCSV(struct recordRGB_t *recordRGB, uint32_t width, uint32_t height)
{
	uint8_t filetitle[] = "fileTitle";
	uint32_t size = width * height;
	FILE *fCSV;
	fCSV = fopen(outputFileName, "w");
	fprintf(fCSV, "%s", filetitle);
	fprintf(fCSV, "\n");
	fprintf(fCSV, "%d*%d,,Red,Green,Blue\n", height, width);
	fprintf(fCSV, "resolution,value,count,count,count\n");
	int32_t i;
	for (i = 0; i < 256; i++){
		fprintf(fCSV, "%u,%d,%u,%u,%u\n", size, i, (recordRGB + i) -> countR, (recordRGB + i) -> countG, (recordRGB + i) -> countB);
	}
}

void readRGB(FILE *fp, uint32_t width, uint32_t height)
{
	fseek(fp, 54, SEEK_SET);
	struct recordRGB_t *recordRGB = malloc(sizeof(recordRGB_t) * 256);

	struct rgb_t **pix;
	pix = (rgb_t **)malloc(sizeof(rgb_t*) * height);	
	uint32_t i,j;
	for (i = 0; i < height; i++)
		*(pix + i) = (rgb_t *)malloc(sizeof(rgb_t) * width);

	for (i = 0; i < 256; i++){
		(recordRGB + i) -> countR = 0;
		(recordRGB + i) -> countG = 0;
		(recordRGB + i) -> countB = 0;
	}

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++){
			fread(*(pix + i) + j, 1, sizeof(rgb_t), fp);
			(recordRGB + ((*(pix + i) +j) -> r)) -> countR += 1;
			(recordRGB + ((*(pix + i) +j) -> g)) -> countG += 1;	
			(recordRGB + ((*(pix + i) +j) -> b)) -> countB += 1;	
		}
	outputCSV(recordRGB, width, height);
}

int main()
{
	FILE *fp;
	struct bmp_t bmpPic;
	uint8_t *ptr;
	ptr = (uint8_t*)&bmpPic;

	fp = fopen(BMPName, "r");

	fread(ptr, 1, sizeOfHeader, fp);
	fseek(fp, sizeOfHeader, SEEK_SET);
	fread(ptr + sizeOfHeader, 1, sizeOfInfo, fp);
	
	printf("width is %u \n", bmpPic.width);
	printf("height is %u \n", bmpPic.height);
	printf("dataOffset is %u \n", bmpPic.dataOffset);
	readRGB(fp, bmpPic.width, bmpPic.height);
	
	return 0;
}
