#include "assignment.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "scene.h"

using namespace std;
using namespace glm;

RubiksCube::RubiksCube() 
{
}

// Building a new Rubiks cube
void RubiksCube::Create_Cube(Scene* scn, int cube_size) {
    scn->AddTexture("../res/textures/plane.png", false);

    size = cube_size;
    scn_shapes = scn->getShapes();
    float distance = float(size) - ((float(size) + 1.f) * 0.5f);
    int index = 0;

    for (float i = -distance; i <= distance; i+=1)
    {
        vector<vector<pair<int, vec3>>> sub_centers1;
        vector<vector<int>> sub_structure1;

        for (float j = -distance; j <= distance; j+=1)
        {
            vector<pair<int, vec3>> sub_centers2;
            vector<int> sub_structure2;

            for (float k = -distance; k <= distance; k+=1)
            {
                scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
                scn->SetShapeTex(index, 0);
                (*scn_shapes)[index]->MyTranslate(vec3(i, j, k), 0);
                (*scn_shapes)[index]->MyScale(vec3(0.5, 0.5, 0.5));

                cubes_angles.push_back(vec3(0, 0, 0));
                sub_centers2.push_back(make_pair(index, vec3(i, j, k)));
                sub_structure2.push_back(index);
                
                index++;
            }
            sub_centers1.push_back(sub_centers2);
            sub_structure1.push_back(sub_structure2);
        }
        cubes_centers.push_back(sub_centers1);
        cubes_structure.push_back(sub_structure1);
    }
    scn->MoveCamera(0, Scene::zTranslate, 10);

    // Default global parameters
    current_center = vec3(1, 1, 1);
    clock_direction = 1;
    rotation_angle = 90;
    
    // Animation parameters
    multiply_factor = 8;
    rotation_per_frame = 0.125;
    unlocked = true;
    activate_animation = true;
    animating = false;

    cout << "Cube Structure Completed" << endl;
}

// Calculating the new angles of a cube
vec3 RubiksCube::Calc_New_Angles(vec3 current_angles, vec3 angles) {
    vec3 new_angles = current_angles + angles;

    if (new_angles.x >= 360) {
        new_angles = new_angles - vec3(360, 0, 0);
    }
    if (new_angles.x < 0) {
        new_angles = vec3(360, 0, 0) + new_angles;
    }
    if (new_angles.y >= 360) {
        new_angles = new_angles - vec3(0, 360, 0);
    }
    if (new_angles.y < 0) {
        new_angles = vec3(0, 360, 0) + new_angles;
    }
    if (new_angles.z >= 360) {
        new_angles = new_angles - vec3(0, 0, 360);
    }
    if (new_angles.z < 0) {
        new_angles = vec3(0, 0, 360) + new_angles;
    }
    return new_angles;
}

// Rotate a cube according to the given direction
void RubiksCube::Rotate_Cube(int i, int j, int k, vec3 rotation_direction) {
    int get_index = cubes_structure[i][j][k];

    // Getting the transposed rotation matrix
    mat4 get_rotation = (*scn_shapes)[get_index]->GetRotate();
    mat3 cube_rotation = mat3(
        vec3(get_rotation[0].x, get_rotation[0].y, get_rotation[0].z),
        vec3(get_rotation[1].x, get_rotation[1].y, get_rotation[1].z),
        vec3(get_rotation[2].x, get_rotation[2].y, get_rotation[2].z)
    );

    // Rotating the cube
    vec3 final_direction = rotation_direction * cube_rotation;
    (*scn_shapes)[get_index]->MyRotate(rotation_per_frame, final_direction, 0);

    // Updating cubes angles structure
    vec3 new_angle = vec3(rotation_direction.x * rotation_per_frame, rotation_direction.y * rotation_per_frame, rotation_direction.z * rotation_per_frame);
    cubes_angles[get_index] = Calc_New_Angles(cubes_angles[get_index], new_angle);
}

// Updating the structure holding all the current cubes locations
void RubiksCube::Update_Structure() {
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                int get_index = cubes_centers[i][j][k].first;

                // Getting the original center of the cube
                vec3 cube_position = cubes_centers[i][j][k].second;

                // Getting the current rotation of the cube
                mat4 get_rotation = (*scn_shapes)[get_index]->GetRotate();
                mat3 cube_rotation = mat3(
                    vec3(get_rotation[0].x, get_rotation[1].x, get_rotation[2].x),
                    vec3(get_rotation[0].y, get_rotation[1].y, get_rotation[2].y),
                    vec3(get_rotation[0].z, get_rotation[1].z, get_rotation[2].z)
                );

                // Calculating the new cube center
                vec3 new_cube_position = cube_position * cube_rotation;

                // Updating the new cube location on the structure
                vec3 movement = new_cube_position - cube_position;
                vec3 final_movement = vec3(round(movement.x), round(movement.y), round(movement.z));
                cubes_structure[i + final_movement.x][j + final_movement.y][k + final_movement.z] = get_index;
            }
        }
    }
}

