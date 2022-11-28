#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "../res/includes/glm/glm.hpp"

using namespace std;
using namespace glm;

class SceneData
{
public:
	// Methods
	void read_scene(string file_name);
    void find_eye_vectors();
    void find_center(int width, int height);
	vec3 ConstructRayThroughPixel(int i, int j);
	Image RayCast(Scene scene, int width, int height);

	// Variables
	vector<vector<string>> scene_data;

	vec3 eye;
	int bonus_mode_flag;
	vec4 ambient;
	vector<vec4>reflective_objects;
	vector<vec4>transparent_objects;
	vector<vec4>objects;
	vector<vec4>colors;
	vector<vec4>directions;
	vector<vec4>positions;
	vector<vec4>intensities;


	vec3 forward_vector;
	vec2 center_dot;
    float epsilon_x, epsilon_y;
};