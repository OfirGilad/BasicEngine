#include "assignment.h"
#include <iostream>
#include <fstream>


// Canny_Edge_Detector (Improved)
void setPixel(unsigned char* data, int i, int j, int value, int width) {
    for (int k = 0; k < 3; k++)
        data[4 * (i + j * width) + k] = value;
    data[4 * (i + j * width) + 3] = 255;
}

unsigned char getPixel(unsigned char* data, int i, int j, int width) {
    return data[4 * (i + j * width)];
}

void setPixel(int* data, int i, int j, int value, int width) {
    data[i + j * width] = value;
}

int getPixel(int* data, int i, int j, int width) {
    return data[i + j * width];
}

int* pictureToIntArray(unsigned char* data, int width, int height) {
    int* n_data = (int*)malloc(sizeof(int) * width * height);
    for (int i = 0; i < width * height; i++)
        n_data[i] = (data[i * 4] + data[i * 4 + 1] + data[i * 4 + 2]) / 3;
    return n_data;
}

unsigned char* intArrayToPicture(int* data, int width, int height) {
    unsigned char* n_data = (unsigned char*)malloc(sizeof(unsigned char*) * width * height * 4);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (data[i + j * width] < 0)
                setPixel(n_data, i, j, 0, width);
            else if (data[i + j * width] > 255)
                setPixel(n_data, i, j, 255, width);
            else
                setPixel(n_data, i, j, data[i + j * width], width);
    return n_data;
}

int* applyFilter(int* data, int filter[9], int filter_size, int width, int height) {
    int* filtered_data = (int*)malloc(sizeof(int) * width * height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int filter_sum = 0;
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    if ((i + m < height) && (i + m >= 0) && (j + n < width) && (j + n >= 0)) {
                        filter_sum += getPixel(data, i + m, j + n, width) * filter[(m + 1) + 3 * (n + 1)];
                    }
                }
            }

            filter_sum /= filter_size;
            setPixel(filtered_data, i, j, filter_sum, width);
        }
    }
    return filtered_data;
}

