#include "assignment.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "scene.h"

using namespace std;
using namespace glm;


RubiksCube::RubiksCube() 
{
}

void RubiksCube::CreateCube(Scene* scn, int size) {
    this->size = size;

    scn->AddTexture("../res/textures/plane.png", false);

    scn_shapes = scn->getShapes();
    //(*scn_shapes)[0]->MyScale(glm::vec3(0.5, 0.5, 0.5));
    //(*scn_shapes)[0]->MyTranslate(glm::vec3(1, 0, 0), 0);
    //(*scn_shapes)[0]->MyRotate(60, glm::vec3(0, 1, 0), 0);

    float distance = float(size) - 0.5f;
    int index = 0;

    for (float i = -distance; i <= distance; i+=2)
    {
        std::vector<std::vector<std::pair<int, vec3>>> sub_rotation1;
        std::vector<std::vector<std::pair<int, vec3>>> sub_translation1;

        for (float j = -distance; j <= distance; j+=2)
        {
            std::vector<std::pair<int, vec3>> sub_rotation2;
            std::vector<std::pair<int, vec3>> sub_translation2;

            for (float k = -distance; k <= distance; k+=2)
            {
                scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
                scn->SetShapeTex(index, 0);
                (*scn_shapes)[index]->MyTranslate(glm::vec3(i, j, k), 0);

                sub_rotation2.push_back(std::make_pair(index, vec3(0, 0, 0)));
                sub_translation2.push_back(std::make_pair(index, vec3(i, j, k)));

                index++;
            }
            sub_rotation1.push_back(sub_rotation2);
            sub_translation1.push_back(sub_translation2);
        }
        cube_rotation.push_back(sub_rotation1);
        cube_translation.push_back(sub_translation1);
    }

    current_center = vec3(1, 1, 1);
    clock_direction = 1;
    rotation_angle = 90;
    std::cout << "Check Structure" << std::endl;
}

// 'R' press state for right wall rotation (90 degrees clockwise).
void RubiksCube::CASE_R(Scene* scn) {
    int i = current_center.x + 1;

    for (int j = 0; j < size; j++)
    {
        for (float k = 0; k < size; k++)
        {
            int get_index = cube_rotation[i][j][k].first;
            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(clock_direction, 0, 0), 0);
        }
    }
}

// 'L' press state for left wall rotation (90 degrees clockwise).
void RubiksCube::CASE_L(Scene* scn) {
    int i = current_center.x - 1;

    for (int j = 0; j < size; j++)
    {
        for (float k = 0; k < size; k++)
        {
            int get_index = cube_rotation[i][j][k].first;
            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(clock_direction, 0, 0), 0);
        }
    }
}

// 'U' press state for up wall rotation(90 degrees clockwise).
void RubiksCube::CASE_U(Scene* scn) {
    int j = current_center.y + 1;

    for (int i = 0; i < size; i++)
    {
        for (float k = 0; k < size; k++)
        {
            int get_index = cube_rotation[i][j][k].first;
            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(0, clock_direction, 0), 0);
        }
    }
}

// In Progress

// 'D' press state for down wall rotation (90 degrees clockwise).
void RubiksCube::CASE_D(Scene* scn) {
    int j = current_center.y - 1;

    for (int i = 0; i < size; i++)
    {
        for (float k = 0; k < size; k++)
        {
            int get_index = cube_rotation[i][j][k].first;
            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(0, clock_direction, 0), 0);
        }
    }
}

// 'B' press state for back wall rotation (90 degrees clockwise).
void RubiksCube::CASE_B(Scene* scn) {
    int k = current_center.z - 1;

    for (int i = 0; i < size; i++)
    {
        for (float j = 0; j < size; j++)
        {
            int get_index = cube_rotation[i][j][k].first;
            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(0, 0, clock_direction), 0);
        }
    }
}

// 'F' press state for front wall rotation (90 degrees clockwise).
void RubiksCube::CASE_F(Scene* scn) {
    int k = current_center.z + 1;

    for (int i = 0; i < size; i++)
    {
        for (float j = 0; j < size; j++)
        {
            int get_index = cube_rotation[i][j][k].first;
            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(0, 0, clock_direction), 0);
        }
    }
}

// ' ' press state for flipping rotation direction (from clockwise to counter clockwise or vise versa).
void RubiksCube::CASE_SPACE() {
    clock_direction *= -1;
}

// 'Z' press state: dividing rotation angle by 2;
void RubiksCube::CASE_Z(Scene* scn) {
    if (rotation_angle >= 180) {
        rotation_angle = rotation_angle / 2;
    }
}

// 'A' press state: multiply rotation angle by 2 (until maximum of 180);
void RubiksCube::CASE_A(Scene* scn) {
    if (rotation_angle < 180) {
        rotation_angle = rotation_angle * 2;
    }
}