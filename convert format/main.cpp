/*
	This is a program to convert video format between two pre-defined video type.
	The task is converting an HD video stream (960x540) to a SD stream (176x144)
	Or, converting a SD video stream (176x144) to a HD video stream (960x540)

	Usage: YourProgram.exe C:/myDir/input_vid.rgb  C:/myDir/output_vid.rgb  SD2HD/HD2SD 0/1
	•	The first parameter is the name of the input media file.
	•	The next parameter is the name of the output file
	•	The third parameter controls the operation of the program. 
		SD2HD should run the code for task 1 above to convert an input video of size 176 x 144 to 960 x 540, 
		HD2SD should run the code for task 2 to convert an input video of size 960 x 540 to 176 x 144.
	•	The last parameter controls the whether or not filtering for anti aliasing should be turned on. 
		A value of 0 indicates this should be off, and a value of 1 indicates it should be switched on.

	Author: Deyu Ma
	Date: 02/01/2017
*/

#include <iostream>
#include "Image.h"
using namespace std;

//pre-define the frames in the file
#define FRAMES 100
#define OFFSET 128
//specify the width and height of the SD/HD
#define SD_WIDTH 176
#define SD_HEIGHT 144
#define HD_WIDTH 960
#define HD_HEIGHT 540

/*
	set my subsample rates rule
	define two rates for every direction subsample sample_L/sample_H
	and accordingly, calculate the number of every samples NUM
*/
#define SUBSAMP_WIDTH_H 6
#define SUBSAMP_WIDTH_H_NUM 80
#define SUBSAMP_WIDTH_L 5
#define SUBSAMP_WIDTH_L_NUM 96
#define SUBSAMP_HEIGHT_H 4
#define SUBSAMP_HEIGHT_H_NUM 108
#define SUBSAMP_HEIGHT_L 3
#define SUBSAMP_HEIGHT_L_NUM 36

//My functions for conversion can be recognised by name
void funConvertSD2HD0(MyImage& in, MyImage& out);
void funConvertSD2HD1(MyImage& in, MyImage& out);
void funConvertHD2SD0(MyImage& in, MyImage& out);
void funConvertHD2SD1(MyImage& in, MyImage& out);
void interpolate(int start, int end, int count, int out[]);

int main(int argc, char* argv[])
{
	
	//Check the number of parameters
	if (argc < 5)
	{
		// Tell the user how to run the program
		cerr << "Usage: " 
			<< "YourProgram.exe C:/myDir/input_vid.rgb  C:/myDir/output_vid.rgb  SD2HD/HD2SD 0/1"
			<< endl;
		return 1;
	}
	
	//Obtain the file name for processing
	char* inputFileName = argv[1];
	char* outputFileName = argv[2];

	if (strstr(inputFileName, ".rgb") == NULL)
	{
		cerr << "Error: Image has to be a '.rgb' file" << endl;
		return 1;
	}
	else if (strstr(outputFileName, ".rgb") == NULL)
	{
		cerr << "Error: Output file has to be a '.rgb' file" << endl;
		return 1;
	}

	//Branch to different functions
	if (!strcmp(argv[3], "SD2HD"))
	{
		// Create valid input image and output image
		MyImage inImage, outImage;
		inImage.setWidth(SD_WIDTH);
		inImage.setHeight(SD_HEIGHT);
		inImage.setImagePath(inputFileName);
		inImage.setImageData(new char[3 * SD_WIDTH*SD_HEIGHT]);
		outImage.setWidth(HD_WIDTH);
		outImage.setHeight(HD_HEIGHT);
		outImage.setImagePath(outputFileName);
		outImage.setImageData(new char[3 * HD_WIDTH*HD_HEIGHT]);

		if (!strcmp(argv[4], "0"))
		{
			cout << "Start conversion from SD to HD" << endl;
			for (int i = 0; i < FRAMES; i++)
			{				
				if (!inImage.ReadNthImage(i))
					return 1;
				funConvertSD2HD0(inImage, outImage);
				outImage.WriteImage();
				cout << "frame " << i + 1 << " finished." << endl;
			}
		}
		else if (!strcmp(argv[4], "1"))
		{
			cout << "Start conversion from SD to HD" << endl;
			for (int i = 0; i < FRAMES; i++)
			{		
				if (!inImage.ReadNthImage(i))
					return 1;
				funConvertSD2HD1(inImage, outImage);
				outImage.WriteImage();
				cout << "frame " << i + 1 << " finished." << endl;
			}
		}
		else
		{
			cerr << "Error: 1/0 indicate whether or not filtering for anti aliasing should be turned on" << endl;
			return 1;
		}
	}
	else if (!strcmp(argv[3], "HD2SD"))
	{
		// Create valid input image and output image
		MyImage inImage, outImage;
		inImage.setWidth(HD_WIDTH);
		inImage.setHeight(HD_HEIGHT);
		inImage.setImagePath(inputFileName);
		inImage.setImageData(new char[3 * HD_WIDTH*HD_HEIGHT]);
		outImage.setWidth(SD_WIDTH);
		outImage.setHeight(SD_HEIGHT);
		outImage.setImagePath(outputFileName);
		outImage.setImageData(new char[3 * SD_WIDTH*SD_HEIGHT]);

		if (!strcmp(argv[4], "0"))
		{
			cout << "Start conversion from HD to SD" << endl;
			for (int i = 0; i < FRAMES; i++)
			{
				if (!inImage.ReadNthImage(i))
					return 1;
				funConvertHD2SD0(inImage, outImage);
				outImage.WriteImage();
				cout << "frame " << i+1 << " finished." << endl;
			}
		}
		else if (!strcmp(argv[4], "1"))
		{
			cout << "Start conversion from HD to SD" << endl;
			for (int i = 0; i < FRAMES; i++)
			{
				if (!inImage.ReadNthImage(i))
					return 1;
				funConvertHD2SD1(inImage, outImage);
				outImage.WriteImage();
				cout << "frame " << i + 1 << " finished." << endl;
			}
		}
		else
		{
			cerr << "Error: 1/0 indicate whether or not filtering for anti aliasing should be turned on" << endl;
			return 1;
		}
	}
	else
	{
		cerr << "Error: SD2HD/HD2SD indicate conversion method" << endl;
		return 1;
	}

	//Display an information for success
	cout << "Successfully convert the video " << inputFileName << " to " << outputFileName << endl;
	system("pause");
	return 0;

}







