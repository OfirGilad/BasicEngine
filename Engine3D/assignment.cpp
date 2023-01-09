#include "assignment.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "scene.h"

using namespace std;
using namespace glm;

Route3DBezier1D::Route3DBezier1D()
{
}

// TODO: build 6 segments and display only 3
// Building 3D route with by manipulating 1D Bezier curve
void Route3DBezier1D::CreateRoute3DBezier1D(Scene* scn, int segNum, int res, int mode) {
    scn_shapes = scn->GetShapes();
    int shape_index = 0;

    // Cube + Octahedrons texture
    scn->AddTexture("../res/textures/box0.bmp", false);

    // Bezier 1D texture
    scn->AddTexture("../res/textures/grass.bmp", false);

    bezier_1D = new Bezier1D(segNum, res, Scene::LINE_STRIP);

    BuildAllShapes(scn);
    NumberOfSegmentsToDisplay(segNum);

    // Building Bezier 1D
    bezier_1D->SetBezier1DMesh(bezier_1D->GetLine());
    scn->AddBezier1DShape(bezier_1D, -1);
    scn->SetShapeTex((*scn_shapes).size() - 1, 1);

    scn->MoveCamera(0, Scene::zTranslate, 50);
}

// TODO: The cube face should change according to the direction of the movement
void Route3DBezier1D::AnimateCubeMovement(bool animate) {
    if (animate) {
        vec3 velo1 = normalize(bezier_1D->GetVelosity(cube_segment, cube_t));

        if (forward_direction) {
            if (cube_t > 0.99f) {
                cube_segment++;
                cube_t = 0;
            }
            else {
                cube_t += cube_step_size;
            }
            if (cube_segment == bezier_1D->GetSegmentsNum()) {
                cube_t = 1;
                cube_segment--;
                forward_direction = false;
            }
        }
        else {
            if (cube_t < 0.01f) {
                cube_segment--;
                cube_t = 1;
            }
            else {
                cube_t -= cube_step_size;
            }
            if (cube_segment == -1) {
                cube_t = 0;
                cube_segment++;
                forward_direction = true;
            }
        }

        vec4 cube_center = (*scn_shapes)[cube_shape_index]->GetTranslate()[3];
        vec4 next_position = bezier_1D->GetPointOnCurve(cube_segment, cube_t);

        vec3 velo2 = glm::normalize(bezier_1D->GetVelosity(cube_segment, cube_t));

        float angle = glm::dot(velo1, velo2);
        vec3 normal = glm::cross(velo1, velo2);

        bool condition = ((normal.x == 0) && (normal.y == 0) && (normal.z == 0));

        // IN PROGRESS
        if (!condition) {
            (*scn_shapes)[cube_shape_index]->MyRotate(angle, normal, 0);
        }

        vec4 move_vector = next_position - cube_center;

        (*scn_shapes)[cube_shape_index]->MyTranslate(vec3(move_vector.x, move_vector.y, move_vector.z), 0);
    }
}

void Route3DBezier1D::UpdateCurveByShapes() {
    vec4 p0 = (*scn_shapes)[0]->GetTranslate()[3];
    vec4 p1 = (*scn_shapes)[1]->GetTranslate()[3];
    vec4 p2 = (*scn_shapes)[2]->GetTranslate()[3];
    vec4 p3 = (*scn_shapes)[3]->GetTranslate()[3];
    bezier_1D->ChangeFirstSegment(p0, p1, p2, p3);

    int segIndx = 1;
    for (int i = 4; i < scn_shapes->size() - 2; i += 3) {
        vec4 p1 = (*scn_shapes)[i]->GetTranslate()[3];
        vec4 p2 = (*scn_shapes)[i + 1]->GetTranslate()[3];
        vec4 p3 = (*scn_shapes)[i + 2]->GetTranslate()[3];

        bezier_1D->ChangeSegment(segIndx, p1, p2, p3);
        segIndx++;
    }
    bezier_1D->SetBezier1DMesh(bezier_1D->GetLine());
}

bool Route3DBezier1D::OnCurvePoint(int index) {
    if (index % 3 == 0) {
        return true;
    }
    else {
        return false;
    }
}

