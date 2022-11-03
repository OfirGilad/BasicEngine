#include "assignment.h"
using namespace std;

vector<vector<int>>* gaussianKernel(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    vector<int> vec1 = { 1, 2, 1 };
    kernel->push_back(vec1);
    vector<int> vec2 = { 2, 4, 2 };
    kernel->push_back(vec2);
    vector<int> vec3 = { 1, 2, 1 };
    kernel->push_back(vec3);
    *div = 16;
    
    return kernel;
}

vector<vector<int>>* onesKernel(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    vector<int> vec1 = { 1, 1, 1 };
    kernel->push_back(vec1);
    vector<int> vec2 = { 1, 1, 1 };
    kernel->push_back(vec2);
    vector<int> vec3 = { 1, 1, 1 };
    kernel->push_back(vec3);
    *div = 9;
    
    return kernel;
}

vector<vector<int>>* dxKernel(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    vector<int> vec1 = { 0, 0, 0 };
    kernel->push_back(vec1);
    vector<int> vec2 = { 0, -1, 0 };
    kernel->push_back(vec2);
    vector<int> vec3 = { 0, 1, 0 };
    kernel->push_back(vec3);
    *div = 1;
    
    return kernel;
}

vector<vector<int>>* dyKernel(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    vector<int> vec1 = { 0, 0, 0 };
    kernel->push_back(vec1);
    vector<int> vec2 = { 0, -1, 1 };
    kernel->push_back(vec2);
    vector<int> vec3 = { 0, 0, 0 };
    kernel->push_back(vec3);
    *div = 1;
    
    return kernel;
}

void print_picture(vector<vector<unsigned char>>* mat, int width, int height) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            std::cout << mat->at(j).at(i) / 25 % 9;
        }
        std::cout << std::endl;
    }
}

vector<vector<unsigned char>>* applyFilter1(vector<vector<unsigned char>>* mat, int width, int height, vector<vector<int>>* kernel, int div) {
    unsigned char threshold = 35;
    vector<vector<unsigned char>>* newMat = new vector<vector<unsigned char>>();

    //upper end of the photo
    vector<unsigned char> upper;
    for (int i = 0; i < width; i++) {
        upper.push_back(i % 100 + 100);
    }
    newMat->push_back(upper);

    for (int j = 1; j < height - 1; j++) {
        vector<unsigned char> innerMat;
        innerMat.push_back(j % 100 + 100); // left most end of the photo
        for (int i = 1; i < width - 1; i++) {
            int temp = 0;
            temp += (*kernel)[0][0] * (*mat)[j - 1][i - 1];
            temp += (*kernel)[0][1] * (*mat)[j - 1][i];
            temp += (*kernel)[0][2] * (*mat)[j - 1][i + 1];
            temp += (*kernel)[1][0] * (*mat)[j][i - 1];
            temp += (*kernel)[1][1] * (*mat)[j][i];
            temp += (*kernel)[1][2] * (*mat)[j][i + 1];
            temp += (*kernel)[2][0] * (*mat)[j + 1][i - 1];
            temp += (*kernel)[2][1] * (*mat)[j + 1][i];
            temp += (*kernel)[2][2] * (*mat)[j + 1][i + 1];
            temp /= div;
            innerMat.push_back(abs(temp));
        }
        innerMat.push_back(j % 100 + 100); // right most end of the photo
        newMat->push_back(innerMat);
    }

    // lower end of the photo
    vector<unsigned char> lower;
    for (int i = 0; i < width; i++) {
        lower.push_back(i % 100 + 100);
    }
    newMat->push_back(lower);

    //print_picture(newMat, width, height);
    return newMat;
}

