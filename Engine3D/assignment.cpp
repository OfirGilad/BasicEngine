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

void RubiksCube::Create_Cube(Scene* scn, int cube_size) {
    scn->AddTexture("../res/textures/plane.png", false);

    size = cube_size;
    scn_shapes = scn->getShapes();
    float distance = float(size) - ((float(size) + 1.f) * 0.5f);
    int index = 0;

    for (float i = -distance; i <= distance; i+=1)
    {
        //std::vector<std::vector<std::pair<int, mat4>>> sub_translation1;
        //std::vector<std::vector<std::pair<int, mat4>>> sub_rotation1;
        //std::vector<std::vector<std::pair<int, mat4>>> sub_scale1;

        std::vector<std::vector<std::pair<int, vec3>>> sub_angles1;
        std::vector<std::vector<std::pair<int, vec3>>> sub_centers1;

        for (float j = -distance; j <= distance; j+=1)
        {
            //std::vector<std::pair<int, mat4>> sub_translation2;
            //std::vector<std::pair<int, mat4>> sub_rotation2;
            //std::vector<std::pair<int, mat4>> sub_scale2;

            std::vector<std::pair<int, vec3>> sub_angles2;
            std::vector<std::pair<int, vec3>> sub_centers2;

            for (float k = -distance; k <= distance; k+=1)
            {
                scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
                scn->SetShapeTex(index, 0);
                (*scn_shapes)[index]->MyTranslate(glm::vec3(i, j, k), 0);
                (*scn_shapes)[index]->MyScale(glm::vec3(0.5, 0.5, 0.5));

                //sub_translation2.push_back(std::make_pair(index, (*scn_shapes)[index]->GetTranslate()));
                //sub_rotation2.push_back(std::make_pair(index, (*scn_shapes)[index]->GetRotate()));
                //sub_scale2.push_back(std::make_pair(index, (*scn_shapes)[index]->GetScale()));

                sub_angles2.push_back(std::make_pair(index, vec3(0, 0, 0)));
                sub_centers2.push_back(std::make_pair(index, vec3(i, j, k)));

                index++;
            }
            //sub_translation1.push_back(sub_translation2);
            //sub_rotation1.push_back(sub_rotation2);
            //sub_scale1.push_back(sub_scale2);

            sub_angles1.push_back(sub_angles2);
            sub_centers1.push_back(sub_centers2);
        }
        //cube_translation.push_back(sub_translation1);
        //cube_rotation.push_back(sub_rotation1);
        //cube_scale.push_back(sub_scale1);

        cube_angles.push_back(sub_angles1);
        cube_centers.push_back(sub_centers1);
    }
    scn->MoveCamera(0, Scene::zTranslate, 15);

    current_center = vec3(1, 1, 1);
    clock_direction = 1;
    rotation_angle = 90;
    std::cout << "Check Structure Completed" << std::endl;

    for (int j = 0; j < size; j++) {
        for (int k = 0; k < size; k++) {
            cout << cube_angles[2][j][k].first;
            cout << " ";
        }
        cout << endl;
    }
}

vec3 RubiksCube::Calc_New_Angles(vec3 current_angles, vec3 angles) {
    vec3 new_angles = current_angles + angles;

    if (new_angles.x >= 360) {
        new_angles = vec3(360, 0, 0) - new_angles;
    }
    if (new_angles.x < 0) {
        new_angles = vec3(360, 0, 0) + new_angles;
    }
    if (new_angles.y >= 360) {
        new_angles = vec3(0, 360, 0) - new_angles;
    }
    if (new_angles.y < 0) {
        new_angles = vec3(0, 360, 0) + new_angles;
    }
    if (new_angles.z >= 360) {
        new_angles = vec3(0, 0, 360) - new_angles;
    }
    if (new_angles.z < 0) {
        new_angles = vec3(0, 0, 360) + new_angles;
    }
    return new_angles;
}


// Note to self:
// To find the cube new position:
// new_center = rotation_matrix_3x3 * original_center
// Example: 
// original_center = (1, -1, -1)
// rotation_matrix_after_90_angle = (1, 0, 0)
//                                  (0, 0, 1)
//                                  (0, -1, 0)
// new_center = (1, -1, 1)
// 


//// From here the problematic part