unsigned char* Canny_Edge_Detector_Improved(unsigned char* data, int width, int height) {
    // Init variables
    float pi = 3.14159265358;
    int filter1[] = { 
        1, 2, 1,
        2, 4, 2,
        1, 2, 1
    };
    int filter2[] = { 
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    };
    int filter3[] = {
         1,  2,  1,
         0,  0,  0,
        -1, -2, -1 
    };
    int* magnitude = (int*)malloc(sizeof(int) * width * height);
    int* non_max_suppression = (int*)malloc(sizeof(int) * width * height);
    int* thresholding = (int*)malloc(sizeof(int) * width * height);
    int* hysteresis = (int*)malloc(sizeof(int) * width * height);
    double* angles = (double*)malloc(sizeof(double) * width * height);

    // Convert to GrayScale
    int* int_data = pictureToIntArray(data, width, height);

    // Gussian Blur
    int* s_data = applyFilter(int_data, filter1, 16, width, height);

    // Magnitude
    int* x_div = applyFilter(s_data, filter2, 1, width, height);
    int* y_div = applyFilter(s_data, filter3, 1, width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int x = (int)getPixel(x_div, i, j, width);
            int y = (int)getPixel(y_div, i, j, width);
            int temp = sqrt(x * x + y * y);
            setPixel(magnitude, i, j, temp, width);
            angles[i + j * width] = atan2(y, x) * 180 / pi;
        }
    }

    // Non Max Suppression
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int direction = ((int)(angles[i + j * width] + 22.5 + 180) / 45) % 4;
            int current_pixel = getPixel(magnitude, i, j, width);

            if ((i == 0) || (j == 0) || (i == 255) || (j == 255)) {
                setPixel(non_max_suppression, i, j, 0, width);
            }
            else if ((direction == 0) && (current_pixel > getPixel(magnitude, i - 1, j, width)) && (current_pixel > getPixel(magnitude, i + 1, j, width))) {
                setPixel(non_max_suppression, i, j, current_pixel, width);
            }
            else if ((direction == 1) && (current_pixel > getPixel(magnitude, i + 1, j - 1, width)) && (current_pixel > getPixel(magnitude, i - 1, j + 1, width))) {
                setPixel(non_max_suppression, i, j, current_pixel, width);
            }
            else if ((direction == 2) && (current_pixel > getPixel(magnitude, i, j - 1, width)) && (current_pixel > getPixel(magnitude, i, j + 1, width))) {
                setPixel(non_max_suppression, i, j, current_pixel, width);
            }
            else if ((direction == 3) && (current_pixel > getPixel(magnitude, i - 1, j - 1, width)) && (current_pixel > getPixel(magnitude, i + 1, j + 1, width))) {
                setPixel(non_max_suppression, i, j, current_pixel, width);
            }
            else {
                setPixel(non_max_suppression, i, j, 0, width);
            }
        }
    }

    // Double Thresholding
    int high = 160;
    int low = 60;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (getPixel(non_max_suppression, i, j, width) > high) {
                setPixel(thresholding, i, j, 255, width);
            }
            else if (getPixel(non_max_suppression, i, j, width) > low) {
                setPixel(thresholding, i, j, 1, width);
            }
            else {
                setPixel(thresholding, i, j, 0, width);
            }
        }
    }

    // Hysteresis
    int strong = 255;
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            if (getPixel(thresholding, i, j, width) > 0) {
                bool keep_pixel = false;
                for (int m = -1; m <= 1; m++) {
                    for (int n = -1; n <= 1; n++) {
                        if (m != 0 || n != 0) {
                            if (getPixel(thresholding, i + m, j + n, width) == strong) {
                                setPixel(hysteresis, i, j, 255, width);
                                keep_pixel = true;
                            }
                        }
                    }
                }
                if (keep_pixel == false) {
                    setPixel(hysteresis, i, j, 0, width);
                }
            }
        }
    }

    unsigned char* final_picture = intArrayToPicture(hysteresis, width, height);

    // write to file
    ofstream outfile;
    outfile.open("../img4.txt");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            outfile << ((int)final_picture[4 * (i * width + j)] + 1) / 256 << ",";
        }
        outfile << endl;
    }

    return final_picture;
}

// Util Functions
void writeToFile(const string& fileName, vector<vector<unsigned char>>* mat, int width, int height, int div) {
    // open a file in write mode
    ofstream outfile;
    outfile.open(fileName);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            outfile << ((int)(((*mat)[i][j])) + 1) / div << ",";
        }
        outfile << endl;
    }
}

void print_picture(vector<vector<unsigned char>>* mat, int width, int height) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            std::cout << mat->at(j).at(i) / 25 % 9;
        }
        std::cout << std::endl;
    }
}

vector<vector<int>>* greyScaleImageConverter(unsigned char* data, int width, int height) {
    vector<vector<int>>* grey_scale_matrix = new vector<vector<int>>();

    for (int i = 0; i < height; i++) {
        vector<int> inner_row;
        for (int j = 0; j < width; j++) {
            //int color_mean = (data[4 * (i * width + j)] + data[4 * (i * width + j) + 1] + data[4 * (i * width + j) + 2]) / 3;
            int color_mean = (data[4 * (i * width + j)] + data[4 * (i * width + j) + 1] + data[4 * (i * width + j) + 2]) / 3;
            inner_row.push_back(color_mean);
        }
        grey_scale_matrix->push_back(inner_row);
    }
    return grey_scale_matrix;
}