//convert an input video of size 176 x 144 to 960 x 540 without filtering for anti aliasing
void funConvertSD2HD0(MyImage& in, MyImage& out)
{
	char* inBuf = in.getImageData();
	char* outBuf = out.getImageData();

	//sample for every colum and row
	int col, row, elem;
	long pos = 0, source = 0;
	for (row = 0; row < SUBSAMP_HEIGHT_L_NUM; row++) {
		//duplicate colums
		for (col = 0; col < SUBSAMP_WIDTH_L_NUM; col++)
		{
			for (elem = 0; elem < SUBSAMP_WIDTH_L; elem++)//per 5 colums
			{
				outBuf[pos++] = inBuf[source];
				outBuf[pos++] = inBuf[source + 1];
				outBuf[pos++] = inBuf[source + 2];
			}
			source += 3; 
		}
		for (col = 0; col < SUBSAMP_WIDTH_H_NUM; col++)
		{
			for (elem = 0; elem < SUBSAMP_WIDTH_H; elem++)//per 6 colums
			{
				outBuf[pos++] = inBuf[source];
				outBuf[pos++] = inBuf[source + 1];
				outBuf[pos++] = inBuf[source + 2];
			}
			source += 3;
		}
		//duplicate 3 rows
		for (int rows = 0; rows < SUBSAMP_HEIGHT_L-1; rows++)
		{
			for (elem = 0; elem < 3 * HD_WIDTH; elem++)
			{
				outBuf[pos++] = outBuf[pos - 3 * HD_WIDTH];
			}
		}
	}
	for (row = 0; row < SUBSAMP_HEIGHT_H_NUM; row++) {
		//duplicate colums
		for (col = 0; col < SUBSAMP_WIDTH_L_NUM; col++)
		{
			for (elem = 0; elem < SUBSAMP_WIDTH_L; elem++)//per 5 colums
			{
				outBuf[pos++] = inBuf[source];
				outBuf[pos++] = inBuf[source + 1];
				outBuf[pos++] = inBuf[source + 2];
			}
			source += 3;
		}
		for (col = 0; col < SUBSAMP_WIDTH_H_NUM; col++)
		{
			for (elem = 0; elem < SUBSAMP_WIDTH_H; elem++)//per 6 colums
			{
				outBuf[pos++] = inBuf[source];
				outBuf[pos++] = inBuf[source + 1];
				outBuf[pos++] = inBuf[source + 2];
			}
			source += 3;
		}
		//duplicate 4 rows
		for (int rows = 0; rows < SUBSAMP_HEIGHT_H-1; rows++)
		{
			for (elem = 0; elem < 3 * HD_WIDTH; elem++)
			{
				outBuf[pos++] = outBuf[pos - 3 * HD_WIDTH];
			}
		}
	}
	//cout << "pos:" << pos << endl;
	//cout << "source:" << source << endl;
}

