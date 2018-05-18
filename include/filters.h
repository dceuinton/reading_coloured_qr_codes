#ifndef DALES_FILTERS_A2
#define DALES_FILTERS_A2

#include <opencv2/opencv.hpp>

using namespace cv;

void displayImage(Mat *src, const char *windowName);
void filterToGreyScale(Mat *src);
void filterToOnlyBlack(Mat *src);
void filterToOnlyBlue(Mat *src);
void medianFilter(Mat* src);

#endif