void RubiksCube::Update_Structure(int value, char ijk) {
    std::vector<std::vector<std::vector<std::pair<int, vec3>>>> cube_angles_copy = cube_angles;
    
    if (ijk == 'i') {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                int get_index = cube_angles_copy[value][j][k].first;

                glm::mat4 get_rotation = (*scn_shapes)[get_index]->GetRotate();
                glm::mat3 cube_rotation = glm::mat3(
                    vec3(get_rotation[0].x, get_rotation[0].y, get_rotation[0].z),
                    vec3(get_rotation[1].x, get_rotation[1].y, get_rotation[1].z),
                    vec3(get_rotation[2].x, get_rotation[2].y, get_rotation[2].z)
                );

                //glm::mat4 get_translation = (*scn_shapes)[get_index]->GetTranslate();
                //glm::vec3 cube_position = glm::vec3(get_translation[3].x, get_translation[3].y, get_translation[3].z);

                glm::vec3 cube_position = cube_centers[value][j][k].second;

                glm::vec3 new_cube_position = cube_position * cube_rotation;
                glm::vec3 movement = new_cube_position - cube_position;
                glm::vec3 final_movement = glm::vec3(round(movement.x), round(movement.y), round(movement.z));

                cube_angles[value][j + final_movement.y][k + final_movement.z].first = get_index;
            }
        }

        // Debug for R
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                cout << cube_angles[value][j][k].first;
                cout << " ";
            }
            cout << endl;
        }
    }
}

float RubiksCube::Absolute_Difference(glm::mat4 mat1, glm::mat4 mat2) {
    glm::mat4 mat_sub = mat1 - mat2;
    float abs_diff = 0;

    //for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            abs_diff += abs(mat_sub[3][j]);
            //std::cout << mat_sub[j][i];
            std::cout << " ";
        }
        std::cout << std::endl;
    //}
    std::cout << "DONE" << std::endl;
    return abs_diff;
}

//// Until here the problematic part



// 'R' press state for right wall rotation (90 degrees clockwise).
void RubiksCube::CASE_R() {
    int i = current_center.x + 1;

    if (i == size) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int j = 0; j < size; j++)
    {
        for (int k = 0; k < size; k++)
        {
            int get_index = cube_angles[i][j][k].first;

            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(clock_direction, 0, 0), 0);

            // Update structure
            cube_angles[i][j][k].second = Calc_New_Angles(cube_angles[i][j][k].second, glm::vec3(rotation_angle * clock_direction, 0, 0));
        }
    }
    Update_Structure(i, 'i');
}

// 'L' press state for left wall rotation (90 degrees clockwise).
void RubiksCube::CASE_L() {
    int i = current_center.x - 1;

    if (i == -1) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int j = 0; j < size; j++)
    {
        for (int k = 0; k < size; k++)
        {
            int get_index = cube_angles[i][j][k].first;
            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(clock_direction, 0, 0), 0);

            // Update structure
            cube_angles[i][j][k].second = Calc_New_Angles(cube_angles[i][j][k].second, glm::vec3(rotation_angle * clock_direction, 0, 0));
        }
    }
}

// 'U' press state for up wall rotation(90 degrees clockwise).
void RubiksCube::CASE_U() {
    int j = current_center.y + 1;

    if (j == size) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            int get_index = cube_angles[i][j][k].first;
            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(0, clock_direction, 0), 0);

            // Update structure
            cube_angles[i][j][k].second = Calc_New_Angles(cube_angles[i][j][k].second, glm::vec3(0, rotation_angle * clock_direction, 0));
        }
    }
}

// 'D' press state for down wall rotation (90 degrees clockwise).
void RubiksCube::CASE_D() {
    int j = current_center.y - 1;

    if (j == -1) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            int get_index = cube_angles[i][j][k].first;
            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(0, clock_direction, 0), 0);

            // Update structure
            cube_angles[i][j][k].second = Calc_New_Angles(cube_angles[i][j][k].second, glm::vec3(0, rotation_angle * clock_direction, 0));
        }
    }
}

// 'B' press state for back wall rotation (90 degrees clockwise).
void RubiksCube::CASE_B() {
    int k = current_center.z - 1;

    if (k == -1) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int get_index = cube_angles[i][j][k].first;
            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(0, 0, clock_direction), 0);

            // Update structure
            cube_angles[i][j][k].second = Calc_New_Angles(cube_angles[i][j][k].second, glm::vec3(0, 0, rotation_angle * clock_direction));
        }
    }
}

