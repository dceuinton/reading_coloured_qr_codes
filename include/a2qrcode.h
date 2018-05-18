#ifndef DALES_A2_VERYSPECIFIC_FUNCTIONS
#define DALES_A2_VERYSPECIFIC_FUNCTIONS

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "canny.h"

using namespace cv;
using namespace std;

const int SEG1_W  = 41;
const int SEG1_H = 6;
const int SEG2_W  = 47;
const int SEG2_H = 35;
const int SEG3_W  = 35;
const int SEG3_H = 6;

struct BinaryNumber;

int getBoxWidth(vector<Vec4i> *lines, int rows, int cols);
Vec4i getHighestVec(vector<Vec4i> *lines, int rows, int cols);
bool isHorizontalWithTolerance(Vec4i &vec);
int middleHeight(Vec4i &vec);
int leftMostPoint(Vec4i &vec);
vector<int> getAnchor(Mat *src);
void checkPush(uchar val, vector<int> &values);
vector<int> sampleSegment(Mat *src);
void printMessage(vector<char> &message);
void deleteBinaryNumbersInVector(vector<BinaryNumber*> &vec);
void readMessage(vector<int> &binary);
void printBinary(vector<int> &binary);

#endif