vector<vector<char>>* applyFilter2(vector<vector<unsigned char>>* mat, int width, int height, vector<vector<int>>* kernel, int div) {
    unsigned char threshold = 35;
    vector<vector<char>>* newMat = new vector<vector<char>>();

    //upper end of the photo
    vector<char> upper;
    for (int i = 0; i < width; i++) {
        upper.push_back(i % 100 + 100);
    }
    newMat->push_back(upper);

    for (int j = 1; j < height - 1; j++) {
        vector<char> innerMat;
        innerMat.push_back(j % 100 + 100); // left most end of the photo
        for (int i = 1; i < width - 1; i++) {
            int temp = 0;
            temp += (*kernel)[0][0] * (*mat)[j - 1][i - 1];
            temp += (*kernel)[0][1] * (*mat)[j - 1][i];
            temp += (*kernel)[0][2] * (*mat)[j - 1][i + 1];
            temp += (*kernel)[1][0] * (*mat)[j][i - 1];
            temp += (*kernel)[1][1] * (*mat)[j][i];
            temp += (*kernel)[1][2] * (*mat)[j][i + 1];
            temp += (*kernel)[2][0] * (*mat)[j + 1][i - 1];
            temp += (*kernel)[2][1] * (*mat)[j + 1][i];
            temp += (*kernel)[2][2] * (*mat)[j + 1][i + 1];
            temp /= div;
            innerMat.push_back(temp);
        }
        innerMat.push_back(j % 100 + 100); // right most end of the photo
        newMat->push_back(innerMat);
    }

    // lower end of the photo
    vector<char> lower;
    for (int i = 0; i < width; i++) {
        lower.push_back(i % 100 + 100);
    }
    newMat->push_back(lower);

    //print_picture(newMat, width, height);
    return newMat;
}

vector<vector<unsigned char>>* greyScaleImageConverter(unsigned char* data, int width, int height) {
    vector<vector<unsigned char>>* grey_scale_matrix = new vector<vector<unsigned char>>();
    for (int i = 0; i < height; i++) {
        vector <unsigned char> inner_row;
        for (int j = 0; j < width; j++) {
            int color_mean = (data[4 * (i * width + j)] + data[4 * (i * width + j) + 1] + data[4 * (i * width + j) + 2]) / 3;
            inner_row.push_back(color_mean);
        }
        grey_scale_matrix->push_back(inner_row);
    }
    return grey_scale_matrix;
}

vector<vector<unsigned char>>* matrixAddition(const vector<vector<char>>* dx, const vector<vector<char>>* dy, int width, int height) {
    vector<vector<unsigned char>>* added = new vector<vector<unsigned char>>();
    for (int i = 0; i < height; i++) {
        vector<unsigned char> innerVec;
        for (int j = 0; j < width; j++) {
            innerVec.push_back(min(255, abs((*dx)[i][j]) + abs((*dy)[i][j])));
        }
        added->push_back(innerVec);
    }
    return added;
}

// Exercise 4
unsigned char* Canny_Edge_Detector(unsigned char* data, int* width, int* height) {
    vector<vector<unsigned char>>* grey_scale_matrix = greyScaleImageConverter(data, *width, *height);
    
    // apply smoothing filter
    int div0;
    vector<vector<int>>* ones = gaussianKernel(&div0);
    vector<vector<unsigned char>>* smoothedPic = applyFilter1(grey_scale_matrix, *width, *height, ones, div0);

    // get derivative in x and y axis
    int div1;
    vector<vector<int>>* kernelX = dxKernel(&div1);
    int div2;
    vector<vector<int>>* kernelY = dyKernel(&div2);
    const vector<vector<char>>* dx = applyFilter2(smoothedPic, *width, *height, kernelX, div1);
    const vector<vector<char>>* dy = applyFilter2(smoothedPic, *width, *height, kernelY, div2);

    // combine the derivatives
    vector<vector<unsigned char>>* dx_plus_dy = matrixAddition(dx, dy, *width, *height);
    
    // non max suppression
    vector<vector<unsigned char>>* nms = non_max_suppression(dx_plus_dy, width, height);

    // threshold
    vector<vector<unsigned char>>* threshold_image = threshold(nms, width, height);
    
    // hysteresis
    vector<vector<unsigned char>>* hysteresis_image = hysteresis(threshold_image, width, height);
    
    // thin the lines
    //vector<vector<unsigned char>>* dfg = thinLines(dx, dy, dx_plus_dy, width, height);

    unsigned char* data_copy = (unsigned char*)(malloc(4 * (*width) * (*height)));

    if (data_copy != NULL) {
        for(int i = 0; i < *height; i++) {
            for(int j = 0; j < *width; j++) {
                //grey_scale_matrix[i][j] /= 2;
                data_copy[4 * (i * *width + j)] = (*hysteresis_image)[i][j];
                data_copy[4 * (i * *width + j) + sizeof(unsigned char)] = (*hysteresis_image)[i][j];
                data_copy[4 * (i * *width + j) + 2 * +sizeof(unsigned char)] = (*hysteresis_image)[i][j];
            }
        }
    }
    
    return data_copy;
}

