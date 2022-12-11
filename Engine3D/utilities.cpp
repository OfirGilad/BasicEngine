#include "utilities.h"

using namespace glm;

float vectorSize(vec3 vec) {
	return sqrt(powf(vec.x, 2.) + powf(vec.y, 2.) + powf(vec.z, 2.));
}

vec3 normalizedVector(vec3 vec) {
	float vecSize = vectorSize(vec);
	vec3 normalized = vec3(vec);
	normalized.x /= vecSize;
	normalized.y /= vecSize;
	normalized.z /= vecSize;
	return normalized;
}

//---------------------------------  Image  -------------------------------------------

Image::Image(int width, int height) {
	this->width = width;
	this->height = height;
	data = (unsigned char*)(malloc(width * height * 4));
}

void Image::setColor(int pixelX, int pixelY, vec4 rgba) {
	data[(this->width * pixelY + pixelX) * 4] = (unsigned char) (rgba.r * 255);
	data[(this->width * pixelY + pixelX) * 4 + 1] = (unsigned char) (rgba.g * 255);
	data[(this->width * pixelY + pixelX) * 4 + 2] = (unsigned char) (rgba.b * 255);
	data[(this->width * pixelY + pixelX) * 4 + 3] = (unsigned char) (rgba.a * 255);
}

unsigned char* Image::getData() {
	return data;
}

//---------------------------------  SceneObject  -------------------------------------------

void SceneObject::setColor(vec4 color) {
	this->rgb_color = vec3(color.r, color.g, color.b);
	this->shiness = color.a;
}

//---------------------------------  Plane  -------------------------------------------

Plane::Plane(vec4 details, objectType objType) {
	this->details = details;
	this->objType = objType;
}

vec3 Plane::normal() {
	return vec3(
		details.x,
		details.y,
		details.z
	);
}

float Plane::d() {
	return details.w;
}

float Plane::Intersect(Ray ray) {
	vec3 planeNormal = this->normal();
	float a = planeNormal.x;
	float b = planeNormal.y;
	float c = planeNormal.z;
	float d = this->d();

	float x0 = ray.position.x;
	float y0 = ray.position.y;
	float z0 = ray.position.z;

	float vecx = ray.direction.x;
	float vecy = ray.direction.y;
	float vecz = ray.direction.z;

	float ans = -(a * x0 + b * y0 + c * z0 + d) / (a * vecx + b * vecy + c * vecz); 

	return ans;
}

vec3 Plane::getColor(vec3 hitPoint) {
	// Checkerboard pattern
	float scaler_parameter = 0.5f;
	float chessboard = 0;

	if (hitPoint.x < 0) {
		chessboard += floor((0.5 - hitPoint.x) / scaler_parameter);
	}
	else {
		chessboard += floor(hitPoint.x / scaler_parameter);
	}

	if (hitPoint.y < 0) {
		chessboard += floor((0.5 - hitPoint.y) / scaler_parameter);
	}
	else {
		chessboard += floor(hitPoint.y / scaler_parameter);
	}

	chessboard = (chessboard * 0.5) - int(chessboard * 0.5);
	chessboard *= 2;
	if (chessboard > 0.5) {
		return 0.5f * this->rgb_color;
	}
	return this->rgb_color;
}

vec3 Plane::getNormal(vec3 hitPoint) {
	return normalizedVector(normal());
}

//---------------------------------  Sphere  ------------------------------------------

Sphere::Sphere(vec4 details, objectType objType) {
	this->details = details;
	this->objType = objType;
}

vec3 Sphere::center() {
	return vec3(
		details.x,
		details.y,
		details.z
	);
}

float Sphere::radius() {
	return details.w;
}

float Sphere::Intersect(Ray ray) {
	vec3 center = this->center();
	float mx = center.x;
	float my = center.y;
	float mz = center.z;
	float radius = this->radius();

	float x0 = ray.position.x;
	float y0 = ray.position.y;
	float z0 = ray.position.z;

	float vecx = ray.direction.x;
	float vecy = ray.direction.y;
	float vecz = ray.direction.z;

	vec3 pointMinusCenterVec = ray.position - center;

	//quadratic = vec3(t^2, t, 1)
	vec3 quadratic = vec3(
		1, //pow(vecx, 2) + pow(vecy, 2) + pow(vecz, 2) == 1 (ray is normalized)
		2 * dot(ray.direction, pointMinusCenterVec),
		dot(pointMinusCenterVec, pointMinusCenterVec) - pow(radius, 2)
	);

	float delta = pow(quadratic.y, 2) - 4 * quadratic.x * quadratic.z; // b^2-4*a*c

	if (delta < 0.0)
		return INFINITY;

	float root = sqrt(delta);
	float ans1 = (-quadratic.y + root) / (2 * quadratic.x); // (-b + root) / 2*a
	float ans2 = (-quadratic.y - root) / (2 * quadratic.x); // (-b - root) / 2*a
	float result = glm::min(ans1, ans2);

	// In case of Transperant spheres
	float threshold = 0.001f;
	if (result <= threshold) {
		result = glm::max(ans1, ans2);
	}

	return result;
}

vec3 Sphere::getColor(vec3 hitPoint) {
	return this->rgb_color;
}

vec3 Sphere::getNormal(vec3 hitPoint) {
	return normalizedVector(hitPoint - center());
}

//---------------------------------  Hit  -------------------------------------------

Hit::Hit(vec3 hitPoint, SceneObject* obj) {
	this->hitPoint = hitPoint;
	this->obj = obj;
}


void Light::setIntensity(vec4 intensity) {
	this->rgb_intensity = vec3(intensity.r, intensity.g, intensity.b);
	this->shiness = intensity.a;
}

//---------------------------  DirectionalLight  --------------------------------------

DirectionalLight::DirectionalLight(vec3 direction) {
	this->liType = Directional;
	this->direction = direction;
}

//------------------------------  SpotLight  ------------------------------------------

SpotLight::SpotLight(vec3 direction) {
	this->liType = Spot;
	this->direction = direction;
}

//------------------------------  Ray  ------------------------------------------

Ray::Ray(vec3 direction, vec3 position) {
	this->direction = direction;
	this->position = position;
}