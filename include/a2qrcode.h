#ifndef DALES_A2_VERYSPECIFIC_FUNCTIONS
#define DALES_A2_VERYSPECIFIC_FUNCTIONS

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

// char encodingarray[64]={' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','x','y','w','z',
// 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','X','Y','W','Z',
// '0','1','2','3','4','5','6','7','8','9','.'};

int getBoxWidth(vector<Vec4i> *lines, int rows, int cols);
Vec4i getHighestVec(vector<Vec4i> *lines, int rows, int cols);
bool isHorizontalWithTolerance(Vec4i &vec);
int middleHeight(Vec4i &vec);
int leftMostPoint(Vec4i &vec);

#endif