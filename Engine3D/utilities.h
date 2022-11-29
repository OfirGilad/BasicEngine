#pragma once
#include "../res/includes/glm/glm.hpp"

using namespace glm;

vec3 normalToPlain(vec3 vec1, vec3 vec2);

vec3 normalizedVector(vec3 vec);

float vectorSize(vec3 vec);

float getT(vec3 normalizedVec, vec3 otherVec);

//---------------------------------  Image  -------------------------------------------

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

//---------------------------------  Model  -------------------------------------------

class Model {
protected:
	vec4 details;

public:
	virtual float FindIntersection(vec3 ray, vec3 eye) = 0;
	virtual vec4 getColor(vec3 ray, vec3 hitPoint) = 0;
	virtual float getAngle(vec3 ray, vec3 hitPoint);
	//virtual void setPhongParams(vec4);
};

//---------------------------------  Plane  -------------------------------------------

class Plane : public virtual Model{

public:
	Plane(vec4 details);
	vec3 normal();
	float d();
	float FindIntersection(vec3 ray, vec3 eye);
	vec4 getColor(vec3 ray, vec3 hitPoint);
	float getAngle(vec3 ray, vec3 hitPoint);
};

//---------------------------------  Sphere  ------------------------------------------

class Sphere : public virtual Model {

public:
	Sphere(vec4 details);
	vec3 center();
	float radius();
	float FindIntersection(vec3 ray, vec3 eye);
	vec4 getColor(vec3 ray, vec3 hitPoint);
	float getAngle(vec3 ray, vec3 hitPoint);
};

//----------------------------------  Hit  --------------------------------------------

class Hit {

public:
	vec3 hitPoint;
	Model* obj;
	Hit(vec3 hitPoint, Model* obj);
};