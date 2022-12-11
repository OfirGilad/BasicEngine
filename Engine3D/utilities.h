#pragma once
#include "../res/includes/glm/glm.hpp"
#include <vector>

using namespace glm;
using namespace std;

float vectorSize(vec3 vec);
vec3 normalizedVector(vec3 vec);

//---------------------------------  Image  -------------------------------------------

class Image {

public:
	// Methods
	Image(int width, int height);
	void setColor(int pixelX, int pixelY, vec4 rgba);
	unsigned char* getData();

	// Variables
	unsigned char* data;
	int width;
	int height;

};

//------------------------------  Ray  ------------------------------------------

class Ray {
public:
	// Methods
	Ray(vec3 direction, vec3 position);

	// Variables
	vec3 direction;
	vec3 position;
};

//---------------------------------  SceneObject  -------------------------------------------

enum objectType {
	Regular,
	Transparent,
	Reflective,
	Space // When a ray intersects with nothing
};

class SceneObject {
public:
	// Methods
	virtual float Intersect(Ray ray) = 0;
	virtual void setColor(vec4 color);
	virtual vec3 getColor(vec3 hitPoint) = 0;
	virtual vec3 getNormal(vec3 hitPoint) = 0;

	// Variables
	objectType objType;
	vec4 details;
	vec3 rgb_color;
	float shiness;
	int objIndex;

	
};

//---------------------------------  Plane  -------------------------------------------

class Plane : public SceneObject {

public:
	// Methods
	Plane(vec4 details, objectType objType);
	vec3 normal();
	float d();
	float Intersect(Ray ray);
	vec3 getColor(vec3 hitPoint);
	vec3 getNormal(vec3 hitPoint);
};

//---------------------------------  Sphere  ------------------------------------------

class Sphere : public SceneObject {

public:
	// Methods
	Sphere(vec4 details, objectType objType);
	vec3 center();
	float radius();
	float Intersect(Ray ray);
	vec3 getColor(vec3 hitPoint);
	vec3 getNormal(vec3 hitPoint);
};

//----------------------------------  Hit  --------------------------------------------

class Hit {

public:
	// Methods
	Hit(vec3 hitPoint, SceneObject* obj);

	// Variables
	vec3 hitPoint;
	SceneObject* obj;
};

//---------------------------------  Light  -------------------------------------------

enum lightType {
	Directional,
	Spot
};

class Light {
public:
	// Methods
	virtual void setIntensity(vec4 intensity);

	// Variables
	lightType liType;
	vec3 direction;
	vec3 position;
	float cosAngle;
	vec3 rgb_intensity;
	float shiness;
};

//---------------------------  DirectionalLight  --------------------------------------

class DirectionalLight : public Light{
public:
	// Methods
	DirectionalLight(vec3 direction);
};

//------------------------------  SpotLight  ------------------------------------------

class SpotLight : public Light {	
public:
	// Methods
	SpotLight(vec3 direction);
};
