#ifndef DALES_CANNY
#define DALES_CANNY

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <stdio.h>
#include "filters.h"
#include "a2transforms.h"

using namespace cv;

Mat* doCanny(Mat *src, bool show);
void cannyTrackbarCallback(int value, void *object);
void goGrey(Mat* src);
void transformToCanny(Mat *src);

vector<Vec4i>* getHoughLines(Mat* src);
void showHoughLines(Mat *src);
vector<Vec3f>* getHoughCircles(Mat* src);



#endif