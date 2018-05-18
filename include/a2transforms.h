
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
void scale(Mat* src);
void rotateBasedOnCircles(Mat *src);
bool isThereCircleInUpperRightQuadrant(Mat *src, vector<Vec3f> *circles);
void rotate90CW(Mat *src);

#endif