// 'F' press state for front wall rotation (90 degrees clockwise).
void RubiksCube::CASE_F() {
    int k = current_center.z + 1;

    if (k == size) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int get_index = cube_angles[i][j][k].first;
            (*scn_shapes)[get_index]->MyRotate(rotation_angle, glm::vec3(0, 0, clock_direction), 0);

            // Update structure
            cube_angles[i][j][k].second = Calc_New_Angles(cube_angles[i][j][k].second, glm::vec3(0, 0, rotation_angle * clock_direction));
        }
    }
}

// ' ' press state for flipping rotation direction (from clockwise to counter clockwise or vise versa).
void RubiksCube::CASE_SPACE() {
    clock_direction *= -1;
}

// 'Z' press state: dividing rotation angle by 2;
void RubiksCube::CASE_Z() {
    if (rotation_angle > 90) {
        rotation_angle = rotation_angle / 2;
    }
    else {
        std::cout << "Rotation is already at minimum of 90" << std::endl;
    }
}

// 'A' press state: multiply rotation angle by 2 (until maximum of 180);
void RubiksCube::CASE_A() {
    if (rotation_angle < 180) {
        rotation_angle = rotation_angle * 2;
    }
    else {
        std::cout << "Rotation is already at maximum of 180" << std::endl;
    }
}

// Moving center of rotation up
void RubiksCube::CASE_UP() {
    if (current_center.y < size - 1) {
        current_center += vec3(0, 1, 0);
    }
    else {
        std::cout << "Moving center failed" << std::endl;
    }
}

// Moving center of rotation down
void RubiksCube::CASE_DOWN() {
    if (current_center.y > 0) {
        current_center -= vec3(0, 1, 0);
    }
    else {
        std::cout << "Moving center failed" << std::endl;
    }
}

// Moving center of rotation left
void RubiksCube::CASE_LEFT() {
    if (current_center.x > 0) {
        current_center -= vec3(1, 0, 0);
    }
    else {
        std::cout << "Moving center failed" << std::endl;
    }
}

// Moving center of rotation right
void RubiksCube::CASE_RIGHT() {
    if (current_center.x < size - 1) {
        current_center += vec3(1, 0, 0);
    }
    else {
        std::cout << "Moving center failed" << std::endl;
    }
}

// Mixer
void RubiksCube::CASE_M() {
    std::cout.setstate(std::ios_base::failbit);
    ofstream mixer_file;
    mixer_file.open("../mixer.txt");

    for (int i = 0; i < 15; i++) {
        int random_integer;
        int lowest = 0, highest = 10;
        int range = (highest - lowest) + 1;
        random_integer = lowest + rand() % range;

        mixer_file << random_integer;

        if (random_integer == 0) {
            CASE_R();
            mixer_file << " - CASE_R()";
        }
        if (random_integer == 1) {
            CASE_L();
            mixer_file << " - CASE_L()";
        }
        if (random_integer == 2) {
            CASE_U();
            mixer_file << " - CASE_U()";
        }
        if (random_integer == 3) {
            CASE_D();
            mixer_file << " - CASE_D()";
        }
        if (random_integer == 4) {
            CASE_B();
            mixer_file << " - CASE_B()";
        }
        if (random_integer == 5) {
            CASE_F();
            mixer_file << " - CASE_F()";
        }
        if (random_integer == 6) {
            CASE_SPACE();
            mixer_file << " - CASE_SPACE()";
        }
        if (random_integer == 7) {
            CASE_UP();
            mixer_file << " - CASE_UP()";
        }
        if (random_integer == 8) {
            CASE_DOWN();
            mixer_file << " - CASE_DOWN()";
        }
        if (random_integer == 9) {
            CASE_LEFT();
            mixer_file << " - CASE_LEFT()";
        }
        if (random_integer == 10) {
            CASE_RIGHT();
            mixer_file << " - CASE_RIGHT()";
        }
        mixer_file << endl;
    }
    std::cout.clear();
    mixer_file.close();

    // Reset center
    current_center = vec3(1, 1, 1);
}