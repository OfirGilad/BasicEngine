#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../res/includes/glm/glm.hpp"
#include "../Engine3D/scene.h"
#include "Bezier1D.h"

using namespace std;
using namespace glm;

class Route3DBezier1D
{
public:
    // Methods
    Route3DBezier1D();
    void Create_Route3DBezier1D(Scene* scn, int segNum,int res,int mode);
    void AnimateCubeMovement(bool animate);
    void NumberOfSegmentsToDisplay(int segNum);
    void UpdateCurveByShapes();
    bool OnCurvePoint(int index);
    bool HasLeft(int index);
    bool HasRight(int index);
    int NextShape();
    int PreviousShape();

    // Variables
    vector<Shape*>* scn_shapes;
    bool C_state = false;
    Bezier1D *bezier_1D;
    int first_point_index, last_point_index;

    // Animation
    int cube_shape_index;
    bool forward_direction = true;
    int cube_segment = 0;
    float cube_t = 0;
    float cube_step_size = 0.0001;

    // Picking (With arrows)
    bool S_mode = false;
    int picked_shape_index = -1;
};