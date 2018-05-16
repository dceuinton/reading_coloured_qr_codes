
#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void displayImage(const char *filename);
void displayBlackWhiteImage(const char *filename);
void displayOnlyBlackImage(const char *filename);

int main(int argc, char const *argv[]) {
	const char *filename;
	if (argc == 1) {
		filename = "images/abcde.jpg";
	} else if (argc == 2) {
		filename = argv[1];
	} else {
		printf("Usage: main <filename>\n");
		return 1;
	}

	printf("Opening %s\n", filename);
	displayImage(filename);
	displayBlackWhiteImage(filename);
	displayOnlyBlackImage(filename);

	return 0;
}

void displayImage(const char *filename) {
	const char *windowName = "Normal Image";
	Mat image = imread(filename);
	imshow(windowName, image);
	waitKey(0);
}

void displayBlackWhiteImage(const char *filename) {
	const char *windowName = "Black and White Image";
	Mat image = imread(filename, 0);
	imshow(windowName, image);
	waitKey(0);
}

void displayOnlyBlackImage(const char *filename) {
	const char *windowName = "Only Black Image";
	Mat image = imread(filename, 0);

	uchar *rowPtr;

	for (int i = 0; i < image.rows; i++) {
		rowPtr = image.ptr<uchar>(i);
		for (int j =0; j < image.cols; j++) {
			if (rowPtr[j] < 10) {
				rowPtr[j] = 0;
			} else {
				rowPtr[j] = 255;
			}
		}
	}

	imshow(windowName, image);

	waitKey(0);
}

