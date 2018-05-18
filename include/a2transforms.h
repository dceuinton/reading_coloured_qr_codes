
#ifndef DALES_TRANSFORMS_A2
#define DALES_TRANSFORMS_A2

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <cmath>
#include <stdio.h>

#include "canny.h"

using namespace cv;

Mat* rotateImage(Mat *src, bool show);
double getLength(Vec4i &vec);
void printRotationMatrix(Mat &src);

#endif