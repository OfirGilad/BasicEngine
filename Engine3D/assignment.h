#pragma once
#include <iostream>
#include <vector>
using namespace std;

// Canny_Edge_Detector (Improved)
unsigned char* Canny_Edge_Detector_Improved(unsigned char* data, int width, int height);

// Canny_Edge_Detector
unsigned char* Canny_Edge_Detector(unsigned char* data, int width, int height);

// Halftone
unsigned char* Halftone(unsigned char* data, int width, int height);

// Floyd_Steinberg_Algorithm
unsigned char* Floyd_Steinberg_Algorithm(unsigned char* data, int width, int height);

// Exporting text file
void writeToFile(const string& fileName, vector<vector<unsigned char>>* mat, int width, int height, int div);
