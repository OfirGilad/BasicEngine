#include "assignment.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "../Game/game.h"
#include "utilities.h"

using namespace std;
using namespace glm;

void SceneData::read_scene(string file_name, int width, int height) {
    int index = -1;
    string text_line, text_argument;
    vector<string> scene_line;
    ifstream scene_file(file_name);

    cout << "File data:" << endl;

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(scene_file, text_line)) {
        cout << text_line << endl;

        std::stringstream text_line_stream(text_line);

        while (getline(text_line_stream, text_argument, ' ')) {
            scene_line.push_back(text_argument);
            index = (index + 1) % 5;

            if (index == 4) {
                scene_data.push_back(scene_line);
                scene_line.clear();
            }
        } 
    }

    for (int i = 0; i < scene_data.size(); i++) {
		vec4 input_vector = vec4(stof(scene_data[i][1]), stof(scene_data[i][2]), stof(scene_data[i][3]), stof(scene_data[i][4]));

        // e = eye - XYZ + Bonus Mode Flag
        if (scene_data[i][0] == "e") {
            eye = vec3(input_vector.x, input_vector.y, input_vector.z);
            bonus_mode_flag = input_vector.w;
        }
        // a = ambient - RGBA
        if (scene_data[i][0] == "a") {
            ambient = input_vector;
        }
        // r = reflective object - XYZW (W>0 -> Spheres) / (W<0 -> Planes)
        if (scene_data[i][0] == "r") {
            if (input_vector.w > 0)
                objects.push_back(new Sphere(input_vector, Reflective));
            else objects.push_back(new Plane(input_vector, Reflective));
        }
        // t = transparent object - XYZW (W>0 -> Spheres) / (W<0 -> Planes)
        if (scene_data[i][0] == "t") {
            if (input_vector.w > 0)
                objects.push_back(new Sphere(input_vector, Transparent));
            else objects.push_back(new Plane(input_vector, Transparent));
        }
        // o = object - XYZW (W>0 -> Spheres) / (W<0 -> Planes)
        if (scene_data[i][0] == "o") {
            if (input_vector.w > 0)
                objects.push_back(new Sphere(input_vector, Regular));
            else objects.push_back(new Plane(input_vector, Regular));
        }
        // c = color (ambient and diffuse of object) - RGBA (A -> Shininess)
        if (scene_data[i][0] == "c") {
            colors.push_back(input_vector);
        }
        // d = direction (of light sources) - XYZW
        if (scene_data[i][0] == "d") {
            if(input_vector.w > 0)
                lights.push_back(new SpotLight(vec3(input_vector.x, input_vector.y, input_vector.z)));
            else lights.push_back(new DirectionalLight(vec3(input_vector.x, input_vector.y, input_vector.z)));
        }
        // p = position (of spotlights only) - XYZW
        if (scene_data[i][0] == "p") {
            positions.push_back(input_vector);
        }
        // i = intensity (of light sources) - RGBA
        if (scene_data[i][0] == "i") {
            intensities.push_back(input_vector);
        }
    }

    // Set objects colors
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->objIndex = i;
        objects[i]->setColor(colors[i]);
    }

    // Set spotlight positions
    int j = 0;
    for (int i = 0; i < lights.size(); i++) {
        if (lights[i]->liType == Spot) {
            vec3 point = vec3(positions[j].x, positions[j].y, positions[j].z);
            float cosAngle = positions[j++].w;
            lights[i]->position = point;
            lights[i]->cosAngle = cosAngle;
        }
        lights[i]->setIntensity(intensities[i]);
    }

    // Finding pixel width and height and saving parameters
    pixel_width = 2.0 / float(width);
    pixel_height = 2.0 / float(height);
    image_width = width;
    image_height = height;
}

Image SceneData::ImageRayCasting() {
    Image image = Image(image_width, image_height);

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            if ((i == 750) && (j == 500)) {
                cout << "TEST" << endl;
            }
            vec3 ray = ConstructRayThroughPixel(i, j);
            Hit hit = FindIntersection(ray, eye, -1);
            vec4 pixel_color = GetColor(ray, hit, eye, 0);

            image.setColor(i, j, pixel_color);
        }
    }
    return image;
}

// top-left (-1, 1)
vec3 SceneData::ConstructRayThroughPixel(int i, int j) {
    vec3 top_left_point = vec3(-1 + (pixel_width / 2), 1 - (pixel_height / 2), 0);
    vec3 hit_on_screen = top_left_point + vec3(i * pixel_width, -1 * (j * pixel_height), 0);
    vec3 ray_direction = hit_on_screen - eye;
    
    return normalizedVector(ray_direction);
}