bool inRange(int pos, int max) {
    return pos >= 0 && pos < max;
}

// https://towardsdatascience.com/canny-edge-detection-step-by-step-in-python-computer-vision-b49c3a2d8123

vector<vector<unsigned char>>* non_max_suppression(vector<vector<unsigned char>>* dx_plus_dy, int* width, int* height) {
    int M = *height;
    int N = *width;
    vector<vector<unsigned char>>* Z = new vector<vector<unsigned char>>();

    for (int i = 0; i < *height; i++) {
        vector<unsigned char> innerVec;
        for (int j = 0; j < *width; j++) {
            innerVec.push_back((unsigned char)0);
        }
        Z->push_back(innerVec);
    }
    
    vector<vector<unsigned char>> angle_matrix;

    for (int i = 0; i < *height; i++) {
        vector<unsigned char> innerVec;
        for (int j = 0; j < *width; j++) {
            unsigned char angle = (*dx_plus_dy)[i][j] * (unsigned char)180 / 3.14;
            if (angle < 0) {
                angle += (unsigned char)180;
            }
            innerVec.push_back(angle);
        }
        angle_matrix.push_back(innerVec);
    }

    for (int i = 1; i < M; i++) {
        for (int j = 1; j < N; j++) {
            try {
                unsigned char q = 255;
                unsigned char r = 255;

                //angle 0
                if ((0 <= (angle_matrix).at(i).at(j) < 22.5) || (157.5 <= (angle_matrix).at(i).at(j) <= 180)) {
                    if (inRange(i, M) && inRange(j + 1, N) && inRange(j - 1, N)) {
                        q = (*dx_plus_dy).at(i).at(j + 1);
                        r = (*dx_plus_dy).at(i).at(j - 1);
                    }
                }

                //angle 45
                else if (22.5 <= (angle_matrix).at(i).at(j) < 67.5) {
                    if (inRange(i + 1, M) && inRange(i - 1, M) && inRange(j + 1, N) && inRange(j - 1, N)) {
                        q = (*dx_plus_dy).at(i + 1).at(j - 1);
                        r = (*dx_plus_dy).at(i - 1).at(j + 1);
                    }
                }
                //angle 90
                else if (67.5 <= (angle_matrix).at(i + 1).at(j) < 112.5) {
                    if (inRange(i + 1, M) && inRange(i - 1, M) && inRange(j, N)) {
                        q = (*dx_plus_dy).at(i + 1).at(j);
                        r = (*dx_plus_dy).at(i - 1).at(j);
                    }
                }
                //angle 135
                else if (112.5 <= (angle_matrix)[i][j] < 157.5) {
                    if (inRange(i + 1, M) && inRange(i - 1, M) && inRange(j + 1, N) && inRange(j - 1, N)) {
                        q = (*dx_plus_dy).at(i - 1).at(j - 1);
                        r = (*dx_plus_dy).at(i + 1).at(j + 1);
                    }
                }
                if (((*dx_plus_dy).at(i).at(j) >= q) && ((*dx_plus_dy).at(i).at(j) >= r)) {
                    Z->at(i).at(j) = (*dx_plus_dy).at(i).at(j);
                }
                else {
                    Z->at(i).at(j) = (unsigned char)0;
                }
            }
            catch (...) {
                cout << 'pass' << endl;
            }
        }
    }
    return Z;
}

