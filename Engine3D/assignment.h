#pragma once
#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>>* gaussianKernel(int* div);

vector<vector<int>>* onesKernel(int* div);

vector<vector<int>>* dxKernel(int* div);

vector<vector<int>>* dyKernel(int* div);

void print_picture(vector<vector<unsigned char>>* mat, int width, int height);

vector<vector<unsigned char>>* applyFilter1(vector<vector<unsigned char>>* mat, int width, int height, vector<vector<int>>* kernel, int div);

vector<vector<char>>* applyFilter2(vector<vector<unsigned char>>* mat, int width, int height, vector<vector<int>>* kernel, int div);

vector<vector<unsigned char>>* greyScaleImageConverter(unsigned char* data, int width, int height);

vector<vector<unsigned char>>* matrixAddition(const vector<vector<unsigned char>>* dx, const vector<vector<unsigned char>>* dy, int width, int height);

unsigned char* halftone(unsigned char* data, int* width, int* height);

unsigned char* Canny_Edge_Detector(unsigned char* data, int* width, int* height);

unsigned char* Floyd_Steinberg_Algorithm(unsigned char* data, int* width, int* height);

void extraStuff(unsigned char e, int xPos, int yPos, vector<vector<unsigned char>>* mat, float alpha, float beta, float gama, float delta);

unsigned char newTrunc(unsigned char pixel_value);

vector<vector<unsigned char>>* thinLines(const vector<vector<char>>* dx, const vector<vector<char>>* dy, vector<vector<unsigned char>>* dx_plus_dy, int* width, int* height);

vector<vector<unsigned char>>* non_max_suppression(const vector<vector<unsigned char>>* grey_scale_matrix, vector<vector<unsigned char>>* dx_plus_dy, int* width, int* height);