Hit SceneData::FindIntersection(vec3 ray, vec3 ray_start, int from_object_index) {
    // Set Default Values
    float min_t = INFINITY;
    Model* min_primitive = new Plane(vec4(1.0, 1.0, 1.0, 1.0), Space);
    min_primitive->setColor(vec4(0.0, 0.0, 0.0, 0.0));
    bool got_hit = false;

    // Looping over all the objects
    for (int i = 0; i < objects.size(); i++) {
        if (i != from_object_index) {
            float t = objects[i]->FindIntersection(ray, ray_start);
            //float t = Intersect(ray, objects[i]);

            if ((t > 0) && (t < min_t)) {
                got_hit = true;
                min_primitive = objects[i];
                min_t = t;
            }
        }
    }

    Hit hit = Hit(ray_start + ray, min_primitive);
    if (got_hit) {
        hit = Hit(ray_start + ray * min_t, min_primitive);
    }
    return hit;
}

vec4 SceneData::GetColor(vec3 ray, Hit hit, vec3 ray_start, int depth) {
    // Regular case
    vec3 color = hit.obj->getColor(hit.hitPoint);
    vec3 phong_model_color = color * vec3(ambient.r, ambient.g, ambient.b); // Ambient

    for (int i = 0; i < lights.size(); i++) {
        vec3 diffuse_color = max(calcDiffuseColor(hit, lights[i]), vec3(0, 0, 0)); // Diffuse
        vec3 specular_color = max(calcSpecularColor(hit, lights[i], ray_start), vec3(0, 0, 0)); // Specular
        float shadow_term = calcShadowTerm(hit, lights[i]);

        phong_model_color += (diffuse_color + specular_color) * shadow_term;
    }
    phong_model_color = min(phong_model_color, vec3(1.0, 1.0, 1.0));

    // Reflective case
    if (hit.obj->objType == Reflective) {
        if (depth == 5) { // MAX_LEVEL=5
            return vec4(0, 0, 0, 0);
        }
        vec3 reflection_ray = ray - 2.0f * hit.obj->getNormal(hit.hitPoint) * dot(ray, hit.obj->getNormal(hit.hitPoint));
        Hit reflected_hit = FindIntersection(reflection_ray, hit.hitPoint, hit.obj->objIndex);

        if (reflected_hit.obj->objType == Space) {
            return vec4(0, 0, 0, 0);
        }

        vec4 reflection_color = GetColor(reflection_ray, reflected_hit, hit.hitPoint, depth + 1);

        // The Correct, but less beautiful, equation
        //phong_model_color = 0.7f * vec3(reflection_color.r, reflection_color.g, reflection_color.b);

        phong_model_color = vec3(reflection_color.r, reflection_color.g, reflection_color.b);
        phong_model_color = min(phong_model_color, vec3(1.0, 1.0, 1.0));
    }

    // Transparent case
    if (hit.obj->objType == Transparent) {
        if (depth == 5) { // MAX_LEVEL=5
            return vec4(0, 0, 0, 0);
        }
        vec4 transparency_color = vec4(0, 0, 0, 0);

        // Transparent Plane
        if (hit.obj->details.w < 0.0) {
            Hit transparency_hit = FindIntersection(ray, hit.hitPoint, hit.obj->objIndex);

            if (transparency_hit.obj->objType == Space) {
                return vec4(0, 0, 0, 0);
            }

            transparency_color = GetColor(ray, transparency_hit, transparency_hit.hitPoint, depth + 1);
        }
        // Transparent Sphere
        else {
            // Snell's law
            float cos_from = dot(hit.obj->getNormal(hit.hitPoint), -ray);
            float theta_from = acos(cos_from) * 180 / 3.14;
            float snell_frac = (1.0f / 1.5f);
            float sin_to = snell_frac * sin(theta_from);
            float theta_to = asin(sin_to) * 180 / 3.14;
            float cos_to = cos(theta_to);

            // Finding the second hit inside the sphere
            vec3 ray_in = (snell_frac * cos_from - cos_to) * hit.obj->getNormal(hit.hitPoint) - snell_frac * (-ray);
            float t = hit.obj->FindIntersection(ray_in, hit.hitPoint);
            vec3 second_hit_point = hit.hitPoint + ray_in * t;

            // Reverse calculations
            cos_from = dot(-hit.obj->getNormal(hit.hitPoint), ray_in);
            theta_from = acos(cos_from) * 180 / 3.14;
            snell_frac = (1.5f / 1.0f);
            sin_to = snell_frac * sin(theta_from);
            theta_to = asin(sin_to) * 180 / 3.14;
            cos_to = cos(theta_to);

            // Finding the raw out of the sphere
            vec3 ray_out = (snell_frac * cos_from - cos_to) * hit.obj->getNormal(hit.hitPoint) - snell_frac * (-ray_in);

            
            Hit transparency_hit = FindIntersection(ray_out, second_hit_point, hit.obj->objIndex);

            if (transparency_hit.obj->objType == Space) {
                return vec4(0, 0, 0, 0);
            }

            transparency_color = GetColor(ray_out, transparency_hit, transparency_hit.hitPoint, depth + 1);
        }


        //phong_model_color = 0.7f * vec3(transparency_color.r, transparency_color.g, transparency_color.b);

        phong_model_color = vec3(transparency_color.r, transparency_color.g, transparency_color.b);
        phong_model_color = min(phong_model_color, vec3(1.0, 1.0, 1.0));
    }

    return vec4(phong_model_color.r, phong_model_color.g, phong_model_color.b, 0.0);
}

