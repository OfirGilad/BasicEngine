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

vec3 projection(vec3 u, vec3 v) {
	return v * (dot(u, v) / pow(vectorSize(v), 2.f));
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

//---------------------------------  Model  -------------------------------------------

void Model::setColor(vec4 color) {
	this->rgb_color = vec3(color.r, color.g, color.b);
	this->shiness = color.a;
}

float Model::getAngle(vec3 hitVec, vec3 normal) {
	// dot product returns cos ==> acos returns angle between the vectors (we need the angle between hitVec and plane) 
	// ==> subtracting pi/2 gives us the actual angle ==> division by 2*pi and multiply by 360 gives us an angle in degrees
	return (acos(dot(hitVec, normal)) - acos(.0)) / (4 * acos(.0)) * 360;
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

float Plane::FindIntersection(vec3 ray, vec3 somePointOnRay) {
	vec3 planeNormal = this->normal();
	float a = planeNormal.x;
	float b = planeNormal.y;
	float c = planeNormal.z;
	float d = this->d();

	float x0 = somePointOnRay.x;
	float y0 = somePointOnRay.y;
	float z0 = somePointOnRay.z;

	float vecx = ray.x;
	float vecy = ray.y;
	float vecz = ray.z;

	float ans = -(a * x0 + b * y0 + c * z0 + d) / (a * vecx + b * vecy + c * vecz); // = -(dot(planeNormal, somePointOnRay) + d) / dot(planeNormal, ray);

	return ans;
}

vec3 Plane::getColor(vec3 hitPoint) {
	//float angle = this->getAngle(ray, hitPoint);

	// checkers board pattern
	if ((int(1.5 * hitPoint.x) % 2) == (int(1.5 * hitPoint.y) % 2)) {
		return 0.5f * this->rgb_color;
	}

	return this->rgb_color; // I = I(emission) + K(ambient) * I(ambient) + K(diffuse) * (N dot L) * I(light intensity) + K(specular) * (V dot R)^n * I(light intensity)
}

float Plane::getAngle(vec3 ray, vec3 hitPoint) {
	vec3 normalToThePlane = getNormal(hitPoint);
	return Model::getAngle(ray, normalToThePlane);
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

float Sphere::FindIntersection(vec3 ray, vec3 somePointOnRay) {
	vec3 center = this->center();
	float mx = center.x;
	float my = center.y;
	float mz = center.z;
	float radius = this->radius();

	float x0 = somePointOnRay.x;
	float y0 = somePointOnRay.y;
	float z0 = somePointOnRay.z;

	float vecx = ray.x;
	float vecy = ray.y;
	float vecz = ray.z;

	vec3 pointMinusCenterVec = somePointOnRay - center;

	//quadratic = vec3(t^2, t, 1)
	vec3 quadratic = vec3(
		1, //pow(vecx, 2) + pow(vecy, 2) + pow(vecz, 2) == 1 (ray is normalized)
		2 * dot(ray, pointMinusCenterVec),
		dot(pointMinusCenterVec, pointMinusCenterVec) - pow(radius, 2)
	);

	float delta = pow(quadratic.y, 2) - 4 * quadratic.x * quadratic.z; // b^2-4*a*c

	if (delta < 0.0)
		return INFINITY;

	float root = sqrt(delta);
	float ans1 = (-quadratic.y + root) / (2 * quadratic.x); // (-b + root) / 2*a
	float ans2 = (-quadratic.y - root) / (2 * quadratic.x); // (-b - root) / 2*a

	return glm::min(ans1, ans2);
}

vec3 Sphere::getColor(vec3 hitPoint) {
	//float angle = this->getAngle(ray, hitPoint);
	return this->rgb_color; // I = I(emission) + K(ambient) * I(ambient) + K(diffuse) * (N dot L) * I(light intensity) + K(specular) * (V dot R)^n * I(light intensity)
}

float Sphere::getAngle(vec3 ray, vec3 hitPoint) {
	vec3 normalToTheSphere = getNormal(hitPoint);
	return Model::getAngle(ray, normalToTheSphere);
}

vec3 Sphere::getNormal(vec3 hitPoint) {
	return normalizedVector(hitPoint - center());
}

//---------------------------------  Hit  -------------------------------------------

Hit::Hit(vec3 hitPoint, Model* obj) {
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

//void DirectionalLight::setParams(vec3 point, float cosAngle){}

//------------------------------  SpotLight  ------------------------------------------

SpotLight::SpotLight(vec3 direction) {
	this->liType = Spot;
	this->direction = direction;
}

//void SpotLight::setParams(vec3 point, float cosAngle) {
//	this->position = point;
//	this->cosAngle = cosAngle;
//}
