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
    void find_eye_vectors();
    void find_center(int width, int height);

	// Variables
	vector<vector<string>> scene_data;

	Vector3f eye;
	int bonus_mode_flag;
	Vector4f ambient;
	vector<Vector4f>reflective_objects;
	vector<Vector4f>transparent_objects;
	vector<Vector4f>objects;
	vector<Vector4f>colors;
	vector<Vector4f>directions;
	vector<Vector4f>positions;
	vector<Vector4f>intensities;


    Vector3f forward_vector;
    Vector2f center_dot;
    float epsilon_x, epsilon_y;
};