#pragma once
#include "../res/includes/glm/glm.hpp"
#include <vector>

using namespace glm;
using namespace std;

vec3 normalToPlain(vec3 vec1, vec3 vec2);

vec3 normalizedVector(vec3 vec);

float vectorSize(vec3 vec);

float getT(vec3 normalizedVec, vec3 otherVec);

vec3 projection(vec3 u, vec3 v);

//---------------------------------  Image  -------------------------------------------

class Image {


public:
	unsigned char* data;
	int width;
	int height;
	
	Image(int width, int height);

	void setColor(int pixelX, int pixelY, vec4 rgba);

	unsigned char* getData();

};

//---------------------------------  Model  -------------------------------------------

enum objectType {
	Regular,
	Transparent,
	Reflective
};

class Model {
public:
	objectType objType;
	vec4 details;
	vec3 rgb_color;
	float shiness;
	int objIndex;

	virtual float FindIntersection(vec3 ray, vec3 somePointOnRay) = 0;
	virtual void setColor(vec4 color);
	virtual vec3 getColor(vec3 hitPoint) = 0;
	virtual float getAngle(vec3 ray, vec3 hitPoint);
	virtual vec3 getNormal(vec3 hitPoint) = 0;
	//virtual void setPhongParams(vec4);
};

//---------------------------------  Plane  -------------------------------------------

class Plane : public Model{

public:
	Plane(vec4 details, objectType objType);
	vec3 normal();
	float d();
	float FindIntersection(vec3 ray, vec3 somePointOnRay);
	vec3 getColor(vec3 hitPoint);
	float getAngle(vec3 ray, vec3 hitPoint);
	vec3 getNormal(vec3 hitPoint);
};

//---------------------------------  Sphere  ------------------------------------------

class Sphere : public Model {

public:
	Sphere(vec4 details, objectType objType);
	vec3 center();
	float radius();
	float FindIntersection(vec3 ray, vec3 somePointOnRay);
	vec3 getColor(vec3 hitPoint);
	float getAngle(vec3 ray, vec3 hitPoint);
	vec3 getNormal(vec3 hitPoint);
};

//----------------------------------  Hit  --------------------------------------------

class Hit {

public:
	vec3 hitPoint;
	Model* obj;
	Hit(vec3 hitPoint, Model* obj);
};

//---------------------------------  Light  -------------------------------------------

enum lightType {
	Directional,
	Spot
};

class Light {
public:
	lightType liType;
	vec3 direction;
	vec3 position;
	float cosAngle;
	vec3 rgb_intensity;
	float shiness;
	virtual void setIntensity(vec4 intensity);
	/*virtual void setParams(vec3 point, float cosAngle) = 0;*/
};

//---------------------------  DirectionalLight  --------------------------------------

class DirectionalLight : public Light{
public:
	DirectionalLight(vec3 direction);
	/*virtual void setParams(vec3 point, float cosAngle);*/
};

//------------------------------  SpotLight  ------------------------------------------

class SpotLight : public Light {	
public:
	SpotLight(vec3 direction);
	/*void setParams(vec3 point, float cosAngle);*/
};