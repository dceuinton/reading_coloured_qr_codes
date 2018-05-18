
#include "a2qrcode.h"

char keyArray[64]={' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','x','y','w','z',
'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','X','Y','W','Z',
'0','1','2','3','4','5','6','7','8','9','.'};

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

vector<int> getAnchor(Mat *src) {
	Point ret;
	vector<Vec4i> *lines = getHoughLines(src);
	vector<Vec3f> *circles = getHoughCircles(src, false);
	int boxSide = getBoxWidth(lines, src->rows, src->cols);
	Vec4i highestLine = getHighestVec(lines, src->rows, src->cols);

	int leftEdge = (highestLine[0] < highestLine[2]) ? highestLine[0] : highestLine[2];
	int whiteThreshold = 50;

	int yFinal = highestLine[1];
	int xFinal = leftEdge;

	Mat test = *src;
	
	ret.x = xFinal;
	ret.y = yFinal;

	Point circleCenter;

	for (int i = 0; i < circles->size(); i++) {
		Vec3f circ = circles->at(i);
		if (circ[0] < src->cols/2 && circ[1] < src->rows/2) {
			circleCenter.x = circ[0];
			circleCenter.y = circ[1];
		}
	}

	circleCenter.x += (int) 3*boxSide;
	circleCenter.y -= (int) 3*boxSide;

	circleCenter.x += (int) boxSide/2;
	circleCenter.y += (int) boxSide/2;

	vector<int> important = {circleCenter.x, circleCenter.y, boxSide};
	
	return important;
}

void checkPush(uchar val, vector<int> &values) {
	
	uchar margin = 120;

	uchar lower = 0 + margin;
	uchar upper = 255 - margin;

	// printf("Val: %u\n", val);

	if (val < lower) {
		values.push_back(0);
		// printf("Going in as 0\n\n");
	} else if (val > upper) {
		// printf("Goint in as 1\n\n");
		values.push_back(1);
	} else {
		printf("ERROR: Margin or read failed at index %i with val: %u\n", (int) values.size()+1, val);
		return;
	}
}

vector<int> sampleSegment(Mat *src) {
	vector<int> importantInfo = getAnchor(src);
	vector<int> output;

	int xAnchor = importantInfo[0];
	int yAnchor = importantInfo[1];
	int step = importantInfo[2];

	if (step % 2 != 0) {
		step++;
	}

	uchar b, g, r;
	int xStart = xAnchor;
	int yStart = yAnchor;

	// printf("Step: %i\n", step);

	uchar *row;
	for (int i = 0; i < SEG1_H; i++) {
	// for (int i = 0; i < 1; i++) {
		row = src->ptr<uchar>(yStart + i*step);
		for (int j = 0; j < SEG1_W; j++) {
		// for (int j = 0; j < 6; j++) {
			b = row[(xStart + j*step)*src->channels() ];
			g = row[(xStart + j*step)*src->channels() + 1 ];
			r = row[(xStart + j*step)*src->channels() + 2 ];

			// printf("BGR: (%u, %u, %u)\n", b, g, r);

			checkPush(b, output);
			checkPush(g, output);
			checkPush(r, output);

			Point center(xStart + j*step, yStart + i*step);
			circle(*src, center, 3, Scalar(0, 0, 255), 3);

			// printf("XY: (%i, %i))\n", xStart + j*step, yStart + i*step);
		}
	}

	// printBinary(output);

	xStart -= 6*step;
	yStart += 6*step;

	for (int i = 0; i < SEG2_H; i++) {
	// for (int i = 0; i < 1; i++) {
		row = src->ptr<uchar>(yStart + i*step);
		for (int j = 0; j < SEG2_W; j++) {
			b = row[(xStart + j*step)*src->channels() ];
			g = row[(xStart + j*step)*src->channels()  + 1];
			r = row[(xStart + j*step)*src->channels()  + 2];

			checkPush(b, output);
			checkPush(g, output);
			checkPush(r, output);

			Point center(xStart + j*step, yStart + i*step);
			circle(*src, center, 3, Scalar(0, 0, 255), 3);
		}
	}

	xStart += 6*step;
	yStart += 35*step;

	for (int i = 0; i < SEG3_H; i++) {
	// for (int i = 0; i < 1; i++) {
		row = src->ptr<uchar>(yStart + i*step);
		for (int j = 0; j < SEG3_W; j++) {
			b = row[(xStart + j*step)*src->channels() ];
			g = row[(xStart + j*step)*src->channels()  + 1];
			r = row[(xStart + j*step)*src->channels()  + 2];

			checkPush(b, output);
			checkPush(g, output);
			checkPush(r, output);

			Point center(xStart + j*step, yStart + i*step);
			circle(*src, center, 3, Scalar(0, 0, 255), 3);
		}
	}

	return output;
}

void printBinary(vector<int> &binary) {
	for (int i = 0; i < (int) binary.size()/6; i++) {
		int index = 6*i;
		printf("(%i, %i, %i, %i, %i, %i)\n", binary[index], binary[index+1], binary[index+2], binary[index+3], binary[index+4], binary[index+5]);
	}
}

void printMessage(vector<char> &message) {
	for (int i = 0; i < message.size(); i++) {
		printf("%c", message.at(i));
	}
	printf("\n");
}
void deleteBinaryNumbersInVector(vector<BinaryNumber*> &vec) {
	for (int i = 0; i < vec.size(); i++) {
		delete vec.at(i);
	}
}

void readMessage(vector<int> &binary) {

	vector<BinaryNumber*> numbers;

	for (int i = 0; i < (int) binary.size()/6; i++) {
		int counter = 6*i;
		BinaryNumber *bin = new BinaryNumber(binary.at(counter), binary.at(counter+1), binary.at(counter+2), binary.at(counter+3), binary.at(counter+4), binary.at(counter+5));
		
		// printf("Bin number: %i\n", bin->number);
		numbers.push_back(bin);
	}

	vector<char> message;

	for (int i = 0; i < numbers.size(); i++) {
		message.push_back(keyArray[numbers.at(i)->number]);
	}

	printMessage(message);

	deleteBinaryNumbersInVector(numbers);

}

