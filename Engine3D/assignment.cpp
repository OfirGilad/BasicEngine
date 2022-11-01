#include "assignment.h"

std::vector<std::vector<int>>* gaussianKernel(int* div) {
    std::vector<std::vector<int>>* kernel = new std::vector<std::vector<int>>();
    std::vector<int> vec1;
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(1);
    kernel->push_back(vec1);
    std::vector<int> vec2;
    vec2.push_back(2);
    vec2.push_back(4);
    vec2.push_back(2);
    kernel->push_back(vec2);
    std::vector<int> vec3;
    vec3.push_back(1);
    vec3.push_back(2);
    vec3.push_back(1);
    kernel->push_back(vec3);
    *div = 16;
    /*for (int i = 0; i < 3; i++) {
        std::cout << (*kernel)[0][i];
        std::cout << (*kernel)[1][i];
        std::cout << (*kernel)[2][i] << std::endl;
    }*/

    return kernel;
}

std::vector<std::vector<int>>* onesKernel(int* div) {
    std::vector<std::vector<int>>* kernel = new std::vector<std::vector<int>>();
    std::vector<int> vec1;
    vec1.push_back(1);
    vec1.push_back(1);
    vec1.push_back(1);
    kernel->push_back(vec1);
    std::vector<int> vec2;
    vec2.push_back(1);
    vec2.push_back(1);
    vec2.push_back(1);
    kernel->push_back(vec2);
    std::vector<int> vec3;
    vec3.push_back(1);
    vec3.push_back(1);
    vec3.push_back(1);
    kernel->push_back(vec3);
    *div = 9;
    /*for (int i = 0; i < 3; i++) {
        std::cout << (*kernel)[0][i];
        std::cout << (*kernel)[1][i];
        std::cout << (*kernel)[2][i] << std::endl;
    }*/

    return kernel;
}

std::vector<std::vector<int>>* dxKernel(int* div) {
    std::vector<std::vector<int>>* kernel = new std::vector<std::vector<int>>();
    std::vector<int> vec1;
    vec1.push_back(0);
    vec1.push_back(0);
    vec1.push_back(0);
    kernel->push_back(vec1);
    std::vector<int> vec2;
    vec2.push_back(0);
    vec2.push_back(-1);
    vec2.push_back(0);
    kernel->push_back(vec2);
    std::vector<int> vec3;
    vec3.push_back(0);
    vec3.push_back(1);
    vec3.push_back(0);
    kernel->push_back(vec3);
    *div = 1;
    /*for (int i = 0; i < 3; i++) {
        std::cout << (*kernel)[0][i];
        std::cout << (*kernel)[1][i];
        std::cout << (*kernel)[2][i] << std::endl;
    }*/

    return kernel;
}

std::vector<std::vector<int>>* dyKernel(int* div) {
    std::vector<std::vector<int>>* kernel = new std::vector<std::vector<int>>();
    std::vector<int> vec1;
    vec1.push_back(0);
    vec1.push_back(0);
    vec1.push_back(0);
    kernel->push_back(vec1);
    std::vector<int> vec2;
    vec2.push_back(0);
    vec2.push_back(-1);
    vec2.push_back(1);
    kernel->push_back(vec2);
    std::vector<int> vec3;
    vec3.push_back(0);
    vec3.push_back(0);
    vec3.push_back(0);
    kernel->push_back(vec3);
    *div = 1;
    /*for (int i = 0; i < 3; i++) {
        std::cout << (*kernel)[0][i];
        std::cout << (*kernel)[1][i];
        std::cout << (*kernel)[2][i] << std::endl;
    }*/

    return kernel;
}

void print_picture(std::vector<std::vector<unsigned char>>* mat, int width, int height) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            std::cout << mat->at(j).at(i) / 25 % 9;
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<unsigned char>>* applyFilter(std::vector<std::vector<unsigned char>>* mat, int width, int height, std::vector<std::vector<int>>* kernel, int div) {
    unsigned char threshold = 35;
    std::vector<std::vector<unsigned char>>* newMat = new std::vector<std::vector<unsigned char>>();

    //upper end of the photo
    std::vector<unsigned char> upper;
    for (int i = 0; i < width; i++) {
        upper.push_back(i % 100 + 100);
    }
    newMat->push_back(upper);

    for (int j = 1; j < height - 1; j++) {
        std::vector<unsigned char> innerMat;
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
    std::vector<unsigned char> lower;
    for (int i = 0; i < width; i++) {
        lower.push_back(i % 100 + 100);
    }
    newMat->push_back(lower);

    print_picture(newMat, width, height);
    return newMat;
}

std::vector<std::vector<unsigned char>>* copyPicture2Dim(unsigned char* data, int width, int height) {
    std::vector<std::vector<unsigned char>>* grey_scale_matrix = new std::vector<std::vector<unsigned char>>();
    for (int i = 0; i < height; i++) {
        std::vector <unsigned char> inner_row;
        for (int j = 0; j < width; j++) {
            int color_mean = (data[4 * (i * width + j)] + data[4 * (i * width + j) + 1] + data[4 * (i * width + j) + 2]) / 3;
            inner_row.push_back(color_mean);
        }
        grey_scale_matrix->push_back(inner_row);
    }
    return grey_scale_matrix;
}