//convert an input video of size 176 x 144 to 960 x 540 with filtering for anti aliasing
void funConvertSD2HD1(MyImage& in, MyImage& out)
{
	char* inBuf = in.getImageData();
	char* outBuf = out.getImageData();

	//sample for every colum and row
	int col, row, elem;
	long pos = 0, source = 0;
	for (row = 0; row < SUBSAMP_HEIGHT_L_NUM; row++) {
		//duplicate colums
		for (col = 0; col < SUBSAMP_WIDTH_L_NUM; col++)
		{
			int tempR[SUBSAMP_WIDTH_L], tempG[SUBSAMP_WIDTH_L], tempB[SUBSAMP_WIDTH_L];
			interpolate(inBuf[source], inBuf[source + 3], SUBSAMP_WIDTH_L, tempR);
			interpolate(inBuf[source+1], inBuf[source + 4], SUBSAMP_WIDTH_L, tempG);
			interpolate(inBuf[source+2], inBuf[source + 5], SUBSAMP_WIDTH_L, tempB);
			for (elem = 0; elem < SUBSAMP_WIDTH_L; elem++)//per 5 colums
			{
				outBuf[pos++] = tempR[elem];
				outBuf[pos++] = tempG[elem];
				outBuf[pos++] = tempB[elem];
			}
			source += 3;
		}
		for (col = 0; col < SUBSAMP_WIDTH_H_NUM; col++)
		{
			int tempR[SUBSAMP_WIDTH_H], tempG[SUBSAMP_WIDTH_H], tempB[SUBSAMP_WIDTH_H];
			interpolate(inBuf[source], inBuf[source + 3], SUBSAMP_WIDTH_H, tempR);
			interpolate(inBuf[source + 1], inBuf[source + 4], SUBSAMP_WIDTH_H, tempG);
			interpolate(inBuf[source + 2], inBuf[source + 5], SUBSAMP_WIDTH_H, tempB);
			for (elem = 0; elem < SUBSAMP_WIDTH_H; elem++)//per 5 colums
			{
				outBuf[pos++] = tempR[elem];
				outBuf[pos++] = tempG[elem];
				outBuf[pos++] = tempB[elem];
			}
			source += 3;
		}
		//duplicate 3 rows
		for (int rows = 0; rows < SUBSAMP_HEIGHT_L - 1; rows++)
		{
			for (elem = 0; elem < 3 * HD_WIDTH; elem++)
			{
				outBuf[pos++] = outBuf[pos - 3 * HD_WIDTH];
			}
		}
	}
	for (row = 0; row < SUBSAMP_HEIGHT_H_NUM; row++) {
		//duplicate colums
		for (col = 0; col < SUBSAMP_WIDTH_L_NUM; col++)
		{
			int tempR[SUBSAMP_WIDTH_L], tempG[SUBSAMP_WIDTH_L], tempB[SUBSAMP_WIDTH_L];
			interpolate(inBuf[source], inBuf[source + 3], SUBSAMP_WIDTH_L, tempR);
			interpolate(inBuf[source + 1], inBuf[source + 4], SUBSAMP_WIDTH_L, tempG);
			interpolate(inBuf[source + 2], inBuf[source + 5], SUBSAMP_WIDTH_L, tempB);
			for (elem = 0; elem < SUBSAMP_WIDTH_L; elem++)//per 5 colums
			{
				outBuf[pos++] = tempR[elem];
				outBuf[pos++] = tempG[elem];
				outBuf[pos++] = tempB[elem];
			}
			source += 3;
		}
		for (col = 0; col < SUBSAMP_WIDTH_H_NUM; col++)
		{
			int tempR[SUBSAMP_WIDTH_H], tempG[SUBSAMP_WIDTH_H], tempB[SUBSAMP_WIDTH_H];
			interpolate(inBuf[source], inBuf[source + 3], SUBSAMP_WIDTH_H, tempR);
			interpolate(inBuf[source + 1], inBuf[source + 4], SUBSAMP_WIDTH_H, tempG);
			interpolate(inBuf[source + 2], inBuf[source + 5], SUBSAMP_WIDTH_H, tempB);
			for (elem = 0; elem < SUBSAMP_WIDTH_H; elem++)//per 5 colums
			{
				outBuf[pos++] = tempR[elem];
				outBuf[pos++] = tempG[elem];
				outBuf[pos++] = tempB[elem];
			}
			source += 3;
		}
		//duplicate 4 rows
		for (int rows = 0; rows < SUBSAMP_HEIGHT_H - 1; rows++)
		{
			for (elem = 0; elem < 3 * HD_WIDTH; elem++)
			{
				outBuf[pos++] = outBuf[pos - 3 * HD_WIDTH];
			}
		}
	}
	//cout << "pos:" << pos << endl;
	//cout << "source:" << source << endl;
}