// Canny Edge Detector Util Functions
vector<vector<int>>* gaussianKernel3x3(int* div) {
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

vector<vector<int>>* dxKernel1(int* div) {
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

vector<vector<int>>* dyKernel1(int* div) {
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

vector<vector<int>>* dxKernel2(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    vector<int> vec1 = { -1, 0, 1 };
    kernel->push_back(vec1);
    vector<int> vec2 = { -2, 0, 2 };
    kernel->push_back(vec2);
    vector<int> vec3 = { -1, 0, 1 };
    kernel->push_back(vec3);
    *div = 1;

    return kernel;
}

vector<vector<int>>* dyKernel2(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    vector<int> vec1 = { 1, 2, 1 };
    kernel->push_back(vec1);
    vector<int> vec2 = { 0, 0, 0 };
    kernel->push_back(vec2);
    vector<int> vec3 = { -1, -2, -1 };
    kernel->push_back(vec3);
    *div = 1;

    return kernel;
}

vector<vector<int>>* applyFilter3x3_1(vector<vector<int>>* mat, int width, int height, vector<vector<int>>* kernel, int div) {
    vector<vector<int>>* newMat = new vector<vector<int>>();

    // Init newMat
    for (int i = 0; i < height; i++) {
        vector<int> innerVec(width, 0);
        newMat->push_back(innerVec);
    }

    for (int j = 1; j < height - 1; j++) {
        for (int i = 1; i < width - 1; i++) {
            int filterSum = 0;
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    filterSum += (*mat)[j + m][i + n] * (*kernel)[m + 1][n + 1];
                }
            }
            (*newMat)[j][i] = (filterSum / div);
        }
    }

    //print_picture(newMat, width, height);
    return newMat;
}

vector<vector<int>>* applyFilter3x3_2(vector<vector<int>>* mat, int width, int height, vector<vector<int>>* kernel, int div) {
    vector<vector<int>>* newMat = new vector<vector<int>>();

    // Init newMat
    for (int i = 0; i < height; i++) {
        vector<int> innerVec(width, 0);
        newMat->push_back(innerVec);
    }

    for (int j = 2; j < height - 2; j++) {
        for (int i = 2; i < width - 2; i++) {
            int filterSum = 0;
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    filterSum += (*mat)[j + m][i + n] * (*kernel)[m + 1][n + 1];
                }
            }
            (*newMat)[j][i] = (filterSum / div);
        }
    }

    //print_picture(newMat, width, height);
    return newMat;
}

vector<vector<int>>* calculate_magnitude(const vector<vector<int>>* dx, const vector<vector<int>>* dy, int width, int height) {
    vector<vector<int>>* magnitude = new vector<vector<int>>();

    for (int i = 0; i < height; i++) {
        vector<int> innerVec;
        for (int j = 0; j < width; j++) {
            int sum_result = (int)sqrt((int)(*dx)[i][j] * (int)(*dx)[i][j] + (int)(*dy)[i][j] * (int)(*dy)[i][j]);
            //int sum_result = abs((*dx)[i][j]) + abs((*dy)[i][j]);

            sum_result = min(255, max(0, sum_result));
            innerVec.push_back(sum_result);
        }
        magnitude->push_back(innerVec);
    }
    return magnitude;
}

vector<vector<int>>* non_max_suppression(vector<vector<int>>* magnitude, int width, int height) {
    vector<vector<int>>* newMat = new vector<vector<int>>();

    // Init newMat
    for (int i = 0; i < height; i++) {
        vector<int> innerVec(width, 0);
        newMat->push_back(innerVec);
    }

    vector<vector<int>> angle_matrix;

    for (int i = 0; i < height; i++) {
        vector<int> innerVec;
        for (int j = 0; j < width; j++) {
            double angle = (*magnitude)[i][j] * (180 / 3.14);
            if (angle < 0) {
                angle += 180;
            }
            innerVec.push_back(angle);
        }
        angle_matrix.push_back(innerVec);
    }

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int q = 255;
            int r = 255;

            // angle 0
            if ((0 <= (angle_matrix)[i][j] < 22.5) || (157.5 <= (angle_matrix)[i][j] <= 180)) {
                q = (*magnitude)[i][j + 1];
                r = (*magnitude)[i][j - 1];
            }
            // angle 45
            else if (22.5 <= (angle_matrix)[i][j] < 67.5) {  
                q = (*magnitude)[i + 1][j - 1];
                r = (*magnitude)[i - 1][j + 1];
            }
            // angle 90
            else if (67.5 <= (angle_matrix)[i][j] < 112.5) {    
                q = (*magnitude)[i + 1][j];
                r = (*magnitude)[i - 1][j];
            }
            // angle 135
            else if (112.5 <= (angle_matrix)[i][j] < 157.5) { 
                q = (*magnitude)[i - 1][j - 1];
                r = (*magnitude)[i + 1][j + 1];
            }

            if (((*magnitude)[i][j] >= q) && ((*magnitude)[i][j] >= r)) {
                (*newMat)[i][j] = (*magnitude)[i][j];
            }
            else {
                (*newMat)[i][j] = 0;
            }
        }
    }
    return newMat;
}

