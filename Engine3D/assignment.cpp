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

vector<vector<unsigned char>>* applyFilter(vector<vector<unsigned char>>* mat, int width, int height, vector<vector<int>>* kernel, int div) {
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

    print_picture(newMat, width, height);
    return newMat;
}

vector<vector<unsigned char>>* greyScalePicture2Dim(unsigned char* data, int width, int height) {
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

vector<vector<unsigned char>>* matrixAddition(vector<vector<unsigned char>>* dx, vector<vector<unsigned char>>* dy, int width, int height) {
    vector<vector<unsigned char>>* added = new vector<vector<unsigned char>>();
    for (int i = 0; i < height; i++) {
        vector<unsigned char> innerVec;
        for (int j = 0; j < width; j++) {
            innerVec.push_back(min(255, (*dx)[i][j] + (*dy)[i][j]));
        }
        added->push_back(innerVec);
    }
    return added;
}

unsigned char* halftonePic(unsigned char* data, int* width, int* height) {
    vector<vector<unsigned char>>* grey_scale_matrix = greyScalePicture2Dim(data, *width, *height);
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
    unsigned char* data2 = (unsigned char*)(malloc(4 * *width * *height));
    for (int i = 0; i < *height; i++) {
        for (int j = 0; j < *width; j++) {
            //grey_scale_matrix[i][j] /= 2;
            data2[4 * (i * *width + j)] = (*newMat)[i][j];
            data2[4 * (i * *width + j) + 1] = (*newMat)[i][j];
            data2[4 * (i * *width + j) + 2] = (*newMat)[i][j];
        }
    }
    return data2;
}

unsigned char* edgeDetection(unsigned char* data, int* width, int* height) {
    vector<vector<unsigned char>>* grey_scale_matrix = greyScalePicture2Dim(data, *width, *height);
    int div0;
    vector<vector<int>>* ones = gaussianKernel(&div0);
    vector<vector<unsigned char>>* smoothedPic = applyFilter(grey_scale_matrix, *width, *height, ones, div0);

    int div1;
    vector<vector<int>>* kernelX = dxKernel(&div1);
    int div2;
    vector<vector<int>>* kernelY = dyKernel(&div2);

    vector<vector<unsigned char>>* dx = applyFilter(smoothedPic, *width, *height, kernelX, div1);
    vector<vector<unsigned char>>* dy = applyFilter(smoothedPic, *width, *height, kernelY, div2);

    vector<vector<unsigned char>>* dx_plus_dy = matrixAddition(dx, dy, *width, *height);

    unsigned char* data2 = (unsigned char*)(malloc(4 * *width * *height));
    if (data2 != NULL) {
        for(int i = 0; i < *height; i++) {
            for(int j = 0; j < *width; j++) {
                //grey_scale_matrix[i][j] /= 2;
                data2[4 * (i * *width + j)] = (*dx_plus_dy)[i][j];
                data2[4 * (i * *width + j) + 1] = (*dx_plus_dy)[i][j];
                data2[4 * (i * *width + j) + 2] = (*dx_plus_dy)[i][j];
            }
        }
    }
    
    return data2;
}

unsigned char* FloydSteinbergAlgorithm(unsigned char* data, int* width, int* height) {
    vector<vector<unsigned char>>* mat = greyScalePicture2Dim(data, *width, *height);
    vector<vector<unsigned char>>* newMat = new vector<vector<unsigned char>>();
    int alpha, beta, gama, delta;

	for (int y = 0; y < *height; y++) {
        vector<unsigned char> inner;
        for (int x = 0; x < *width; x++) {
            unsigned char val = newTrunc((*mat)[y][x]);
            inner.push_back(val);
            unsigned char e = (*mat)[y][x] - val;
            if(x == 0 && y < *height - 1)
                extraStuff(e, x, y, mat, 0.4, 0.2, 0.4, 0.0); // left not bottom
            else if(x < *width - 1 && y == *height - 1)
                extraStuff(e, x, y, mat, 1.0, 0.0, 0.0, 0.0); // left or middle bottom
            else if(x == *width - 1 && y < *height - 1)
                extraStuff(e, x, y, mat, 0.0, 0.0, 0.6, 0.4); // right not bottom
            else if (x == *width - 1 && y == *height - 1)
                extraStuff(e, x, y, mat, 0.0, 0.0, 0.0, 0.0); // right bottom
            else
                extraStuff(e, x, y, mat, 0.35, 0.15, 0.35, 0.15); // middle not bottom 
		}
		newMat->push_back(inner);
	}

    unsigned char* data2 = (unsigned char*)(malloc(4 * *width * *height));
    if (data2 != NULL) {
        for (int i = 0; i < *height; i++) {
            for (int j = 0; j < *width; j++) {
                //grey_scale_matrix[i][j] /= 2;
                data2[4 * (i * *width + j)] = (*newMat)[i][j];
                data2[4 * (i * *width + j) + 1] = (*newMat)[i][j];
                data2[4 * (i * *width + j) + 2] = (*newMat)[i][j];
            }
        }
    }

    return data2;
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