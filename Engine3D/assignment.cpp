#include "assignment.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "assignment_utilities.h"

using namespace std;
using namespace glm;

// Set Recursive Steps Max Level
int MAX_LEVEL = 5;

SceneData::SceneData(string file_name, int width, int height) {
    bonus_mode_flag = 0.f;
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
        objects[i]->object_index = i;
        objects[i]->setColor(colors[i]);
    }

    // Set spotlight positions
    int j = 0;
    for (int i = 0; i < lights.size(); i++) {
        if (lights[i]->light_type == Spot) {
            vec3 point = vec3(positions[j].x, positions[j].y, positions[j].z);
            float cos_angle = positions[j++].w;
            lights[i]->position = point;
            lights[i]->cos_angle = cos_angle;
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

    // Looping over all the pixels
    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            vec3 pixel_color;

            // Single Sampling
            if (bonus_mode_flag < 0.5) {
                Ray ray = ConstructRayThroughPixel(i, j, 0);
                Hit hit = FindIntersection(ray);
                pixel_color = GetColor(ray, hit, 0);
            }
            // Multi Sampling (Bonus)
            else {
                Ray ray1 = ConstructRayThroughPixel(i, j, 1);
                Ray ray2 = ConstructRayThroughPixel(i, j, 2);
                Ray ray3 = ConstructRayThroughPixel(i, j, 3);
                Ray ray4 = ConstructRayThroughPixel(i, j, 4);
                Hit hit1 = FindIntersection(ray1);
                Hit hit2 = FindIntersection(ray2);
                Hit hit3 = FindIntersection(ray3);
                Hit hit4 = FindIntersection(ray4);
                vec3 pixel_color1 = GetColor(ray1, hit1, 0);
                vec3 pixel_color2 = GetColor(ray2, hit2, 0);
                vec3 pixel_color3 = GetColor(ray3, hit3, 0);
                vec3 pixel_color4 = GetColor(ray4, hit4, 0);
                pixel_color = (pixel_color1 + pixel_color2 + pixel_color3 + pixel_color4) / 4.0f;
            }

            image.setColor(i, j, vec4(pixel_color.r, pixel_color.g, pixel_color.b, 0.0));
        }
    }
    return image;
}

Ray SceneData::ConstructRayThroughPixel(int i, int j, int position_on_pixel) {
    // top-left     (-1,  1)
    // top-right    ( 1,  1)
    // bottom-left  (-1, -1)
    // bottom-right ( 1, -1)
    vec3 top_left_point, hit_on_screen, ray_direction;

    // Pixel center hit
    if (position_on_pixel == 0) {
        top_left_point = vec3(-1 + (pixel_width / 2), 1 - (pixel_height / 2), 0);
        hit_on_screen = top_left_point + vec3(i * pixel_width, -1 * (j * pixel_height), 0);
    }
    else {
        top_left_point = vec3(-1 + (pixel_width / 4), 1 - (pixel_height / 4), 0);

        // Pixel center top left hit
        if (position_on_pixel == 1) {
            hit_on_screen = top_left_point + vec3(i * pixel_width, -1 * (j * pixel_height), 0);
        }
        // Pixel center top right hit
        if (position_on_pixel == 2) {
            hit_on_screen = top_left_point + vec3((i * pixel_width) + (pixel_width / 2), -1 * (j * pixel_height), 0);
        }
        // Pixel center bottom left hit
        if (position_on_pixel == 3) {
            hit_on_screen = top_left_point + vec3(i * pixel_width, -1 * ((j * pixel_height) + (pixel_height / 2)), 0);
        }
        // Pixel center bottom right hit
        if (position_on_pixel == 4) {
            hit_on_screen = top_left_point + vec3((i * pixel_width) + (pixel_width / 2), -1 * ((j * pixel_height) + (pixel_height / 2)), 0);
        }
    }
    ray_direction = normalizedVector(hit_on_screen - eye);
    Ray ray = Ray(ray_direction, eye);
    
    return ray;
}

Hit SceneData::FindIntersection(Ray ray) {
    // Set Default Values
    float min_t = INFINITY;
    SceneObject* min_primitive = new Plane(vec4(1.0, 1.0, 1.0, 1.0), Space);
    min_primitive->object_index = -1;
    min_primitive->setColor(vec4(0.0, 0.0, 0.0, 0.0));
    bool got_hit = false;

    // Looping over all the objects
    for (int i = 0; i < objects.size(); i++) {
        float t = objects[i]->Intersect(ray);

        // Checking if there was intersection
        if ((t >= 0) && (t < min_t)) {
            got_hit = true;
            min_primitive = objects[i];
            min_t = t;
        }
    }

    // Returning result
    Hit hit = Hit(ray.origin + ray.direction, min_primitive);
    if (got_hit) {
        hit = Hit(ray.origin + ray.direction * min_t, min_primitive);
    }
    return hit;
}