vector<vector<int>>* threshold(vector<vector<int>>* nms, int width, int height) {
    // Option 1
    int highThreshold = 26;
    int lowThreshold = 5;

    // Option 1
    //int highThreshold = 150;
    //int lowThreshold = 50;

    vector<vector<int>>* newMat = new vector<vector<int>>();

    for (int i = 0; i < height; i++) {
        vector<int> innerVec;
        for (int j = 0; j < width; j++) {
            if ((*nms)[i][j] >= highThreshold) {
                innerVec.push_back(255);
            }
            else if ((lowThreshold <= (*nms)[i][j]) && ((*nms)[i][j] < highThreshold)) {
                innerVec.push_back(1);
            }
            else {
                innerVec.push_back(0);
            }
        }
        newMat->push_back(innerVec);
    }
    return newMat;
}

vector<vector<unsigned char>>* hysteresis(vector<vector<int>>* threshold_image, int width, int height) {
    int strong = 255;

    // Init newMat
    vector<vector<unsigned char>>* newMat = new vector<vector<unsigned char>>();
    for (int i = 0; i < height; i++) {
        vector<unsigned char> innerVec(width, 0);
        newMat->push_back(innerVec);
    }

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            if (threshold_image->at(i).at(j) > 0) {
                for (int m = -1; m <= 1; m++) {
                    for (int n = -1; n <= 1; n++) {
                        if (m != 0 || n != 0) {
                            if (threshold_image->at(i + m).at(j + n) == strong) {
                                newMat->at(i).at(j) = (unsigned char)strong;
                            }
                        }
                    }
                }
            }
        }
    }
    return newMat;
}

// https://towardsdatascience.com/canny-edge-detection-step-by-step-in-python-computer-vision-b49c3a2d8123

// Exercise 4
unsigned char* Canny_Edge_Detector(unsigned char* data, int width, int height) {
    vector<vector<int>>* grey_scale_matrix = greyScaleImageConverter(data, width, height);

    // apply smoothing filter
    int div0, div1, div2;
    vector<vector<int>>* kernelG = gaussianKernel3x3(&div0);
    vector<vector<int>>* smoothedPic = applyFilter3x3_1(grey_scale_matrix, width, height, kernelG, div0);

    // get derivative in x and y axis
    // Option 1
    vector<vector<int>>* kernelX = dxKernel1(&div1);
    vector<vector<int>>* kernelY = dyKernel1(&div2);

    // Option 2
    //vector<vector<int>>* kernelX = dxKernel2(&div1);
    //vector<vector<int>>* kernelY = dyKernel2(&div2);

    const vector<vector<int>>* dx = applyFilter3x3_2(smoothedPic, width, height, kernelX, div1);
    const vector<vector<int>>* dy = applyFilter3x3_2(smoothedPic, width, height, kernelY, div2);

    // combine the derivatives
    vector<vector<int>>* magnitude_image = calculate_magnitude(dx, dy, width, height);

    // non max suppression
    vector<vector<int>>* nms_image = non_max_suppression(magnitude_image, width, height);

    // threshold
    vector<vector<int>>* threshold_image = threshold(nms_image, width, height);

    // hysteresis
    vector<vector<unsigned char>>* hysteresis_image = hysteresis(threshold_image, width, height);

    // write to file
    writeToFile("../img4.txt", hysteresis_image, width, height, 256);

    unsigned char* data_copy = (unsigned char*)(malloc(4 * (width) * (height)));

    if (data_copy != NULL) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                data_copy[4 * (i * width + j)] = (*hysteresis_image)[i][j];
                data_copy[4 * (i * width + j) + 1] = (*hysteresis_image)[i][j];
                data_copy[4 * (i * width + j) + 2] = (*hysteresis_image)[i][j];
                data_copy[4 * (i * width + j) + 3] = (unsigned char)255;
            }
        }
    }

    return data_copy;
}

