#pragma once
#include <iostream>
#include <string>
#include <vector>
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
    bool Cube_Lock_Status();
    void Update_Frame_Rotation();

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
    void CASE_I();
    void CASE_O();
    void CASE_P();
    void CASE_M();

    // Animation Methods
    void Animate();

    // Variables
    int size;
    vector<Shape*>* scn_shapes;
    vector<vec3> cubes_angles;
    vector<vector<vector<pair<int, vec3>>>> cubes_centers;
    vector<vector<vector<int>>> cubes_structure;

    vec3 current_center;
    int clock_direction;
    int rotation_angle;

    // Animation Variables
    float rotation_per_frame;
    int multiply_factor;
    int num_of_actions;
    char action;
    bool unlocked;
    bool activate_animation;
    bool animating;

    bool left_right_lock1 = false;
    bool left_right_lock2 = false;
    bool up_down_lock1 = false;
    bool up_down_lock2 = false;
    bool front_back_lock1 = false;
    bool front_back_lock2 = false;
};