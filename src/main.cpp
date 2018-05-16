
#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

// sudo env http_proxy='username:password@alb-cache.massey.ac.nz:8080' apt-get

int CVBW     = CV_8UC1; // Black and white
int CVCOLOUR = CV_8UC3; // Colour

void displayImage          (const char *filename);
void displayBlackWhiteImage(const char *filename);
void displayOnlyBlackImage (const char *filename);
void displayOnlyBlueImage  (const char *filename);
void displayCannyTransform (const char *filename);

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
	// displayImage(filename);
	// displayBlackWhiteImage(filename);
	// displayOnlyBlackImage(filename);
	// displayOnlyBlueImage(filename);
	displayCannyTransform(filename);

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

void displayOnlyBlueImage(const char *filename) {
	const char *windowName = "Only Blue Image";
	Mat image = imread(filename);

	uchar *rowPtr;
	int channels = image.channels();
	printf("Channels: %i\n", channels);
	int rows = image.rows;
	// int cols = image.cols * channels;
	int cols = image.cols;

	for (int i = 0; i < rows; i++) {
		rowPtr = image.ptr<uchar>(i);
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
	imshow(windowName, image);
	waitKey(0);
}

// Params for Canny 
// Low Threshold: 30
// High Threshold : ratio = 3 * Low Threshold

Mat imageForCanny;
const char *cannyTrackbarName = "Low Threshold";
const char *cannyWindowName = "Canny Image";
int cannyMarkerClick =0;
int ratio = 3; // Between 2 and 3

void callback_trackbar(int value, void *object) {
	Mat toBeShown(imageForCanny.rows, imageForCanny.cols, CVBW);

	Canny(imageForCanny, toBeShown, cannyMarkerClick, ratio*cannyMarkerClick);

	imshow(cannyWindowName, toBeShown);
}

void displayCannyTransform(const char *filename) {

	int lowThreshold = 50;
	int highThreshold = 100;
	int maxLowThreshold = (int) 255/ratio;
	
	imageForCanny = imread(filename, 0);
	Mat dst(imageForCanny.rows, imageForCanny.cols, CVBW);

	namedWindow(cannyWindowName, WINDOW_AUTOSIZE);	
	
	createTrackbar(cannyTrackbarName, cannyWindowName, &cannyMarkerClick, maxLowThreshold, callback_trackbar);
	setTrackbarPos(cannyTrackbarName, cannyWindowName, 50);	

	Canny(imageForCanny, dst, lowThreshold, highThreshold);

	imshow(cannyWindowName, dst);
	waitKey(0);
}

