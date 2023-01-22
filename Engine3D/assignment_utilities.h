#pragma once
#include "../res/includes/glm/glm.hpp"
#include <vector>

using namespace glm;
using namespace std;

float vectorSize(vec3 vec);
vec3 normalizedVector(vec3 vec);

//---------------------------------  Image  ----------------------------------------

class Image {
public:
	// Methods
	Image(int width, int height);
	void setColor(int pixel_x, int pixel_y, vec4 rgba);
	unsigned char* getData();

	// Variables
	unsigned char* data;
	int width;
	int height;
};

//---------------------------------  Ray  -----------------------------------------------

class Ray {
public:
	// Methods
	Ray(vec3 direction, vec3 origin);

	// Variables
	vec3 direction;
	vec3 origin;
};

//---------------------------------  SceneObject  ---------------------------------------

enum ObjectType {
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
	virtual vec3 getColor(vec3 hit_point) = 0;
	virtual vec3 getNormal(vec3 hit_point) = 0;

	// Variables
	ObjectType object_type;
	vec4 details;
	vec3 rgb_color;
	float shininess;
	int object_index;
};

//---------------------------------  Plane  ---------------------------------------------

class Plane : public SceneObject {
public:
	// Methods
	Plane(vec4 details, ObjectType object_type);
	vec3 normal();
	float d();
	float Intersect(Ray ray);
	vec3 getColor(vec3 hit_point);
	vec3 getNormal(vec3 hit_point);
};

//---------------------------------  Sphere  --------------------------------------------

class Sphere : public SceneObject {
public:
	// Methods
	Sphere(vec4 details, ObjectType object_type);
	vec3 center();
	float radius();
	float Intersect(Ray ray);
	vec3 getColor(vec3 hit_point);
	vec3 getNormal(vec3 hit_point);
};

//---------------------------------  Hit  -----------------------------------------------

class Hit {
public:
	// Methods
	Hit(vec3 hit_point, SceneObject* scene_object);

	// Variables
	vec3 hit_point;
	SceneObject* scene_object;
};

//---------------------------------  Light  ---------------------------------------------

enum lightType {
	Directional,
	Spot
};

class Light {
public:
	// Methods
	virtual void setIntensity(vec4 intensity);

	// Variables
	lightType light_type;
	vec3 direction;
	vec3 position;
	float cos_angle;
	vec3 rgb_intensity;
	float shininess;
};

//---------------------------------  DirectionalLight  ----------------------------------

class DirectionalLight : public Light{
public:
	// Methods
	DirectionalLight(vec3 direction);
};

//---------------------------------  SpotLight  -----------------------------------------

class SpotLight : public Light {	
public:
	// Methods
	SpotLight(vec3 direction);
};
