#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "../res/includes/glm/glm.hpp"
#include "utilities.h"

using namespace std;
using namespace glm;

class SceneData
{
public:
	// Methods
	void read_scene(string file_name);
    void find_pixel_size(int width, int height);

    Image ImageRayCasting();
	vec3 ConstructRayThroughPixel(int i, int j);
    Hit FindIntersection(vec3 ray);
    //vec4 GetColor(vec3 ray, Hit hit);
	//float GetAngle(vec3 ray, Hit hit);

	/*float Intersect(vec3 ray, vec4 object);
	float FindIntersectionWithSphere(vec3 ray, vec4 object);
	float FindIntersectionWithPlane(vec3 ray, vec4 object);*/

	// Variables
	vector<vector<string>> scene_data;

	vec3 eye;
	int bonus_mode_flag;
	vec4 ambient;
	vector<vec4>reflective_objects;
	vector<vec4>transparent_objects;
	vector<Model*>objects;
	vector<vec4>colors;
	vector<vec4>directions;
	vector<vec4>positions;
	vector<vec4>intensities;

	int image_width, image_height;
	float pixel_width, pixel_height;

	vec3 forward_vector;
	vec3 up_vector;
	vec3 right_vector;
    vec2 center_dot;
};

//class SceneData
//{
//public:
//	// Methods
//	void read_scene(string file_name);
//	void find_pixel_size(int width, int height);
//
//	Image ImageRayCasting();
//	vec3 ConstructRayThroughPixel(int i, int j);
//	Hit FindIntersection(vec3 ray);
//	vec4 GetColor(vec3 ray, Hit hit);
//	float GetAngle(vec3 ray, Hit hit);
//
//	float Intersect(vec3 ray, vec4 object);
//	float FindIntersectionWithSphere(vec3 ray, vec4 object);
//	float FindIntersectionWithPlane(vec3 ray, vec4 object);
//
//	// Variables
//	vector<vector<string>> scene_data;
//
//	vec3 eye;
//	int bonus_mode_flag;
//	vec4 ambient;
//	vector<vec4>reflective_objects;
//	vector<vec4>transparent_objects;
//	vector<vec4>objects;
//	vector<vec4>colors;
//	vector<vec4>directions;
//	vector<vec4>positions;
//	vector<vec4>intensities;
//
//	int image_width, image_height;
//	float pixel_width, pixel_height;
//
//	vec3 forward_vector;
//	vec3 up_vector;
//	vec3 right_vector;
//	vec2 center_dot;
//};