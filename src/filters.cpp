
#include "filters.h"

void displayImage(Mat *src, const char *windowName) {
	imshow(windowName, *src);
	waitKey(0);
}

void filterToGreyScale(Mat *src) {
	cvtColor(*src, *src, CV_BGR2GRAY);
}

void filterToOnlyBlack(Mat *src) {
	int threshold = 22;
	filterToGreyScale(src);
	uchar *rowPtr;
	for (int i = 0; i < src->rows; i++) {
		rowPtr = src->ptr<uchar>(i);
		for (int j =0; j < src->cols; j++) {
			if (rowPtr[j] < threshold) {
				rowPtr[j] = 0;
			} else {
				rowPtr[j] = 255;
			}
		}
	}
}

void filterToOnlyBlue(Mat *src) {
	uchar *rowPtr;
	int channels = src->channels();
	int rows = src->rows;
	int cols = src->cols;

	for (int i = 0; i < rows; i++) {
		rowPtr = src->ptr<uchar>(i);
		for (int j =0; j < cols; j++) {
			if (rowPtr[j * channels] < 150) {
				rowPtr[j * channels] = 255;
				rowPtr[j * channels + 1] = 255;
				rowPtr[j * channels + 2] = 255;
			} else if (rowPtr[j * channels + 1] > 100) {
				rowPtr[j * channels + 0] = 255;
				rowPtr[j * channels + 1] = 255;
				rowPtr[j * channels + 2] = 255;
			} else if (rowPtr[j * channels + 2] > 100) {
				rowPtr[j * channels + 0] = 255;
				rowPtr[j * channels + 1] = 255;
				rowPtr[j * channels + 2] = 255;
			}
		}
	}
}

// void filterOutBlue(Mat *src) {
// 	cvtColor(*src, *src, COLOR_BGR2HSV);
// }

void medianFilter(Mat* src) {
	medianBlur(*src, *src, 3);
}