// Exercise 5
unsigned char* Halftone(unsigned char* data, int width, int height) {
    vector<vector<int>>* grey_scale_matrix = greyScaleImageConverter(data, width, height);
    vector<vector<unsigned char>>* newMat = new vector<vector<unsigned char>>();

    for (int i = 0; i < height; i++) {
        vector<unsigned char> inner1;
        vector<unsigned char> inner2;
        for (int j = 0; j < width; j++) {
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

    width *= 2;
    height *= 2;

    // write to file
    writeToFile("../img5.txt", newMat, width, height, 256);

    unsigned char* data_copy = (unsigned char*)(malloc(4 * width * height));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            data_copy[4 * (i * width + j)] = (*newMat)[i][j];
            data_copy[4 * (i * width + j) + 1] = (*newMat)[i][j];
            data_copy[4 * (i * width + j) + 2] = (*newMat)[i][j];
            data_copy[4 * (i * width + j) + 3] = (unsigned char)255;
        }
    }

    return data_copy;
}

// Floyd Steinberg Algorithm Util Functions
void error_pass(unsigned char e, int xPos, int yPos, vector<vector<int>>* mat, float alpha, float beta, float gama, float delta) {
    if (alpha > 0.01) {
        (*mat)[yPos][xPos + 1] += alpha * e;
    }
    if (beta > 0.01) {
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
    return pixel_value - (pixel_value % 16);
}

// Exercise 6
unsigned char* Floyd_Steinberg_Algorithm(unsigned char* data, int width, int height) {
    vector<vector<int>>* mat = greyScaleImageConverter(data, width, height);
    vector<vector<unsigned char>>* newMat = new vector<vector<unsigned char>>();

    for (int y = 0; y < height; y++) {
        vector<unsigned char> inner;
        for (int x = 0; x < width; x++) {
            unsigned char ktr = 255;
            unsigned char val = newTrunc(min((unsigned char)(*mat)[y][x], ktr));
            inner.push_back(val);
            unsigned char e = (*mat)[y][x] - val;
            if (x == 0 && y < height - 1)
                error_pass(e, x, y, mat, 8.0 / 16, 2.0 / 16, 6.0 / 16, 0.0);    // left not bottom
            else if (x < width - 1 && y == height - 1)
                error_pass(e, x, y, mat, 1.0, 0.0, 0.0, 0.0);             // left or middle bottom
            else if (x == width - 1 && y < height - 1)
                error_pass(e, x, y, mat, 0.0, 0.0, 10.0 / 16, 6.0 / 16);      // right not bottom
            else if (x == width - 1 && y == height - 1)
                error_pass(e, x, y, mat, 0.0, 0.0, 0.0, 0.0);             // right bottom
            else
                error_pass(e, x, y, mat, 7.0 / 16, 1.0 / 16, 5.0 / 16, 3.0 / 16); // middle not bottom 
        }
        newMat->push_back(inner);
    }

    // write to file
    writeToFile("../img6.txt", newMat, width, height, 16);

    unsigned char* data_copy = (unsigned char*)(malloc(4 * width * height));
    if (data_copy != NULL) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                data_copy[4 * (i * width + j)] = (*newMat)[i][j];
                data_copy[4 * (i * width + j) + 1] = (*newMat)[i][j];
                data_copy[4 * (i * width + j) + 2] = (*newMat)[i][j];
                data_copy[4 * (i * width + j) + 3] = (unsigned char)255;
            }
        }
    }

    return data_copy;
}