vector<vector<unsigned char>>* threshold(vector<vector<unsigned char>>* nms, int* width, int* height) {
    unsigned char highThreshold = 20;
    unsigned char lowThreshold = 5;

    int M = *height;
    int N = *width;
    vector<vector<unsigned char>>* res = new vector<vector<unsigned char>>();

    for (int i = 0; i < *height; i++) {
        vector<unsigned char> innerVec;
        for (int j = 0; j < *width; j++) {
            if ((*nms).at(i).at(j) >= highThreshold) {
                innerVec.push_back((unsigned char)255);
            }
            else if (lowThreshold <= (*nms).at(i).at(j) <= highThreshold) {
                innerVec.push_back((unsigned char)0);
            }
        }
        res->push_back(innerVec);
    }
    return res;
}

vector<vector<unsigned char>>* hysteresis(vector<vector<unsigned char>>* threshold_image, int* width, int* height) {
    unsigned char strong = (unsigned char)255;

    int M = *height;
    int N = *width;
    vector<vector<unsigned char>>* Z = new vector<vector<unsigned char>>();

    for (int i = 0; i < *height; i++) {
        vector<unsigned char> innerVec;
        for (int j = 0; j < *width; j++) {
            innerVec.push_back((unsigned char)0);
        }
        Z->push_back(innerVec);
    }

    for (int i = 1; i < M; i++) {
        for (int j = 1; j < N; j++) {
            if (threshold_image->at(i).at(j) == (unsigned char)255) {
                try {
                    if (threshold_image->at(i + 1).at(j - 1) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i + 1).at(j) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i + 1).at(j + 1) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i).at(j - 1) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i).at(j + 1) == strong) {

                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i - 1).at(j - 1) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i - 1).at(j) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i - 1).at(j + 1) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    cout << 'pass' << endl;
                }
            }
        }
    }
    return Z;
}

