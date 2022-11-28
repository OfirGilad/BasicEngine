#pragma once
#include "../res/includes/glm/glm.hpp"

using namespace glm;
//find normal to a plain
vec3 normalToPlain(vec3 vec1, vec3 vec2);

vec3 normalizedVector(vec3 vec);

float vectorSize(vec3 vec);

float getT(vec3 normalizedVec, vec3 otherVec);

class Image {

private:
	unsigned char* data;
	int width;
	int height;


public:
	Image(int width, int height);

	void setColor(int pixelX, int pixelY, vec4 rgba);

	unsigned char* getData();

};