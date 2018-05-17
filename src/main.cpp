
#include <stdio.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
// #include "decoding.h"

using namespace std;
using namespace cv;

// sudo env http_proxy='username:password@alb-cache.massey.ac.nz:8080' apt-get

char encodingarray[64]={' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','x','y','w','z',
'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','X','Y','W','Z',
'0','1','2','3','4','5','6','7','8','9','.'};

int CVBW     = CV_8UC1; // Black and white
int CVCOLOUR = CV_8UC3; // Colour

// Hardcoded for the QR code types that we are using in this assignment
const int SEG1_WIDTH  = 41;
const int SEG1_HEIGHT = 6;
const int SEG2_WIDTH  = 47;
const int SEG2_HEIGHT = 35;
const int SEG3_WIDTH  = 35;
const int SEG3_HEIGHT = 6;

int boxWidth  = 20;
int boxHeight = 20;

Point seg1Start;
Point seg2Start;
Point seg3Start;

// Method declarations
void displayImage              (const char *filename);
void displayBlackWhiteImage    (const char *filename);
void displayOnlyBlackImage     (const char *filename);
void displayOnlyBlueImage      (const char *filename);
void displayCannyTransform     (const char *filename);
void displayHoughTransform     (const char *filename);
vector<Vec4i>* diplayAndPrintWidthAndHeightOfSquares(const char *filename, bool show); 
int getWidth     (Vec4i &vec);
int getHeight    (Vec4i &vec);
bool isHorizontal(Vec4i &vec);
bool isVertical  (Vec4i &vec);
void setWidthHeightAndPoints(const char *filename);
void sampleFirstSegment(const char *filename, bool drawCircles);
void convertSample(vector<int> &binary);

