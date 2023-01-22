#include "assignment_utilities.h"

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
	this->shininess = color.a;
}

//---------------------------------  Plane  -------------------------------------------

Plane::Plane(vec4 details, ObjectType object_type) {
	this->details = details;
	this->object_type = object_type;
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
	vec3 plane_normal = this->normal();
	float d = this->d();
    float answer = -1;

    if (dot(ray.direction, plane_normal) != 0.0f)
        answer = -(dot(ray.origin, plane_normal) + d) / dot(ray.direction, plane_normal);

	return answer;
}

vec3 Plane::getColor(vec3 hitPoint) {
	// Checkerboard pattern
	float scale_parameter = 0.5f;
	float chessboard = 0;

	if (hitPoint.x < 0) {
		chessboard += floor((0.5 - hitPoint.x) / scale_parameter);
	}
	else {
		chessboard += floor(hitPoint.x / scale_parameter);
	}

	if (hitPoint.y < 0) {
		chessboard += floor((0.5 - hitPoint.y) / scale_parameter);
	}
	else {
		chessboard += floor(hitPoint.y / scale_parameter);
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

Sphere::Sphere(vec4 details, ObjectType object_type) {
	this->details = details;
	this->object_type = object_type;
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
	float radius = this->radius();

	vec3 L = ray.origin - center;

	//quadratic = vec3(t^2, t, 1)
	vec3 quadratic = vec3(
		1, //pow(ray.direction.x, 2) + pow(ray.direction.y, 2) + pow(ray.direction.z, 2) == 1 (ray is normalized)
		2 * dot(ray.direction, L),
		dot(L, L) - pow(radius, 2)
	);

	float delta = pow(quadratic.y, 2) - 4 * quadratic.x * quadratic.z; // b^2-4*a*c

	if (delta < 0.0)
		return INFINITY;

	float root = sqrt(delta);
	float answer_1 = (-quadratic.y + root) / (2 * quadratic.x); // (-b + root) / 2*a
	float answer_2 = (-quadratic.y - root) / (2 * quadratic.x); // (-b - root) / 2*a
	float result = glm::min(answer_1, answer_2);

	// In case of Transparent spheres
	float threshold = 0.001f;
	if (result <= threshold) {
		result = glm::max(answer_1, answer_2);
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

Hit::Hit(vec3 hit_point, SceneObject* scene_object) {
	this->hit_point = hit_point;
	this->scene_object = scene_object;
}


void Light::setIntensity(vec4 intensity) {
	this->rgb_intensity = vec3(intensity.r, intensity.g, intensity.b);
	this->shininess = intensity.a;
}

//---------------------------  DirectionalLight  --------------------------------------

DirectionalLight::DirectionalLight(vec3 direction) {
	this->light_type = Directional;
	this->direction = direction;
}

//------------------------------  SpotLight  ------------------------------------------

SpotLight::SpotLight(vec3 direction) {
	this->light_type = Spot;
	this->direction = direction;
}

//------------------------------  Ray  ------------------------------------------

Ray::Ray(vec3 direction, vec3 origin) {
	this->direction = direction;
	this->origin = origin;
}