vec3 SceneData::calcDiffuseColor(Hit hit, Light* light) {
    float object_factor = 1;
    if (hit.obj->details.w < 0.0) object_factor = -1;

    vec3 normalized_ray_direction = object_factor * normalizedVector(light->direction);

    if (light->liType == Spot) {
        vec3 virtual_spotlight_ray = normalizedVector(hit.hitPoint - light->position);
        float light_cos_value = dot(virtual_spotlight_ray, object_factor *normalized_ray_direction);

        if (light_cos_value < light->cosAngle) {
            return vec3(0.0, 0.0, 0.0);
        }
        else {
            normalized_ray_direction = object_factor * virtual_spotlight_ray;
        }
    }
    vec3 object_normal = hit.obj->getNormal(hit.hitPoint);

    // N^*L^ = cos(t)
    float hit_cos_value = dot(object_normal, -normalized_ray_direction);

    // Id = Kd*(N^*L^)*Il
    vec3 diffuse_color = hit.obj->getColor(hit.hitPoint) * hit_cos_value * light->rgb_intensity;
    return diffuse_color;
}

vec3 SceneData::calcSpecularColor(Hit hit, Light* light, vec3 ray_start) {
    vec3 normalized_ray_direction = normalizedVector(light->direction);

    if (light->liType == Spot) {
        vec3 virtual_spotlight_ray = normalizedVector(hit.hitPoint - light->position);
        float light_cos_value = dot(virtual_spotlight_ray, normalized_ray_direction);

        if (light_cos_value < light->cosAngle) {
            return vec3(0.0, 0.0, 0.0);
        }
        else {
            normalized_ray_direction = virtual_spotlight_ray;
        }
    }
    vec3 object_normal = hit.obj->getNormal(hit.hitPoint);
    vec3 reflected_light_ray = normalized_ray_direction - 2.0f * object_normal * dot(normalized_ray_direction, object_normal);
    vec3 ray_to_viewer = normalizedVector(ray_start - hit.hitPoint);

    // V^*R^ = max(0, V^*R^)
    float hit_cos_value = dot(ray_to_viewer, reflected_light_ray);
    hit_cos_value = glm::max(0.0f, hit_cos_value);
    // (V^*R^)^n
    hit_cos_value = pow(hit_cos_value, hit.obj->shiness);

    // Id = Ks*(V^*R^)^n*Il
    // Ks = (0.7, 0.7, 0.7)
    vec3 speculat_color = 0.7f * hit_cos_value * light->rgb_intensity;
    return speculat_color;
}

float SceneData::calcShadowTerm(Hit hit, Light* light) {
    vec3 normalized_ray_direction = normalizedVector(light->direction);
    float min_t = INFINITY;

    if (light->liType == Spot) {
        vec3 virtual_spotlight_ray = normalizedVector(hit.hitPoint - light->position);
        float light_cos_value = dot(virtual_spotlight_ray, normalized_ray_direction);

        if (light_cos_value < light->cosAngle) {
            return 0.0;
        }
        else {
            normalized_ray_direction = virtual_spotlight_ray;
            // Update min_t to the value of the light position
            min_t = -(dot(hit.hitPoint, light->position)) / abs(dot(-normalized_ray_direction, light->position));
        }
    }

    // Checking the path between the light source and the object
    // Looping over all the objects
    for (int i = 0; i < objects.size(); i++) {
        if (i != hit.obj->objIndex) {
            float t = objects[i]->FindIntersection(-normalized_ray_direction, hit.hitPoint);

            if ((t > 0) && (t < min_t)) {
                return 0.0;
            }
        }
    }
    return 1.0;
}


