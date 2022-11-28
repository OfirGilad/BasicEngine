#include "utilities.h"

using namespace glm;

vec3 normalToPlain(vec3 vec1, vec3 vec2) {
	return cross(vec1, vec2);
}

vec3 normalizedVector(vec3 vec) {
	float vecSize = vectorSize(vec);
	vec3 normalized = vec3(vec);
	normalized.x /= vecSize;
	normalized.y /= vecSize;
	normalized.z /= vecSize;
	/*normalized.w /= vecSize;*/
	return normalized;
}

float vectorSize(vec3 vec) {
	return sqrt(powf(vec.x, 2.) + powf(vec.y, 2.) + powf(vec.z, 2.));
}

float getT(vec3 normalizedVec, vec3 otherVec) {
	return vectorSize(otherVec) / vectorSize(normalizedVec);
}

Image::Image(int width, int height) {
	this->width = width;
	this->height = height;
	data = (unsigned char*)(malloc(width * height * 4));
}

void Image::setColor(int pixelX, int pixelY, vec4 rgba) {
	data[(this->width * pixelY + pixelX) * 4] = rgba.r;
	data[(this->width * pixelY + pixelX) * 4 + 1] = rgba.g;
	data[(this->width * pixelY + pixelX) * 4 + 2] = rgba.b;
	data[(this->width * pixelY + pixelX) * 4 + 3] = rgba.a;
}

unsigned char* Image::getData() {
	return data;
}