// No Use
vector<vector<unsigned char>>* thinLines(const vector<vector<char>>* dx, const vector<vector<char>>* dy, vector<vector<unsigned char>>* dx_plus_dy, int* width, int* height) {
    vector<vector<unsigned char>>* newMat = new vector<vector<unsigned char>>(*height);
    for (int vec = 0; vec < *height; vec++) {
        (*newMat)[vec] = *(new vector<unsigned char>(*width, 0));
    }
    unsigned char threshold = 10;

    for (int y = 0; y < *height; y++) {
        vector<unsigned char> inner = (*newMat)[y];
        for (int x = 0; x < *width; x++) {
            if ((*dx_plus_dy)[y][x] > threshold) {
                // check dx = 0
                if ((*dx)[y][x] == 0) {
                    if ((inRange(y + 1, *height) && (*dx_plus_dy)[y + 1][x] > (*dx_plus_dy)[y][x]) ||
                        (inRange(y - 1, *height) && (*dx_plus_dy)[y - 1][x] > (*dx_plus_dy)[y][x])) {
                        inner[x] = max((unsigned char)0, inner[x]);
                    }
                    else if ((inRange(y + 1, *height) && (*dx_plus_dy)[y + 1][x] == (*dx_plus_dy)[y][x]) ||
                        (inRange(y - 1, *height) && (*dx_plus_dy)[y - 1][x] == (*dx_plus_dy)[y][x])) {
                        int upperY = x;
                        int lowerY = x;
                        int step;

                        step = -1;
                        while (inRange(y + step, *height) &&
                            (*dx_plus_dy)[y + step][x] == (*dx_plus_dy)[y][x])
                            step--;
                        upperY += step;

                        step = 1;
                        while (inRange(y + step, *height) &&
                            (*dx_plus_dy)[y + step][x] == (*dx_plus_dy)[y][x]) {
                            step++;
                        }
                        lowerY += step;

                        // update value for current cell (0) and for the peak cell (the value from dx_plus_dy)
                        if ((upperY + lowerY) / 2 == y) {
                            inner[x] = max(inner[x], (unsigned char)255);//(*dx_plus_dy)[y][x]);
                        }
                        else {
                            inner[x] = max((unsigned char)0, inner[x]);
                        }
                        (*newMat)[(upperY + lowerY) / 2][x] = max((*newMat)[(upperY + lowerY) / 2][x], (*dx_plus_dy)[(upperY + lowerY) / 2][x]);
                        int u = 9;
                    }
                    else {
                        inner[x] = max(inner[x], (unsigned char)255);//(*dx_plus_dy)[y][x]);
                    }
                }
                else {
                    int slope = ((*dy)[y][x] / (*dx)[y][x]) % 3;
                    if ((inRange(y + slope, *height) && inRange(x + 1, *width) && (*dx_plus_dy)[y + slope][x + 1] > (*dx_plus_dy)[y][x]) ||
                        (inRange(y - slope, *height) && inRange(x - 1, *width) && (*dx_plus_dy)[y - slope][x - 1] > (*dx_plus_dy)[y][x])) {
                        inner[x] = max((unsigned char)0, inner[x]);
                    }
                    else if ((inRange(y + slope, *height) && inRange(x + 1, *width) && (*dx_plus_dy)[y + slope][x + 1] == (*dx_plus_dy)[y][x]) ||
                        (inRange(y - slope, *height) && inRange(x - 1, *width) && (*dx_plus_dy)[y - slope][x - 1] == (*dx_plus_dy)[y][x])) {
                        int leftX = x;
                        int rightX = x;
                        int step;

                        step = -1;
                        while (inRange(x + step, *width) && inRange(y + step * slope, *height) &&
                            (*dx_plus_dy)[y + step * slope][x + step] == (*dx_plus_dy)[y][x])
                            step--;
                        leftX += step;

                        step = 1;
                        while (inRange(x + step, *width) && inRange(y + step * slope, *height) &&
                            (*dx_plus_dy)[y + step * slope][x + step] == (*dx_plus_dy)[y][x]) {
                            step++;
                        }
                        rightX += step;

                        // update value for current cell (0) and for the peak cell (the value from dx_plus_dy)
                        if ((leftX + rightX) / 2 == x) {
                            inner[x] = max(inner[x], (unsigned char)255);//(*dx_plus_dy)[y][x]);
                        }
                        else {
                            inner[x] = max((unsigned char)0, inner[x]);
                        }
                        (*newMat)[y + ((leftX + rightX) / 2 - x) * slope][(leftX + rightX) / 2] =
                            max((*newMat)[y + ((leftX + rightX) / 2 - x) * slope][(leftX + rightX) / 2],
                                (*dx_plus_dy)[y + ((leftX + rightX) / 2 - x) * slope][(leftX + rightX) / 2]);
                        int t = 8;
                    }
                    else {
                        inner[x] = max(inner[x], (unsigned char)255);//(*dx_plus_dy)[y][x]);
                    }
                }
            }
        }
    }
    return newMat;
}

