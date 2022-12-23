#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "../res/includes/glm/glm.hpp"
#include "../Engine3D/scene.h"

using namespace std;
using namespace glm;

class RubiksCube 
{
public:
    // Methods
    RubiksCube();
    void Create_Cube(Scene* scn, int cube_size);
    vec3 Calc_New_Angles(vec3 current_angles, vec3 angles);
    void Rotate_Cube(int i, int j, int k, vec3 rotation_direction);
    void Update_Structure();

    void CASE_R();
    void CASE_L();
    void CASE_U();
    void CASE_D();
    void CASE_B();
    void CASE_F();
    void CASE_SPACE();
    void CASE_Z();
    void CASE_A();
    void CASE_UP();
    void CASE_DOWN();
    void CASE_LEFT();
    void CASE_RIGHT();
    void CASE_P();
    void CASE_M();

    void Animate();

    // Variables
    int size;
    std::vector<Shape*>* scn_shapes;
    std::vector<std::vector<std::vector<std::pair<int, vec3>>>> cubes_angles;
    std::vector<std::vector<std::vector<std::pair<int, vec3>>>> cubes_centers;

    vec3 current_center;
    int clock_direction;
    int rotation_angle;
    int rotation_per_frame;

    bool activate_animation;
    bool animating;
    char action;
    int num_of_actions;
};