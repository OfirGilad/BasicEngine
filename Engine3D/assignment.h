#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <Eigen/Core>

using namespace std;
using namespace Eigen;

class SceneData
{
public:
	// Methods
	void read_scene(string file_name);

	// Variables
	vector<vector<string>> scene_data;
	Vector3f eye;
	int bonus_mode_flag;
	Vector4f ambient;
};