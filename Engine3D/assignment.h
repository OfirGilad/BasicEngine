#pragma once
#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>>* gaussianKernel(int* div);

vector<vector<int>>* onesKernel(int* div);

vector<vector<int>>* dxKernel(int* div);

vector<vector<int>>* dyKernel(int* div);

void print_picture(vector<vector<unsigned char>>* mat, int width, int height);

vector<vector<unsigned char>>* applyFilter(vector<vector<unsigned char>>* mat, int width, int height, vector<vector<int>>* kernel, int div);

vector<vector<unsigned char>>* greyScalePicture2Dim(unsigned char* data, int width, int height);

vector<vector<unsigned char>>* matrixAddition(vector<vector<unsigned char>>* dx, vector<vector<unsigned char>>* dy, int width, int height);

unsigned char* halftonePic(unsigned char* data, int* width, int* height);

unsigned char* edgeDetection(unsigned char* data, int* width, int* height);

unsigned char* FloydSteinbergAlgorithm(unsigned char* data, int* width, int* height);

void extraStuff(unsigned char e, int xPos, int yPos, vector<vector<unsigned char>>* mat, float alpha, float beta, float gama, float delta);

unsigned char newTrunc(unsigned char pixel_value);