int main(int argc, char const *argv[]) {
	const char *filename;
	if (argc == 1) {
		filename = "images/abcde.jpg";
	} else if (argc == 2) {
		filename = argv[1];
	} else {
		printf("Usage: main <filename>\n");
		return -1;
	}

	printf("Opening %s\n", filename);
	// displayImage(filename);
	// displayBlackWhiteImage(filename);
	// displayOnlyBlackImage(filename);
	// displayOnlyBlueImage(filename);
	// displayCannyTransform(filename);
	// displayHoughTransform(filename);

	// diplayAndPrintWidthAndHeightOfSquares(filename, true);
	// setWidthHeightAndPoints(filename);
	sampleFirstSegment(filename, true);
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

// This is also goodS
// Low: 25
// Ratio: 4 

Mat imageForCanny;
const char *cannyTrackbarName = "Low Threshold";
const char *cannyWindowName = "Canny Image";
int cannyMarkerClick =0;
int ratio = 2; // Between 2 and 3

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

Mat* getHardCodedCanny(const char *filename) {
	Mat image = imread(filename, 0);
	Mat *output = new Mat(image.size(), CVBW);

	Canny(image, *output, 25, 100);

	return output;
}

vector<Vec4i>* getHardCodedHoughLines(const char *filename) {
	Mat *image = getHardCodedCanny(filename);
	vector<Vec4i> *lines = new vector<Vec4i>();
	HoughLinesP(*image, *lines, 1, CV_PI/180, 150, 0, 5);
	return lines;
} 

const char *houghWindowName = "Hough Image";

void displayHoughTransform(const char *filename) {
	Mat *image = getHardCodedCanny(filename);
	Mat output;

	cvtColor(*image, output, CV_GRAY2BGR);

	vector<Vec4i> lines;
	HoughLinesP(*image, lines, 1, CV_PI/180, 150, 0, 5);

	for (int i = 0; i < lines.size(); i++) {
		Vec4i l = lines[i];

		if (l[0] == l[2]) {
			// CV_AA is antialiased 
			line(output, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
			// imshow(houghWindowName, output);
			// waitKey(0);
		}
	}

	imshow(houghWindowName, output);
	waitKey(0);	
	delete image; 
	image = NULL;
}

// The gap can't be less than 5 or we won't find it.

vector<Vec4i>* diplayAndPrintWidthAndHeightOfSquares(const char *filename, bool show) {
	Mat *image = getHardCodedCanny(filename);
	Mat output;
	vector<Vec4i> *lines = getHardCodedHoughLines(filename);
	vector<Vec4i> *importantVecs = new vector<Vec4i>();

	if (lines->size() == 0) {
		printf("ERROR: getHardCodedHoughLines failed.\n");
		return NULL;
	}
	vector<Vec4i> &lineAdr = *lines;
	Vec4i vec = lineAdr[0];
	printf("(%i, %i, %i, %i)\n", vec[0], vec[1], vec[2], vec[3]);
	printf("Rows: %i\n", image->rows);
	printf("Cols: %i\n", image->cols);
	printf("Channels: %i\n", image->channels());

	Vec4i leftMostVec       = {1000, 1000, 1000, 1000};
	Vec4i secondLeftMostVec = {1000, 1000, 1000, 1000};
	Vec4i topMostVec        = {1000, 1000, 1000, 1000};
	Vec4i secondTopMostVec  = {1000, 1000, 1000, 1000};

	int errorGap = 5;

	for (int i = 0; i < lines->size(); i++) {
		Vec4i vec = lineAdr[i];
		if (isVertical(vec)) {
			if (vec[0] < leftMostVec[0]) {
				leftMostVec = vec;
			}
		} else if (isHorizontal(vec)) {
			if (vec[1] < topMostVec[1]) {
				topMostVec = vec;
			}
		}
	}

	for (int i = 0; i < lines->size(); i++) {
		Vec4i vec = lineAdr[i];
		if (isVertical(vec)) {
			if (vec[0] < secondLeftMostVec[0]) {
				if (abs(vec[0] - leftMostVec[0]) > errorGap) {
					secondLeftMostVec = vec;
				}
			} else if (vec[0] == secondLeftMostVec[0]) {
				if (getHeight(vec) > getHeight(secondLeftMostVec)) {
					secondLeftMostVec = vec;
				}
			}
		} else if (isHorizontal(vec)) {
			if (vec[1] < secondTopMostVec[1]) {
				if (abs(vec[1] - topMostVec[1]) > errorGap) {
					secondTopMostVec = vec;
				}
			} else if (vec[1] == secondTopMostVec[1]) {
				if (getHeight(vec) > getHeight(secondTopMostVec)) {
					secondTopMostVec = vec;
				}
			}
		}
	}

	printf("Left-est Vec:        (%i, %i, %i, %i)\n", leftMostVec[0], leftMostVec[1], leftMostVec[2], leftMostVec[3]);
	printf("Second Left-est Vec: (%i, %i, %i, %i)\n", secondLeftMostVec[0], secondLeftMostVec[1], secondLeftMostVec[2], secondLeftMostVec[3]);
	printf("Highest Vec:         (%i, %i, %i, %i)\n", topMostVec[0], topMostVec[1], topMostVec[2], topMostVec[3]);
	printf("Second Highest Vec:  (%i, %i, %i, %i)\n", secondTopMostVec[0], secondTopMostVec[1], secondTopMostVec[2], secondTopMostVec[3]);

	importantVecs->push_back(leftMostVec);
	importantVecs->push_back(secondLeftMostVec);
	importantVecs->push_back(topMostVec);
	importantVecs->push_back(secondTopMostVec);

	if (show) {
		cvtColor(*image, output, CV_GRAY2BGR);

		line(output, Point(leftMostVec[0], leftMostVec[1]), Point(leftMostVec[2], leftMostVec[3]), Scalar(0, 0, 255), 3, CV_AA);
		line(output, Point(secondLeftMostVec[0], secondLeftMostVec[1]), Point(secondLeftMostVec[2], secondLeftMostVec[3]), Scalar(0, 0, 255), 3, CV_AA);
		line(output, Point(topMostVec[0], topMostVec[1]), Point(topMostVec[2], topMostVec[3]), Scalar(0, 255, 0), 3, CV_AA);
		line(output, Point(secondTopMostVec[0], secondTopMostVec[1]), Point(secondTopMostVec[2], secondTopMostVec[3]), Scalar(0, 255, 0), 3, CV_AA);

		imshow("Width will be inbetween the lines", output);
		waitKey(0);	
	}	

	delete lines;
	lines = NULL;

	return importantVecs;
}

void setWidthHeightAndPoints(const char *filename) {
	vector<Vec4i> *importantVecs = diplayAndPrintWidthAndHeightOfSquares(filename, false);

	boxWidth = abs(importantVecs->at(0)[0] - importantVecs->at(1)[0]);
	boxHeight = abs(importantVecs->at(2)[1] - importantVecs->at(3)[1]);

	printf("boxWidth:  %i\n", boxWidth);
	printf("boxHeight: %i\n", boxHeight);

	// importantVecs 0 is left most
	// importantVecs 1 is second left most
	// importantVecs 2 is top most
	// importantVecs 3 is second  top most

	int topMostX1 = importantVecs->at(2)[0];
	int topMostX2 = importantVecs->at(2)[2];

	if (topMostX1 < topMostX2) {
		seg1Start.x = topMostX1;
	} else {
		seg1Start.x = topMostX2;
	}
	seg1Start.y = importantVecs->at(2)[1];

	seg2Start.x = importantVecs->at(0)[0];
	seg3Start.x = seg1Start.x;

	int leftMostY1 = importantVecs->at(0)[1];
	int leftMostY2 = importantVecs->at(0)[3];

	if (leftMostY1 < leftMostY2) {
		seg2Start.y = leftMostY1;
		seg3Start.y = leftMostY2;
	} else {
		seg2Start.y = leftMostY2;
		seg3Start.y = leftMostY1;
	}
	
	printf("seg1Start (%i, %i)\n", seg1Start.x, seg1Start.y);
	printf("seg2Start (%i, %i)\n", seg2Start.x, seg2Start.y);
	printf("seg3Start (%i, %i)\n", seg3Start.x, seg3Start.y);

	delete importantVecs;
	importantVecs = NULL;
}

int getWidth(Vec4i &vec) {
	return abs(vec[0] - vec[2]);
}

int getHeight(Vec4i &vec) {
	return abs(vec[1] - vec[3]);
}

bool isHorizontal(Vec4i &vec) {
	return (vec[1] == vec[3]) ? true : false;
}

bool isVertical(Vec4i &vec) {
	return (vec[0] == vec[2]) ? true : false;
}

void checkValueAndPush(uchar val, vector<int> &values) {
	// printf("Val: %u\n", val);

	uchar margin = 10;

	uchar lower = 0 + margin;
	uchar upper = 255 - margin;

	printf("Val: %u\n", val);

	if (val < lower) {
		values.push_back(0);
		printf("Going in as 0\n\n");
	} else if (val > upper) {
		printf("Goint in as 1\n\n");
		values.push_back(1);
	} else {
		printf("ERROR: Margin or read failed at index %i with val: %u", (int) values.size()+1, val);
		return;
	}
}

void printBinary(vector<int> &binary) {
	// if (binary.size() % 6 == 0) {
		for (int i = 0; i < (int)binary.size()/6; i++) {
			int counter = 6*i;
			printf("%i, %i, %i, %i, %i, %i\n", binary.at(counter), binary.at(counter + 1), binary.at(counter + 2), binary.at(counter + 3), binary.at(counter + 4), binary.at(counter + 5));
		}
	// } else {
		// printf("ERROR: printBinary has size %i which is not a multiple of 6\n", (int) binary.size());
	// }
}

void sampleFirstSegment(const char *filename, bool drawCircles) {
	Mat image = imread(filename, 1);
	int channels = image.channels();
	// waitKey(0);
	setWidthHeightAndPoints(filename);

	int xLocation = (int) seg1Start.x + boxWidth/2;
	int yLocation = (int) seg1Start.y + boxHeight/2;

	printf("xLocation: %i\n", xLocation);
	printf("yLocation: %i\n", yLocation);

	vector<int> binary;

	uchar *rowStart;

	uchar b;
	uchar g;
	uchar r;

// SEGMENT 1
	for (int i = 0; i < SEG1_HEIGHT; i++) {
	// for (int i = 0; i < 1; i++) {
		rowStart = image.ptr<uchar>(yLocation + i*boxHeight);
		for (int j = 0; j < SEG1_WIDTH; j++) {
		// for (int j = 0; j < 8; j++) {
			// printf("Col: %i\n", xLocation + j*boxWidth);
			b = rowStart[(xLocation+j*boxWidth) * channels];
			g = rowStart[(xLocation+j*boxWidth) * channels + 1];
			r = rowStart[(xLocation+j*boxWidth) * channels + 2];
			// printf("(%u, %u, %u)\n", b, g, r);
			checkValueAndPush(b, binary);
			checkValueAndPush(g, binary);
			checkValueAndPush(r, binary);

			if (drawCircles) {
				Point center(xLocation + j*boxWidth, yLocation + i*boxHeight);
				circle(image, center, 5, Scalar(50, 255, 125), 3);	
			}
			
		}
		// break;
	}

	for ()

	// printBinary(binary);

	if (drawCircles) {
		imshow("Check", image);
		waitKey(0);
	}

	convertSample(binary);

}

struct BinaryNumber {
	int *values;
	int number;


// Mixing up the binary number init because I realised while Mats are BGR the assignment specs are RGB
	BinaryNumber(int a, int b, int c, int d, int e, int f) {
		values = new int[6];
		values[0] = c;
		values[1] = b;
		values[2] = a;
		values[3] = f;
		values[4] = e;
		values[5] = d;

		number = 32*values[0] + 16*values[1] + 8*values[2] + 4*values[3] + 2*values[4] + 1*values[5];
	}

	~BinaryNumber() {
		delete[] values;
		values = NULL;
	}
};

void printMessage(vector<char> &message) {
	for (int i = 0; i < message.size(); i++) {
		printf("%c", message.at(i));
	}
	printf("\n");
}

void printNumbers(vector<BinaryNumber*> &numbers) {
	for (int i = 0; i < numbers.size(); i++) {
		printf("%i\n", numbers.at(i)->number);
	}
}

void convertSample(vector<int> &binary) {
	vector<BinaryNumber*> numbers;

	printBinary(binary);

	// printf("Binary size: %i\n", (int) binary.size());

	for (int i = 0; i < (int) binary.size()/6; i++) {
		int counter = 6*i;
		BinaryNumber *bin = new BinaryNumber(binary.at(counter), binary.at(counter+1), binary.at(counter+2), binary.at(counter+3), binary.at(counter+4), binary.at(counter+5));
		
		// printf("Bin number: %i\n", bin->number);
		numbers.push_back(bin);
	}

	vector<char> message;

	for (int i = 0; i < numbers.size(); i++) {
		message.push_back(encodingarray[numbers.at(i)->number]);
	}

	// printNumbers(numbers);
	printMessage(message);

	Mat image = imread("images/abcde.jpg", 1);
	imshow("Check", image);
	waitKey(0);
}




