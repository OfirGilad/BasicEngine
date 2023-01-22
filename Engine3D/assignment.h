#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "../res/includes/glm/glm.hpp"
#include "assignment_utilities.h"

using namespace std;
using namespace glm;

class SceneData {
public:
	// Methods
	SceneData(string file_name, int width, int height);
    Image ImageRayCasting();
	Ray ConstructRayThroughPixel(int i, int j, int position_on_pixel);
    Hit FindIntersection(Ray ray, int ignore_object_index);
    vec4 GetColor(Ray ray, Hit hit, int depth);
	vec3 calcDiffuseColor(Hit hit, Light* light);
	vec3 calcSpecularColor(Ray ray, Hit hit, Light* light);
	float calcShadowTerm(Hit hit, Light* light);

	// Variables
	vector<vector<string>> scene_data;
	vec3 eye;
	float bonus_mode_flag;
	vec4 ambient;
	vector<SceneObject*>objects;
	vector<vec4>colors;
	vector<Light*>lights;
	vector<vec4>positions;
	vector<vec4>intensities;
	int image_width, image_height;
	float pixel_width, pixel_height;
};
