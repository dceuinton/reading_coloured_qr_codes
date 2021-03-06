#include "canny.h"

Mat *cannyImage;
const char *cannyTBarName = "Low Threshold";
const char *cannyWindow = "Canny Image";
int cannyClick = 0;
int cannyRatio = 3;

Mat* doCanny(Mat *src, bool show) {
	int lowThreshold = 50;
	int highThreshold = 100;
	int maxLowThreshold = (int) 255/cannyRatio;

	goGrey(src);
	cannyImage = src;
	Mat *dst = new Mat(src->size(), src->type());
	Canny(*src, *dst, lowThreshold, highThreshold);

	if (show) {
		namedWindow(cannyWindow, WINDOW_AUTOSIZE);
		createTrackbar(cannyTBarName, cannyWindow, &cannyClick, maxLowThreshold, cannyTrackbarCallback);
		setTrackbarPos(cannyTBarName, cannyWindow, 50);	

		imshow(cannyWindow, *dst);
		waitKey(0);
	}

	return dst;
}

void cannyTrackbarCallback(int value, void *object) {
	Mat toBeShown(cannyImage->rows, cannyImage->cols, CV_8UC1);
	Canny(*cannyImage, toBeShown, cannyClick, cannyRatio*cannyClick);
	imshow(cannyWindow, toBeShown);
}

void goGrey(Mat* src) {
	if (src->type() != CV_8UC1) {
		filterToGreyScale(src);
	}
}

void transformToCanny(Mat *src) {
	goGrey(src);
	Canny(*src, *src, 50, 100);
}

// ------------------------------------------------------------
// Hough
// ------------------------------------------------------------

vector<Vec4i>* getHoughLines(Mat* src) {
	Mat copy = *src;
	transformToCanny(&copy);
	vector<Vec4i> *lines = new vector<Vec4i>();
	HoughLinesP(copy, *lines, 1, CV_PI/180, 150, 0, 5);
	return lines;
}

void showHoughLines(Mat *src) {
	vector<Vec4i> *lines = getHoughLines(src);
	Mat dst;
	if (src->type() == CV_8UC1) {
		cvtColor(*src, dst, CV_GRAY2BGR);		
	} else {
		dst = *src;
	}	

	for (int i = 0; i < lines->size(); i++) {
		Vec4i l = lines->at(i);
		line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
	}

	imshow("Hough Lines", dst);
	waitKey(0);

	delete lines;
	lines = NULL;
}

vector<Vec3f>* getHoughCircles(Mat* src, bool show) {
	vector<Vec3f>* circles = new vector<Vec3f>();
	Mat *dst = rotateImage(src, false);
	Mat original = *dst;

	// imshow("original", original);
	// waitKey(0);

	filterToOnlyBlue(dst);
	cvtColor(*dst, *dst, CV_BGR2GRAY);

	// imshow("Testing", *dst);
	// waitKey(0);

	GaussianBlur(*dst, *dst, Size(9,9), 2, 2);

	// imshow("Testing 2", *dst);
	// waitKey(0);

	HoughCircles(*dst, *circles, CV_HOUGH_GRADIENT, 1, 3*dst->rows/7, 100, 80, 0, dst->rows/5);
	if (show) {
		for (int i = 0; i < circles->size(); i++) {
			Point center(cvRound(circles->at(i)[0]), cvRound(circles->at(i)[1]));
			int radius = cvRound(circles->at(i)[2]);

			circle(original, center, radius, Scalar(0,255,0), -1, 8, 0);
			circle(original, center, radius, Scalar(0,0,255), 3, 8, 0);
			
		}		
			imshow("Drawn Hough Circles", original);
			waitKey(0);
	}

	return circles;
}