//float SceneData::Intersect(vec3 ray, vec4 object) {
//    if (object.r > 0) {
//        return FindIntersectionWithSphere(ray, object);
//    }
//    else {
//        return FindIntersectionWithPlane(ray, object);
//    }
//}

//float SceneData::FindIntersectionWithSphere(vec3 ray, vec4 sphere) {
//    float mx = sphere.x;
//    float my = sphere.y;
//    float mz = sphere.z;
//    float radius = sphere.w;
//
//    float x0 = eye.x;
//    float y0 = eye.y;
//    float z0 = eye.z;
//
//    float vecx = ray.x;
//    float vecy = ray.y;
//    float vecz = ray.z;
//
//    //quadratic = vec3(t^2, t, 1)
//    vec3 quadratic = vec3(
//        pow(vecx, 2) + pow(vecy, 2) + pow(vecz, 2),
//        2 * (x0 - mx + y0 - my + z0 - mz),
//        pow(x0 - mx, 2) + pow(y0 - my, 2) + pow(z0 - mz, 2) - pow(radius, 2)
//    );
//
//    float delta = pow(quadratic.y, 2) - 4 * quadratic.x * quadratic.z; // b^2-4*a*c
//
//    if (delta < 0.)
//        return -1;
//
//    float root = sqrt(delta);
//    float ans1 = (-quadratic.y + root) / (2 * quadratic.x); // (-b + root) / 2*a
//    float ans2 = (-quadratic.y - root) / (2 * quadratic.x); // (-b - root) / 2*a
//
//    return glm::min(ans1, ans2);
//}

//float SceneData::FindIntersectionWithPlane(vec3 ray, vec4 plane) {
//    float a = plane.x;
//    float b = plane.y;
//    float c = plane.z;
//    float d = - plane.w;
//
//    float x0 = eye.x;
//    float y0 = eye.y;
//    float z0 = eye.z;
//
//    float vecx = ray.x;
//    float vecy = ray.y;
//    float vecz = ray.z;
//
//    float ans = -(a * x0 + b * y0 + c * z0 + d) / (a * vecx + b * vecy + c * vecz);
//
//    return ans;
//}

//float SceneData::GetAngle(vec3 ray, Hit hit) {
//    vec4 hitObj = hit.getObj();
//    vec3 normalToThePlane = 
//        hitObj.w < 0 ?
//        normalizedVector(vec3(hitObj.x, hitObj.y, hitObj.z)) :
//        normalizedVector(hit.hitPoint - vec3(hitObj.x, hitObj.y, hitObj.z));
//    return (acos(dot(ray, normalToThePlane)) - acos(.0)) / (4 * acos(.0)) * 360;
//}

// Kd = diffuse constant for this type of object, normalizedN = normal orthogonal to the surface, normalizedL = direction from hit point to light, Il = intensity of light
float SceneData::calcDiffuse(float Kd, vec3 normalizedN, vec3 normalizedL, float Il) {
    return Kd * dot(normalizedN, normalizedL) * Il; //Id = Kd * (N^ dot N^) * Il
}

// Ks = specular constant for this type of object, normalizedV = direction from hit point to the viewer's eye, 
// normalizedR = radiance vector representing the light direction after bouncing off the surface, Il = intensity of light
// n = phong exponent (aparent smoothness of the surface)
float SceneData::calcSpecular(float Ks, vec3 normalizedV, vec3 normalizedR, int n, float Il) {
    return Ks * pow(dot(normalizedV, normalizedR), n) * Il; //Is = Ks * (V^ dot R^) ^ n * Il
}

//vec4 SceneData::calcPhongColor(vec3 ray, Hit hit, vector<Light> lights) {
//    vec3 V = -ray;
//    vec3 N = hit.obj->getNormal(hit.hitPoint);
//
//    vec4 Ie = vec4(0., 0., 0., 0.);
//    vec4 Ia = vec4(0., 0., 0., 0.);
//    float Ka = 0.;
//    float Kd = 0.;
//    float Ks = 0.7;
//
//    float sumDiffuseSpecular = 0.;
//    for each (Light light in lights) {
//        vec3 L = light.point - hit.hitPoint;
//        vec3 R = 2.f * projection(L, N) - L;
//
//        for each (Model * someObj in objects) {
//            vec3 lightRay = hit.hitPoint - light.point;
//            float otherT = someObj->FindIntersection(lightRay, hit.hitPoint);
//
//            //if(light
//        }
//
//        //sumDiffuseSpecular += calcDiffuse(Kd, N, L, lightIntensity) + calcSpecular(Ks, V, R, n, lightIntensity);
//    }
//
//    return Ie + Ka * Ia + sumDiffuseSpecular;
//}