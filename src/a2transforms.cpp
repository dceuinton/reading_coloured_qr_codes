
#include "a2transforms.h"

Mat* rotateImage(Mat *src, bool show) {
	Mat *original = new Mat(src->size(), src->type());
	// showHoughLines(src);
	*original = *src;
	vector<Vec4i> *lines = getHoughLines(src);
	vector<Vec4i> &lineAdr = *lines;

	Mat *dst = new Mat(original->size(), original->type());

	Point2f center(original->cols/2.0f, original->rows/2.0f);

	double longestLength = 0.0;
	Vec4i longestVec;

	// Find longest line
	for (int i = 0; i < lines->size(); i++) {
		Vec4i vec = lineAdr[i];
		double vecLength = getLength(vec);
		if (vecLength > longestLength) {
			longestLength = vecLength;
			longestVec = vec;
		}
	}

	int x1 = longestVec[0];
	int y1 = longestVec[1];
	int x2 = longestVec[2];
	int y2 = longestVec[3];

	// printf("Vec: (%i, %i, %i, %i)\n", x1, y1, x2, y2);
	// printf("Length: %lf\n", longestLength);

	double ys = abs(y1 - y2);
	// printf("Ys %lf\n", ys);
	double xs = abs(x1 - x2);
	// printf("Xs %lf\n", xs);
	double ysxsdivided = ys/xs;
	// printf("Result: %lf\n", ysxsdivided);

	double radians = atan(ysxsdivided);
	double degrees = radians * 180/M_PI;
	double scale = 1;

	if (degrees < 90.0001 && degrees > 89.9999) {
		degrees = 0.0;
	}

	// printf("Tan Theta: %lf\n", ysxsdivided);
	// printf("Radians: %lf\n", radians);
	// printf("Should be rotating by %lf\n", degrees);

	Mat rotation = getRotationMatrix2D(center, -degrees, scale);

	// imshow("Original", *original);
	// waitKey(0);

	warpAffine(*original, *dst, rotation, dst->size(), INTER_LINEAR, BORDER_REPLICATE, 0);
	warpAffine(*original, *src, rotation, dst->size(), INTER_LINEAR, BORDER_REPLICATE, 0);

	if (show) {
		imshow("Rotated Image", *dst);
		waitKey(0);
	}

	delete lines;
	lines = NULL;

	// delete src;
	// src = NULL;

	return dst;
}

double getLength(Vec4i &vec) {
	double xlength = abs(vec[0] - vec[2]);
	double ylength = abs(vec[1] - vec[3]);

	return sqrt(xlength*xlength + ylength*ylength);
}

void printRotationMatrix(Mat &src) {
	float *rowPtr;
	for (int i = 0; i < src.rows; i++) {
		rowPtr = src.ptr<float>(i);
		for (int j = 0; j < src.cols; j++) {
			printf("%f, ", rowPtr[j]);
		}
		printf("\n");
	}
}

void scale(Mat* src) {
	int rows = src->rows;
	int cols = src->cols;

	if (rows == cols) {
		if (rows != 1500) {
			resize(*src, *src, Size(1500,1500));
		}
	}
}

void rotateBasedOnCircles(Mat *src) {
	vector<Vec3f> *circles = getHoughCircles(src, false);
	while(isThereCircleInUpperRightQuadrant(src, circles)) {
		rotate90CW(src);
		delete circles;
		circles = getHoughCircles(src, false);
	}
}

bool isThereCircleInUpperRightQuadrant(Mat *src, vector<Vec3f> *circles) {
	int cols = src->cols;
	int rows = src->rows;

	int halfCols = (int) cols/2;
	int halfRows = (int) rows/2;

	bool retVal = false;
	// printf("Circles size: %i\n", (int) circles->size());
	// printf("Should be greater than %i, and less than %i to be retried\n", halfCols, halfRows);
	for (int i = 0; i < circles->size(); i++) {
		Vec3f circ = circles->at(i);
		// printf("Circle values: (%lf, %lf))\n", circ[0], circ[1]);
		if ((circ[0] > halfCols) && (circ[1] < halfRows)) {

			retVal = true;
		}
	}

	return retVal;
}

void rotate90CW(Mat *src) {
	transpose(*src, *src);
	flip(*src, *src, 1);
}