bool Route3DBezier1D::HasLeft(int index) {
    if (index > first_point_index) {
        return true;
    }
    else {
        return false;
    }
}

bool Route3DBezier1D::HasRight(int index) {
    if (index < last_point_index) {
        return true;
    }
    else {
        return false;
    }
}

int Route3DBezier1D::NextShape() {
    picked_shape_index++;
    if (picked_shape_index > cube_shape_index) {
        picked_shape_index = -1;
    }
    return picked_shape_index;
}
int Route3DBezier1D::PreviousShape() {
    picked_shape_index--;
    if (picked_shape_index < -1) {
        picked_shape_index = cube_shape_index;
    }
    return picked_shape_index;
}

void Route3DBezier1D::NumberOfSegmentsToDisplay(int segNum) {
    int res = ((bezier_1D->GetResT() - 1) / bezier_1D->GetSegmentsNum() * segNum) + 1;
    bezier_1D->SetSegmentsNum(segNum);
    bezier_1D->SetResT(res);

    int config_num = segNum - 2;

    // Reset Cube position
    vec3 cube_new_position = bezier_configs[config_num][0];
    vec4 cube_old_position = (*scn_shapes)[cube_shape_index]->GetTranslate()[3];
    vec3 movement = cube_new_position - vec3(cube_old_position.x, cube_old_position.y, cube_old_position.z);
    (*scn_shapes)[cube_shape_index]->MyTranslate(movement, 0);

    for (int i = 0; i < number_of_octahedrons; i++) {
        if (i < bezier_configs[config_num].size()) {
            (*scn_shapes)[i]->Unhide();

            // Reset Octahedrons posotions
            vec3 point_new_position = bezier_configs[config_num][i];
            vec4 point_old_position = (*scn_shapes)[i]->GetTranslate()[3];
            vec3 movement = point_new_position - vec3(point_old_position.x, point_old_position.y, point_old_position.z);
            (*scn_shapes)[i]->MyTranslate(movement, 0);
        }
        else {
            (*scn_shapes)[i]->Hide();
        }
    }

    UpdateCurveByShapes();
    bezier_1D->SetBezier1DMesh(bezier_1D->GetLine());
}

void Route3DBezier1D::BuildAllShapes(Scene* scn) {
    int shape_index = 0;
    first_point_index = shape_index;

    // Octahedrons
    for (int i = 0; i < number_of_octahedrons; i++) {
        scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
        scn->SetShapeTex(shape_index, 0);
        (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
        (*scn_shapes)[shape_index]->Hide();
        shape_index++;
    }

    // Cube
    scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);

    last_point_index = shape_index - 1;
    cube_shape_index = shape_index;

    // Setting segments
    vec4 zero_vector = vec4(0, 0, 0, 0);
    bezier_1D->AddFirstSegment(zero_vector, zero_vector, zero_vector, zero_vector);

    for (int i = 4; i < number_of_octahedrons; i += 3)
        bezier_1D->AddSegment(zero_vector, zero_vector, zero_vector);

    // Configurations
    std::vector<glm::vec3> config2;
    config2.resize(7);
    config2[0] = vec3(-9, -9, 0);
    config2[1] = vec3(-9, -3, 0);
    config2[2] = vec3(-6, 0, 0);
    config2[3] = vec3(0, 0, 0);
    config2[4] = vec3(6, 0, 0);
    config2[5] = vec3(9, -3, 0);
    config2[6] = vec3(9, -9, 0);

    std::vector<glm::vec3> config3;
    config3.resize(10);
    config3[0] = vec3(-15, -9, 0);
    config3[1] = vec3(-15, -3, 0);
    config3[2] = vec3(-12, 0, 0);
    config3[3] = vec3(-6, 0, 0);
    config3[4] = vec3(-3, 0, 0);
    config3[5] = vec3(3, 0, 0);
    config3[6] = vec3(6, 0, 0);
    config3[7] = vec3(12, 0, 0);
    config3[8] = vec3(15, -3, 0);
    config3[9] = vec3(15, -9, 0);


    bezier_configs.push_back(config2);
    bezier_configs.push_back(config3);
}