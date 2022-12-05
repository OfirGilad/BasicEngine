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
            if ((i == 200) && (j == 600)) {
                cout << "TEST" << endl;
            }
            vec3 ray = ConstructRayThroughPixel(i, j);
            Hit hit = FindIntersection(ray);
            vec4 pixel_color = GetColor(ray, hit);

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

Hit SceneData::FindIntersection(vec3 ray) {
    // Set Default Values
    float min_t = INFINITY;
    Model* min_primitive = new Plane(vec4(1.0, 1.0, 1.0, 1.0), Regular);
    min_primitive->setColor(vec4(0.0, 0.0, 0.0, 0.0));
    bool got_hit = false;

    // Looping over all the objects
    for (int i = 0; i < objects.size(); i++) {
        float t = objects[i]->FindIntersection(ray, eye);
        //float t = Intersect(ray, objects[i]);

        if (t < min_t) {
            got_hit = true;
            min_primitive = objects[i];
            min_t = t;
        }
    }

    Hit hit = Hit(eye + ray, min_primitive);
    if (got_hit) {
        hit = Hit(eye + ray * min_t, min_primitive);
    }
    return hit;
}

vec4 SceneData::GetColor(vec3 ray, Hit hit) {
    vec3 color = hit.obj->getColor(hit.hitPoint);
    
    //vec3 final_color = color * vec3(ambient.r, ambient.g, ambient.b);
    //vec3 final_color = vec3(0, 0, 0);
    vec3 final_color = vec3(ambient.r, ambient.g, ambient.b) * color;

    vec3 total_diffuse_color = vec3(0, 0, 0);
    vec3 total_specular_color = vec3(0, 0, 0);
    vec3 sum_color = vec3(1, 1, 1);
    
    for (int i = 0; i < lights.size(); i++) {
        vec3 diffuse_color = color * calcDiffuseColor(hit, lights[i]);
        vec3 specular_color = calcSpecularColor(hit, lights[i]);
        float shadow_term = calcShadowTerm(hit, lights[i]);

        //total_diffuse_color += max(diffuse_color, vec3(0, 0, 0));
        //total_specular_color += max(specular_color, vec3(0, 0, 0));
         
        if (shadow_term > 0.5) {
            total_diffuse_color += max(diffuse_color, vec3(0, 0, 0));
            total_specular_color += max(specular_color, vec3(0, 0, 0));
        }
        //sum_color *= shadow_term;

    }
    final_color += total_diffuse_color + total_specular_color;
    //final_color = sum_color;
    final_color = min(final_color, vec3(1.0, 1.0, 1.0));

    return vec4(final_color.r, final_color.g, final_color.b, 0.0);
}

vec3 SceneData::calcDiffuseColor(Hit hit, Light* light) {
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

    // N^*L^ = cos(t)
    float hit_cos_value = dot(object_normal, -normalized_ray_direction);

    // Id = Kd*(N^*L^)*Il
    vec3 diffuse_color = hit_cos_value * light->rgb_intensity;
    return diffuse_color;
}

vec3 SceneData::calcSpecularColor(Hit hit, Light* light) {
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
    vec3 obj_to_eye_vec = normalizedVector(eye - hit.hitPoint);

    // V^*R^ = max(0, V^*R^)
    float hit_cos_value = dot(obj_to_eye_vec, reflected_light_ray);
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

    if (light->liType == Spot) {
        vec3 virtual_spotlight_ray = normalizedVector(hit.hitPoint - light->position);
        float light_cos_value = dot(virtual_spotlight_ray, normalized_ray_direction);

        if (light_cos_value < light->cosAngle) {
            return 0.0;
        }
        else {
            normalized_ray_direction = virtual_spotlight_ray;
        }
    }

    // Checking the path between the light source and the object
    float min_t = INFINITY;
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