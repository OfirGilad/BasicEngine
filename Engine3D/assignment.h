#pragma once
#include <iostream>
#include <vector>

std::vector<std::vector<int>>* gaussianKernel(int* div);

std::vector<std::vector<int>>* onesKernel(int* div);

std::vector<std::vector<int>>* dxKernel(int* div);

std::vector<std::vector<int>>* dyKernel(int* div);

void print_picture(std::vector<std::vector<unsigned char>>* mat, int width, int height);

std::vector<std::vector<unsigned char>>* applyFilter(std::vector<std::vector<unsigned char>>* mat, int width, int height, std::vector<std::vector<int>>* kernel, int div);

std::vector<std::vector<unsigned char>>* copyPicture2Dim(unsigned char* data, int width, int height);