//convert an input video of size 960 x 540 to 176 x 144 without filtering for anti aliasing
void funConvertHD2SD0(MyImage& in, MyImage& out)
{
	char* inBuf = in.getImageData();
	char* outBuf = out.getImageData();

	//sample for every colum and row
	int i, j;
	long pos = 0, source = 0;
	for (j = 0; j < SUBSAMP_HEIGHT_L_NUM; j++) {		
		for (i = 0; i < SUBSAMP_WIDTH_L_NUM; i++)
		{
			outBuf[pos++] = inBuf[source];
			outBuf[pos++] = inBuf[source+1];
			outBuf[pos++] = inBuf[source+2];
			source += 3 * SUBSAMP_WIDTH_L; //per 5 colums
		}
		for (i = 0; i < SUBSAMP_WIDTH_H_NUM; i++)
		{
			outBuf[pos++] = inBuf[source];
			outBuf[pos++] = inBuf[source+1];
			outBuf[pos++] = inBuf[source+2];
			source += 3 * SUBSAMP_WIDTH_H; //per 6 colums 
		}
		source += 3 * HD_WIDTH * (SUBSAMP_HEIGHT_L-1); //per 3 rows
	}
	for (j = 0; j < SUBSAMP_HEIGHT_H_NUM; j++) {
		for (i = 0; i < SUBSAMP_WIDTH_L_NUM; i++)
		{
			outBuf[pos++] = inBuf[source];
			outBuf[pos++] = inBuf[source+1];
			outBuf[pos++] = inBuf[source+2];
			source += 3 * SUBSAMP_WIDTH_L; //per 5 colums
		}
		for (i = 0; i < SUBSAMP_WIDTH_H_NUM; i++)
		{
			outBuf[pos++] = inBuf[source];
			outBuf[pos++] = inBuf[source+1];
			outBuf[pos++] = inBuf[source+2];
			source += 3 * SUBSAMP_WIDTH_H; //per 6 colums
		}
		source += 3 * HD_WIDTH * (SUBSAMP_HEIGHT_H-1); //4 rows by rows
	}
	//cout << "pos:" << pos << endl;
	//cout << "source:" << source << endl;
}

