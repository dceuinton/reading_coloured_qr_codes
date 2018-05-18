
#include "a2qrcode.h"

int getBoxWidth(vector<Vec4i> *lines, int rows, int cols) {
	vector<Vec4i> &linesAdr = *lines;

	Vec4i highestHorizontal     = {cols, rows, cols, rows};
	Vec4i nextHighestHorizontal = {cols, rows, cols, rows};

	for (int i = 0; i < lines->size(); i++) {
		Vec4i line = linesAdr[i];
		if (isHorizontalWithTolerance(line)) {
			if (middleHeight(line) < middleHeight(highestHorizontal)) {
				highestHorizontal = line;
			} /*else if (middleHeight(line) == middleHeight(highestHorizontal)) {
				if (leftMostPoint(line) < leftMostPoint(highestHorizontal)) {
					highestHorizontal = line;
				}
			}*/
		}
	}

	for (int i = 0; i < lines->size(); i++) {
		int errorGap = 5;
		Vec4i line = linesAdr[i];
		if (isHorizontalWithTolerance(line)) {
			if (middleHeight(line) < middleHeight(nextHighestHorizontal)) {
				if (abs(middleHeight(line) - middleHeight(highestHorizontal)) > errorGap) {
					nextHighestHorizontal = line;
				}	
			} 
		}
	}

	printf("Highest Vector Heights, %i, %i\n", highestHorizontal[1], highestHorizontal[3]);
	printf("Next Highest Vector Heights, %i, %i\n", nextHighestHorizontal[1], nextHighestHorizontal[3]);

	return middleHeight(nextHighestHorizontal) - middleHeight(highestHorizontal);
}

Vec4i getHighestVec(vector<Vec4i> *lines, int rows, int cols) {
	vector<Vec4i> &linesAdr = *lines;

	Vec4i highestHorizontal = {cols, rows, cols, rows};

	for (int i = 0; i < lines->size(); i++) {
		Vec4i line = linesAdr[i];
		if (isHorizontalWithTolerance(line)) {
			if (middleHeight(line) < middleHeight(highestHorizontal)) {
				highestHorizontal = line;
			}
		}
	}
	return highestHorizontal;
}

bool isHorizontalWithTolerance(Vec4i &vec) {
	int tolerance = 1;
	if (abs(vec[1] - vec[3]) < tolerance) {
		return true;
	} else {
		return false;	
	}	
}

int middleHeight(Vec4i &vec) {
	int result = (int) (vec[1] + vec[3])/2;
	return result;
}

int leftMostPoint(Vec4i &vec) {
	return (vec[0] < vec[2]) ? vec[0] : vec[2];
}