// Exercise 5
unsigned char* halftone(unsigned char* data, int* width, int* height) {
    vector<vector<unsigned char>>* grey_scale_matrix = greyScaleImageConverter(data, *width, *height);
    vector<vector<unsigned char>>* newMat = new vector<vector<unsigned char>>();
    for (int i = 0; i < *height; i++) {
        vector<unsigned char> inner1;
        vector<unsigned char> inner2;
        for (int j = 0; j < *width; j++) {
            if ((*grey_scale_matrix)[i][j] <= 50) {
                inner1.push_back(0);
                inner1.push_back(0);
                inner2.push_back(0);
                inner2.push_back(0);
            }
            else if ((*grey_scale_matrix)[i][j] <= 101) {
                inner1.push_back(0);
                inner1.push_back(0);
                inner2.push_back(255);
                inner2.push_back(0);
            }
            else if ((*grey_scale_matrix)[i][j] <= 152) {
                inner1.push_back(0);
                inner1.push_back(255);
                inner2.push_back(255);
                inner2.push_back(0);
            }
            else if ((*grey_scale_matrix)[i][j] <= 203) {
                inner1.push_back(0);
                inner1.push_back(255);
                inner2.push_back(255);
                inner2.push_back(255);
            }
            else {
                inner1.push_back(255);
                inner1.push_back(255);
                inner2.push_back(255);
                inner2.push_back(255);
            }
        }
        newMat->push_back(inner1);
        newMat->push_back(inner2);
    }

    *width *= 2;
    *height *= 2;
    unsigned char* data_copy = (unsigned char*)(malloc(4 * *width * *height));
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            //grey_scale_matrix[i][j] /= 2;
            data_copy[4 * (i * *width + j)] = (*newMat)[i][j];
            data_copy[4 * (i * *width + j) + 1] = (*newMat)[i][j];
            data_copy[4 * (i * *width + j) + 2] = (*newMat)[i][j];
        }
    }

    return data_copy;
}

// Exercise 6
unsigned char* Floyd_Steinberg_Algorithm(unsigned char* data, int* width, int* height) {
    vector<vector<unsigned char>>* mat = greyScaleImageConverter(data, *width, *height);
    vector<vector<unsigned char>>* newMat = new vector<vector<unsigned char>>();

	for (int y = 0; y < *height; y++) {
        vector<unsigned char> inner;
        for (int x = 0; x < *width; x++) {
            unsigned char ktr = 255;
            unsigned char val = newTrunc(min((*mat)[y][x], ktr));
            inner.push_back(val);
            unsigned char e = (*mat)[y][x] - val;
            if(x == 0 && y < *height - 1)
                extraStuff(e, x, y, mat, 8.0/16, 2.0/16, 6.0/16, 0.0); // left not bottom
            else if(x < *width - 1 && y == *height - 1)
                extraStuff(e, x, y, mat, 1.0, 0.0, 0.0, 0.0); // left or middle bottom
            else if(x == *width - 1 && y < *height - 1)
                extraStuff(e, x, y, mat, 0.0, 0.0, 10.0/16, 6.0/16); // right not bottom
            else if (x == *width - 1 && y == *height - 1)
                extraStuff(e, x, y, mat, 0.0, 0.0, 0.0, 0.0); // right bottom
            else
                extraStuff(e, x, y, mat, 7.0/16, 1.0/16, 5.0/16, 3.0/16); // middle not bottom 
		}
		newMat->push_back(inner);
	}

    unsigned char* data_copy = (unsigned char*)(malloc(4 * *width * *height));
    if (data_copy != NULL) {
        for (int i = 0; i < *height; i++) {
            for (int j = 0; j < *width; j++) {
                //grey_scale_matrix[i][j] /= 2;
                data_copy[4 * (i * *width + j)] = (*newMat)[i][j];
                data_copy[4 * (i * *width + j) + 1] = (*newMat)[i][j];
                data_copy[4 * (i * *width + j) + 2] = (*newMat)[i][j];
            }
        }
    }

    return data_copy;
}

void extraStuff(unsigned char e, int xPos, int yPos, vector<vector<unsigned char>>* mat, float alpha, float beta, float gama, float delta){
	if (alpha > 0.01){
		(*mat)[yPos][xPos + 1] += alpha * e;
    }
    if (beta > 0.01){
		(*mat)[yPos + 1][xPos + 1] += beta * e;
    }
	if (gama > 0.01) {
		(*mat)[yPos + 1][xPos] += gama * e;
	}
	if (delta > 0.01) {
		(*mat)[yPos + 1][xPos - 1] += delta * e;
	}
}

unsigned char newTrunc(unsigned char pixel_value) {
    unsigned char return_value = 0;
    if (pixel_value < 128) {
		return_value = 255;
    }
    return return_value;
}