// 'R' press state for right wall rotation (90 degrees clockwise).
void RubiksCube::CASE_R() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    int i = current_center.x + 1;

    if (i == size) {
        cout << "Invalid rotation" << endl;
        return;
    }

    if (activate_animation) {
        action = 'R';
        num_of_actions = rotation_angle * multiply_factor;
        animating = true;
    }
    else {
        int i = current_center.x + 1;
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                vec3 rotation_direction = vec3(clock_direction, 0, 0);
                Rotate_Cube(i, j, k, rotation_direction);
            }
        }
        Update_Structure();
    }
}

// 'L' press state for left wall rotation (90 degrees clockwise).
void RubiksCube::CASE_L() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    int i = current_center.x - 1;

    if (i == -1) {
        cout << "Invalid rotation" << endl;
        return;
    }

    if (activate_animation) {
        action = 'L';
        num_of_actions = rotation_angle * multiply_factor;
        animating = true;
    }
    else {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                vec3 rotation_direction = vec3(clock_direction, 0, 0);
                Rotate_Cube(i, j, k, rotation_direction);
            }
        }
        Update_Structure();
    }
}

// 'U' press state for up wall rotation(90 degrees clockwise).
void RubiksCube::CASE_U() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    int j = current_center.y + 1;

    if (j == size) {
        cout << "Invalid rotation" << endl;
        return;
    }

    if (activate_animation) {
        action = 'U';
        num_of_actions = rotation_angle * multiply_factor;
        animating = true;
    }
    else {
        for (int i = 0; i < size; i++)
        {
            for (int k = 0; k < size; k++)
            {
                vec3 rotation_direction = vec3(0, clock_direction, 0);
                Rotate_Cube(i, j, k, rotation_direction);
            }
        }
        Update_Structure();
    }
}

// 'D' press state for down wall rotation (90 degrees clockwise).
void RubiksCube::CASE_D() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    int j = current_center.y - 1;

    if (j == -1) {
        cout << "Invalid rotation" << endl;
        return;
    }

    if (activate_animation) {
        action = 'D';
        num_of_actions = rotation_angle * multiply_factor;
        animating = true;
    }
    else {
        int j = current_center.y - 1;
        for (int i = 0; i < size; i++)
        {
            for (int k = 0; k < size; k++)
            {
                vec3 rotation_direction = vec3(0, clock_direction, 0);
                Rotate_Cube(i, j, k, rotation_direction);
            }
        }
        Update_Structure();
    }    
}

// 'B' press state for back wall rotation (90 degrees clockwise).
void RubiksCube::CASE_B() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    int k = current_center.z - 1;

    if (k == -1) {
        cout << "Invalid rotation" << endl;
        return;
    }

    if (activate_animation) {
        action = 'B';
        num_of_actions = rotation_angle * multiply_factor;
        animating = true;
    }
    else {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                vec3 rotation_direction = vec3(0, 0, clock_direction);
                Rotate_Cube(i, j, k, rotation_direction);
            }
        }
        Update_Structure();
    }
}

// 'F' press state for front wall rotation (90 degrees clockwise).
void RubiksCube::CASE_F() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    int k = current_center.z + 1;

    if (k == size) {
        cout << "Invalid rotation" << endl;
        return;
    }

    if (activate_animation) {
        action = 'F';
        num_of_actions = rotation_angle * multiply_factor;
        animating = true;
    }
    else {
        int k = current_center.z + 1;
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                vec3 rotation_direction = vec3(0, 0, clock_direction);
                Rotate_Cube(i, j, k, rotation_direction);
            }
        }
        Update_Structure();
    }
}

// ' ' press state for flipping rotation direction (from clockwise to counter clockwise or vise versa).
void RubiksCube::CASE_SPACE() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    clock_direction *= -1;
}

// 'Z' press state: dividing rotation angle by 2;
void RubiksCube::CASE_Z() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    if (rotation_angle > 90) {
        rotation_angle = rotation_angle / 2;
    }
    else {
        cout << "Rotation is already at minimum of 90" << endl;
    }
}

// 'A' press state: multiply rotation angle by 2 (until maximum of 180);
void RubiksCube::CASE_A() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    if (rotation_angle < 180) {
        rotation_angle = rotation_angle * 2;
    }
    else {
        cout << "Rotation is already at maximum of 180" << endl;
    }
}

// Moving center of rotation up
void RubiksCube::CASE_UP() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    if (current_center.y < size - 1) {
        current_center += vec3(0, 1, 0);
    }
    else {
        cout << "Invalid center reposition" << endl;
    }
}

