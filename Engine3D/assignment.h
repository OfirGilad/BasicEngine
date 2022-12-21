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
    void Update_Structure(int value, char ijk);
    float Absolute_Difference(mat4 mat1, mat4 mat2);

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
    void CASE_M(); 

    // Variables
    int size;
    std::vector<Shape*>* scn_shapes;
    std::vector<std::vector<std::vector<std::pair<int, vec3>>>> cube_angles;
    std::vector<std::vector<std::vector<std::pair<int, vec3>>>> cube_centers;
    
    //std::vector<std::vector<std::vector<std::pair<int, mat4>>>> cube_translation;
    //std::vector<std::vector<std::vector<std::pair<int, mat4>>>> cube_rotation;
    //std::vector<std::vector<std::vector<std::pair<int, mat4>>>> cube_scale;

    vec3 current_center;
    int clock_direction;
    int rotation_angle;
};