//convert an input video of size 960 x 540 to 176 x 144 with filtering for anti aliasing
void funConvertHD2SD1(MyImage& in, MyImage& out)
{
	char* inBuf = in.getImageData();
	char* outBuf = out.getImageData();

	//sample for every colum and row
	int i, j, m,n;
	long pos = 0, source = 0;
	int tempR,tempG,tempB;
	for (j = 0; j < SUBSAMP_HEIGHT_L_NUM; j++) {
		for (i = 0; i < SUBSAMP_WIDTH_L_NUM; i++)
		{	//average the data
			tempR = 0; tempG = 0; tempB = 0;
			for (m = 0;m < SUBSAMP_WIDTH_L;m++)
				for (n = 0;n < SUBSAMP_HEIGHT_L;n++)
				{
					tempR += OFFSET + inBuf[source + m * 3 + n * 3 * HD_WIDTH];
					tempG += OFFSET + inBuf[source + 1 +m * 3 + n * 3 * HD_WIDTH];
					tempB += OFFSET + inBuf[source + 2 +m * 3 + n * 3 * HD_WIDTH];
				}
			outBuf[pos++] = (char)(tempR / (SUBSAMP_WIDTH_L*SUBSAMP_HEIGHT_L) - OFFSET);
			outBuf[pos++] = (char)(tempG / (SUBSAMP_WIDTH_L*SUBSAMP_HEIGHT_L) - OFFSET);
			outBuf[pos++] = (char)(tempB / (SUBSAMP_WIDTH_L*SUBSAMP_HEIGHT_L) - OFFSET);
			source += 3 * SUBSAMP_WIDTH_L; //per 5 colums
			//cout << (int)outBuf[pos-1] << endl;
		}
		for (i = 0; i < SUBSAMP_WIDTH_H_NUM; i++)
		{	//average the data
			tempR = 0; tempG = 0; tempB = 0;
			for (m = 0;m < SUBSAMP_WIDTH_H;m++)
				for (n = 0;n < SUBSAMP_HEIGHT_L;n++)
				{
					tempR += OFFSET + inBuf[source +m * 3 + n * 3 * HD_WIDTH];
					tempG += OFFSET + inBuf[source + 1 +m * 3 + n * 3 * HD_WIDTH];
					tempB += OFFSET + inBuf[source + 2 +m * 3 + n * 3 * HD_WIDTH];
				}
			outBuf[pos++] = (char)(tempR / (SUBSAMP_WIDTH_H*SUBSAMP_HEIGHT_L)-OFFSET);
			outBuf[pos++] = (char)(tempG / (SUBSAMP_WIDTH_H*SUBSAMP_HEIGHT_L)-OFFSET);
			outBuf[pos++] = (char)(tempB / (SUBSAMP_WIDTH_H*SUBSAMP_HEIGHT_L)-OFFSET);
			source += 3 * SUBSAMP_WIDTH_H; //per 6 colums
		}
		source += 3 * HD_WIDTH * (SUBSAMP_HEIGHT_L - 1); //per 3 rows
	}
	for (j = 0; j < SUBSAMP_HEIGHT_H_NUM; j++) {
		for (i = 0; i < SUBSAMP_WIDTH_L_NUM; i++)
		{	//average the data
			tempR = 0; tempG = 0; tempB = 0;
			for (m = 0;m < SUBSAMP_WIDTH_L;m++)
				for (n = 0;n < SUBSAMP_HEIGHT_H;n++)
				{
					tempR += OFFSET + inBuf[source +m * 3 + n * 3 * HD_WIDTH];
					tempG += OFFSET + inBuf[source + 1 +m * 3 + n * 3 * HD_WIDTH];
					tempB += OFFSET + inBuf[source + 2 +m * 3 + n * 3 * HD_WIDTH];
				}
			outBuf[pos++] = (char)(tempR / (SUBSAMP_WIDTH_L*SUBSAMP_HEIGHT_H)-OFFSET);
			outBuf[pos++] = (char)(tempG / (SUBSAMP_WIDTH_L*SUBSAMP_HEIGHT_H)-OFFSET);
			outBuf[pos++] = (char)(tempB / (SUBSAMP_WIDTH_L*SUBSAMP_HEIGHT_H)-OFFSET);
			source += 3 * SUBSAMP_WIDTH_L; //per 5 colums
		}
		for (i = 0; i < SUBSAMP_WIDTH_H_NUM; i++)
		{	//average the data
			tempR = 0; tempG = 0; tempB = 0;
			for (m = 0;m < SUBSAMP_WIDTH_H;m++)
				for (n = 0;n < SUBSAMP_HEIGHT_H;n++)
				{
					tempR += OFFSET + inBuf[source +m * 3 + n * 3 * HD_WIDTH];
					tempG += OFFSET + inBuf[source + 1 +m * 3 + n * 3 * HD_WIDTH];
					tempB += OFFSET + inBuf[source + 2 +m * 3 + n * 3 * HD_WIDTH];
				}
			outBuf[pos++] = (char)(tempR / (SUBSAMP_WIDTH_H*SUBSAMP_HEIGHT_H)-OFFSET);
			outBuf[pos++] = (char)(tempG / (SUBSAMP_WIDTH_H*SUBSAMP_HEIGHT_H)-OFFSET);
			outBuf[pos++] = (char)(tempB / (SUBSAMP_WIDTH_H*SUBSAMP_HEIGHT_H)-OFFSET);
			source += 3 * SUBSAMP_WIDTH_H; //per 6 colums
		}
		source += 3 * HD_WIDTH * (SUBSAMP_HEIGHT_H - 1); //4 rows by rows
	}
	//cout << "pos:" << pos << endl;
	//cout << "source:" << source << endl;
}

void interpolate(int start, int end, int count, int out[])
{
	float diff = (float)(end - start) / count;
	for (int i = 0;i < count;i++)
	{
		out[i] = start + i * diff;
	}
}