vec3 SceneData::GetColor(Ray ray, Hit hit, int depth) {
    // Stop the Recursion
    if (depth >= MAX_LEVEL) {
        return vec3(0.f, 0.f, 0.f);
    }

    // Calculating Phong Model Color:
    // I = I(emission) + K(ambient) * I(ambient) + Sum[ (K(diffuse) * dot(N,L) + K(specular) * dot(V,R)^n) * S(term) * I(light) ] + K(reflection) * I(reflection)
    vec3 phong_model_color = vec3(0.0, 0.0, 0.0);

    // Regular case
    if (hit.scene_object->object_type == Regular) {
        vec3 object_color = vec3(0.0, 0.0, 0.0);

        // Emission color
        vec3 emission_color = vec3(0.0, 0.0, 0.0); // Emission
        object_color += emission_color;

        // Ambient color
        vec3 ambient_color = max(calcAmbientColor(hit), vec3(0, 0, 0)); // Ambient
        object_color += ambient_color ; 

        // Sum of Diffuse, Specular and Shadow color
        for (int i = 0; i < lights.size(); i++) {
            vec3 diffuse_color = max(calcDiffuseColor(hit, lights[i]), vec3(0, 0, 0)); // Diffuse
            vec3 specular_color = max(calcSpecularColor(ray, hit, lights[i]), vec3(0, 0, 0)); // Specular
            float shadow_term = calcShadowTerm(hit, lights[i]); // Shadow

            object_color += (diffuse_color + specular_color) * shadow_term * lights[i]->rgb_intensity;
        }
        phong_model_color += min(object_color, vec3(1.0, 1.0, 1.0));
    }

    // Reflective case
    if (hit.scene_object->object_type == Reflective) {
        vec3 reflection_color = vec3(0.f, 0.f, 0.f);

        // Finding the reflected ray
        vec3 reflection_ray_direction = ray.direction - 2.0f * hit.scene_object->getNormal(hit.hit_point) * dot(ray.direction, hit.scene_object->getNormal(hit.hit_point));
        Ray reflection_ray = Ray(reflection_ray_direction, hit.hit_point);

        Hit reflected_hit = FindIntersection(reflection_ray);

        if (reflected_hit.scene_object->object_type == Space) {
            return vec3(0.f, 0.f, 0.f);
        }

        // Reflection color
        reflection_color += GetColor(reflection_ray, reflected_hit, depth + 1);
        phong_model_color = min(reflection_color, vec3(1.0, 1.0, 1.0));
    }

    // Transparent case
    if (hit.scene_object->object_type == Transparent) {
        vec3 transparency_color = vec3(0.f, 0.f, 0.f);

        // Transparent Plane
        if (hit.scene_object->details.w < 0.0) {
            Ray ray_through = Ray(ray.direction, hit.hit_point);

            Hit transparency_hit = FindIntersection(ray_through);

            if (transparency_hit.scene_object->object_type == Space) {
                return vec3(0.f, 0.f, 0.f);
            }

            transparency_color += GetColor(ray_through, transparency_hit, depth + 1);
        }
        // Transparent Sphere
        else {
            vec3 hit_point = hit.hit_point;
            float n1 = 1.0;
            float n2 = 1.5;

            // Snell's law
            float pi = 3.14159265;
            float cos_from = dot(hit.scene_object->getNormal(hit_point), -ray.direction);
            float theta_from = acos(cos_from) * (180.0f / pi);
            float snell_frac = (n1 / n2);
            float sin_from = sin(theta_from * (pi / 180.0f));
            float sin_to = snell_frac * sin_from;
            float theta_to = asin(sin_to) * (180.0f / pi);
            float cos_to = cos(theta_to * (pi / 180.0f));

            // Finding the second hit inside the sphere
            vec3 ray_in_direction = (snell_frac * cos_from - cos_to) * hit.scene_object->getNormal(hit_point) - snell_frac * (-ray.direction);
            ray_in_direction = normalizedVector(ray_in_direction);
            //vec3 ray_in_direction = normalizedVector(refract(ray.direction, hit.scene_object->getNormal(hit.hit_point), snell_frac));
            Ray ray_in = Ray(ray_in_direction, hit_point);

            Hit transparency_hit = FindIntersection(ray_in);

            // Second hit inside the sphere to other object
            if ((transparency_hit.scene_object->object_index != hit.scene_object->object_index) && (transparency_hit.scene_object->object_type != Transparent)){
                transparency_color += GetColor(ray_in, transparency_hit, depth + 1);
            }
            // Second hit inside the sphere to outside
            else {
                float t = hit.scene_object->Intersect(ray_in);
                hit_point = ray_in.origin + ray_in.direction * t;
                n1 = 1.5;
                n2 = 1.0;

                // Reverse calculations
                cos_from = dot(-hit.scene_object->getNormal(hit_point), -ray_in.direction);
                theta_from = acos(cos_from) * (180.0f / pi);
                snell_frac = (n1 / n2);
                sin_from = sin(theta_from * (pi / 180.0f));
                sin_to = snell_frac * sin_from;
                theta_to = asin(sin_to) * (180.0f / pi);
                cos_to = cos(theta_to * (pi / 180.0f));

                // Finding the ray out of the sphere
                vec3 ray_out_direction = (snell_frac * cos_from - cos_to) * -hit.scene_object->getNormal(hit_point) - snell_frac * (-ray_in.direction);
                ray_out_direction = normalizedVector(ray_out_direction);
                //vec3 ray_out_direction = normalizedVector(refract(ray_in.direction, -hit.scene_object->getNormal(hit_point), snell_frac));
                Ray ray_out = Ray(ray_out_direction, hit_point);

                Hit transparency_hit = FindIntersection(ray_out);

                if (transparency_hit.scene_object->object_type == Space) {
                    return vec3(0.f, 0.f, 0.f);
                }

                transparency_color += GetColor(ray_out, transparency_hit, depth + 1);
            }
        }
        phong_model_color += min(transparency_color, vec3(1.0, 1.0, 1.0));
    }
    return phong_model_color;
}

