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

    // Variables
    vector<Shape*>* scn_shapes;
    bool C_state = false;
    Bezier1D bezier_1D;
};