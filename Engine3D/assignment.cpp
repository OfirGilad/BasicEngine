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

// Building 3D route with by manipulating 1D Bezier curve
void Route3DBezier1D::Create_Route3DBezier1D(Scene* scn, int segNum, int res, int mode) {
    scn_shapes = scn->getShapes();
    int shape_index = 0;

    // Cube + Octahedrons texture
    scn->AddTexture("../res/textures/box0.bmp", false);

    // Bezier 1D texture
    scn->AddTexture("../res/textures/grass.bmp", false);

    // Cube
    scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);
    (*scn_shapes)[shape_index]->MyTranslate(vec3(-15, -9, 0), 0);
    shape_index++;

    // Octahedrons
    // p0
    scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);
    (*scn_shapes)[shape_index]->MyTranslate(vec3(-15, -9, 0), 0);
    (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
    shape_index++;

    // p1
    scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);
    (*scn_shapes)[shape_index]->MyTranslate(vec3(-17, -3, 0), 0);
    (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
    shape_index++;

    // p2
    scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);
    (*scn_shapes)[shape_index]->MyTranslate(vec3(-12, 1, 0), 0);
    (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
    shape_index++;

    // p3
    scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);
    (*scn_shapes)[shape_index]->MyTranslate(vec3(-6, 0, 0), 0);
    (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
    shape_index++;

    // p4
    scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);
    (*scn_shapes)[shape_index]->MyTranslate(vec3(-3, 0, 0), 0);
    (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
    shape_index++;

    // p5
    scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);
    (*scn_shapes)[shape_index]->MyTranslate(vec3(3, 0, 0), 0);
    (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
    shape_index++;

    // p6
    scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);
    (*scn_shapes)[shape_index]->MyTranslate(vec3(6, 0, 0), 0);
    (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
    shape_index++;

    // p7
    scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);
    (*scn_shapes)[shape_index]->MyTranslate(vec3(12, 1, 0), 0);
    (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
    shape_index++;

    // p8
    scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);
    (*scn_shapes)[shape_index]->MyTranslate(vec3(17, -3, 0), 0);
    (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
    shape_index++;

    // p9
    scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);
    (*scn_shapes)[shape_index]->MyTranslate(vec3(15, -9, 0), 0);
    (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
    shape_index++;


    bezier_1D = new Bezier1D(segNum, res, Scene::LINE_STRIP);

    bezier_1D->AddFirstSegment(
        (*scn_shapes)[1]->GetTranslate()[3],
        (*scn_shapes)[2]->GetTranslate()[3],
        (*scn_shapes)[3]->GetTranslate()[3],
        (*scn_shapes)[4]->GetTranslate()[3]
    );

    bezier_1D->AddSegment(
        (*scn_shapes)[5]->GetTranslate()[3],
        (*scn_shapes)[6]->GetTranslate()[3],
        (*scn_shapes)[7]->GetTranslate()[3]
    );

    bezier_1D->AddSegment(
        (*scn_shapes)[8]->GetTranslate()[3],
        (*scn_shapes)[9]->GetTranslate()[3],
        (*scn_shapes)[10]->GetTranslate()[3]
    );

    bezier_1D->SetBezier1DMesh(bezier_1D->GetLine());

    // Those 3 lines cause the error at the end
    scn->AddBezier1DShape(bezier_1D, -1);
    scn->SetShapeTex(shape_index, 1);
    shape_index++;


    scn->MoveCamera(0, Scene::zTranslate, 50);
}

void Route3DBezier1D::AnimateCubeMovement(bool animate) {
    if (animate) {
        if (foward_direction) {
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
                foward_direction = false;
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
                foward_direction = true;
            }
        }

        glm::vec4 cube_center = (*scn_shapes)[0]->GetTranslate()[3];
        glm::vec4 next_position = bezier_1D->GetPointOnCurve(cube_segment, cube_t);
        glm::vec4 move_vector = next_position - cube_center;

        (*scn_shapes)[0]->MyTranslate(vec3(move_vector.x, move_vector.y, move_vector.z), 0);
    }
}