vec3 SceneData::calcAmbientColor(Hit hit) {
    vec3 material_ambient = hit.scene_object->getColor(hit.hit_point);
    vec3 global_ambient_intensity = vec3(ambient.r, ambient.g, ambient.b);

    vec3 ambient_color = material_ambient * global_ambient_intensity;
    return ambient_color;
}


vec3 SceneData::calcDiffuseColor(Hit hit, Light* light) {
    vec3 normalized_ray_direction = vec3(0.0, 0.0, 0.0);

    // Spotlight
    if (light->light_type == Spot) {
        normalized_ray_direction += normalizedVector(hit.hit_point - light->position);
    }
    // Directional light
    else {
        normalized_ray_direction += normalizedVector(light->direction);
    }
    vec3 object_normal = hit.scene_object->getNormal(hit.hit_point);

    // N^*L^ = cos(t)
    float hit_cos_value = dot(object_normal, -normalized_ray_direction);

    // Id = Kd*(N^*L^)*Il
    vec3 material_diffuse = hit.scene_object->getColor(hit.hit_point);
    vec3 diffuse_color = material_diffuse * hit_cos_value;
    return diffuse_color;
}

vec3 SceneData::calcSpecularColor(Ray ray, Hit hit, Light* light) {
    vec3 normalized_ray_direction = vec3(0.0, 0.0, 0.0);

    // Spotlight
    if (light->light_type == Spot) {
        normalized_ray_direction += normalizedVector(hit.hit_point - light->position);
    }
    // Directional light
    else {
        normalized_ray_direction += normalizedVector(light->direction);
    }
    vec3 object_normal = hit.scene_object->getNormal(hit.hit_point);
    vec3 reflected_light_ray = normalized_ray_direction - 2.0f * object_normal * dot(normalized_ray_direction, object_normal);
    vec3 ray_to_viewer = normalizedVector(ray.origin - hit.hit_point);

    // V^*R^ = max(0, V^*R^)
    float hit_cos_value = dot(ray_to_viewer, reflected_light_ray);
    hit_cos_value = glm::max(0.0f, hit_cos_value);

    // (V^*R^)^n
    hit_cos_value = pow(hit_cos_value, hit.scene_object->material_shininess);

    // Id = Ks*(V^*R^)^n*Il
    vec3 material_specular = vec3(0.7, 0.7, 0.7);
    vec3 specular_color = material_specular * hit_cos_value;
    return specular_color;
}

float SceneData::calcShadowTerm(Hit hit, Light* light) {
    vec3 normalized_ray_direction = normalizedVector(light->direction);
    float min_t = INFINITY;

    // Spotlight special case
    if (light->light_type == Spot) {
        vec3 virtual_spotlight_ray = normalizedVector(hit.hit_point - light->position);
        float light_cos_value = dot(virtual_spotlight_ray, normalized_ray_direction);

        // Checking if the spotlight rays hit the object
        float light_angle = acos(light_cos_value);
        float cutoff_angle = acos(light->cos_angle);
        if (light_angle < cutoff_angle) {
            normalized_ray_direction = virtual_spotlight_ray;

            // Update min_t to the value of the length from the hit point to the light position
            min_t = length(light->position - hit.hit_point);
        }
        else {
            return 0.0;
        }
    }

    // Checking the path between the object and the light source by looping over all the objects
    for (int i = 0; i < objects.size(); i++) {
        if (i != hit.scene_object->object_index) {
            Ray ray = Ray(-normalized_ray_direction, hit.hit_point);
            float t = objects[i]->Intersect(ray);

            if ((t > 0) && (t < min_t)) {
                return 0.0;
            }
        }
    }
    return 1.0;
}