// Moving center of rotation down
void RubiksCube::CASE_DOWN() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    if (current_center.y > 0) {
        current_center -= vec3(0, 1, 0);
    }
    else {
        cout << "Invalid center reposition" << endl;
    }
}

// Moving center of rotation left
void RubiksCube::CASE_LEFT() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    if (current_center.x > 0) {
        current_center -= vec3(1, 0, 0);
    }
    else {
        cout << "Invalid center reposition" << endl;
    }
}

// Moving center of rotation right
void RubiksCube::CASE_RIGHT() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    if (current_center.x < size - 1) {
        current_center += vec3(1, 0, 0);
    }
    else {
        cout << "Invalid center reposition" << endl;
    }
}

// Moving center of rotation inside
void RubiksCube::CASE_I() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    if (current_center.z > 0) {
        current_center -= vec3(0, 0, 1);
    }
    else {
        cout << "Invalid center reposition" << endl;
    }
}

// Moving center of rotation outsize
void RubiksCube::CASE_O() {
    if (animating) {
        cout << "Please wait! Animation is in progress" << endl;
        return;
    }

    if (current_center.z < size - 1) {
        current_center += vec3(0, 0, 1);
    }
    else {
        cout << "Invalid center reposition" << endl;
    }
}

// Toggle Animation
void RubiksCube::CASE_P() {
    // Disable Animation
    if (activate_animation) {
        activate_animation = false;
        rotation_per_frame = rotation_angle;
    }
    // Enable Animation
    else {
        activate_animation = true;
        rotation_per_frame = 0.125;
    }
}

// Mixer
void RubiksCube::CASE_M() {
    // Opening mixer.txt file
    cout.setstate(ios_base::failbit);
    ofstream mixer_file;
    mixer_file.open("../mixer.txt");

    // Reset center
    current_center = vec3(1, 1, 1);

    // Disable animation
    bool previous_animation_status = activate_animation;
    activate_animation = false;
    rotation_per_frame = rotation_angle;

    // Selecting random 15 actions
    for (int i = 0; i < 15; i++) {
        int random_integer;
        int lowest = 0, highest = 12;
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
        if (random_integer == 11) {
            CASE_I();
            mixer_file << " - CASE_I()";
        }
        if (random_integer == 12) {
            CASE_O();
            mixer_file << " - CASE_O()";
        }

        mixer_file << endl;
    }

    // Closing mixer.txt file
    cout.clear();
    mixer_file.close();

    // Return animation if it was active before
    if (previous_animation_status) {
        activate_animation = true;
        rotation_per_frame = 0.125;
    }

    // Reset center
    current_center = vec3(1, 1, 1);
}

// Handle Animation
void RubiksCube::Animate() {
    // Animation is in progress
    if (animating && unlocked) {
        // Lock code area
        unlocked = false;

        if (action == 'R') {
            int i = current_center.x + 1;
            for (int j = 0; j < size; j++)
            {
                for (int k = 0; k < size; k++)
                {
                    vec3 rotation_direction = vec3(clock_direction, 0, 0);
                    Rotate_Cube(i, j, k, rotation_direction);
                }
            }
        }
        if (action == 'L') {
            int i = current_center.x - 1;
            for (int j = 0; j < size; j++)
            {
                for (int k = 0; k < size; k++)
                {
                    vec3 rotation_direction = vec3(clock_direction, 0, 0);
                    Rotate_Cube(i, j, k, rotation_direction);
                }
            }
        }
        if (action == 'U') {
            int j = current_center.y + 1;
            for (int i = 0; i < size; i++)
            {
                for (int k = 0; k < size; k++)
                {
                    vec3 rotation_direction = vec3(0, clock_direction, 0);
                    Rotate_Cube(i, j, k, rotation_direction);
                }
            }
        }
        if (action == 'D') {
            int j = current_center.y - 1;
            for (int i = 0; i < size; i++)
            {
                for (int k = 0; k < size; k++)
                {
                    vec3 rotation_direction = vec3(0, clock_direction, 0);
                    Rotate_Cube(i, j, k, rotation_direction);
                }
            }
        }
        if (action == 'B') {
            int k = current_center.z - 1;
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    vec3 rotation_direction = vec3(0, 0, clock_direction);
                    Rotate_Cube(i, j, k, rotation_direction);
                }
            }
        }
        if (action == 'F') {
            int k = current_center.z + 1;
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    vec3 rotation_direction = vec3(0, 0, clock_direction);
                    Rotate_Cube(i, j, k, rotation_direction);
                }
            }
        }

        num_of_actions--;

        // Animation completed
        if (num_of_actions == 0) {
            Update_Structure();
            animating = false;
        }

        // Unlock code area
        unlocked = true;
    }
}