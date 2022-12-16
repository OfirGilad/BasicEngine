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
    void CreateCube(Scene* scn, int size);

    void CASE_R(Scene* scn);
    void CASE_L(Scene* scn);
    void CASE_U(Scene* scn);
    void CASE_D(Scene* scn);
    void CASE_B(Scene* scn);
    void CASE_F(Scene* scn);
    void CASE_SPACE();
    void CASE_Z(Scene* scn);
    void CASE_A(Scene* scn);

    // Variables
    int size;
    std::vector<Shape*>* scn_shapes;
    std::vector < std::vector<std::vector<std::pair<int, vec3>>>> cube_translation;
    std::vector < std::vector<std::vector<std::pair<int, vec3>>>> cube_rotation;

    vec3 current_center;
    int clock_direction;
    int rotation_angle;
};