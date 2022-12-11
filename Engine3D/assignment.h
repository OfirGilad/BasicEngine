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
	void read_scene(string file_name, int width, int height);

    Image ImageRayCasting();
	vec3 ConstructRayThroughPixel(int i, int j, int position_on_pixel);
    Hit FindIntersection(vec3 ray, vec3 ray_start, int from_object_index);
    vec4 GetColor(vec3 ray, Hit hit, vec3 ray_start, int depth);

	vec3 calcDiffuseColor(Hit hit, Light* light);
	vec3 calcSpecularColor(Hit hit, Light* light, vec3 ray_start);
	float calcShadowTerm(Hit hit, Light* light);
	

	// Kd = diffuse constant for this type of object, normalizedN = normal orthogonal to the surface, normalizedL = direction from hit point to light, Il = intensity of light
	float calcDiffuse(float Kd, vec3 normalizedN, vec3 normalizedL, float Il);

	float calcSpecular(float Ks, vec3 normalizedV, vec3 normalizedR, int n, float Il);

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
