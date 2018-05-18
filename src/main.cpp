
// ----------------------------------------------------------------------------
// Author: Dale Euinton
// ID: 14026002
// Purpose: To read coloured QR codes for an assignment
// ----------------------------------------------------------------------------

#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <cmath>
#include "a2qrcode.h"
#include "filters.h"
#include "canny.h"
#include "a2transforms.h"


using namespace std;
using namespace cv;

const char *defaultFilename = "images/abcde_rotated.jpg";

int main(int argc, char const *argv[]) {
	const char *filename;
	if (argc == 1) {
		filename = defaultFilename;
	} else if (argc == 2) {
		filename = argv[1];
	} else {
		printf("Usage: ./main <filename>\n");
		return -1;
	}

	printf("Opening %s\n", filename);

	Mat *src = new Mat();
	Mat original = imread(filename);
	// Mat *src2 = new Mat();
	*src = imread(filename);
	// *src2 = imread(filename);

	medianFilter(src);
	medianFilter(src);
	rotateBasedOnCircles(src);

	// getHoughCircles(src, false);

	uchar *row = src->ptr<uchar>(358);
	// printf("Values are: (%u, %u, %u)\n", row[358], row[359], row[360]);

	uchar *row2 = original.ptr<uchar>(358);
	// printf("Original are: (%u, %u, %u)\n", row2[358], row2[359], row2[360]);
	
	vector<int> numbers = sampleSegment(src);
	readMessage(numbers);
	displayImage(src, filename);
	


	return 0;
}

