#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../res/includes/glm/glm.hpp"
#include "../Engine3D/scene.h"

using namespace std;
using namespace glm;

class Route3DBezier1D
{
public:
    // Methods
    Route3DBezier1D();
    void Create_Route3DBezier1D(Scene* scn, int segNum,int res,int mode, int viewport);

    // Variables
    vector<Shape*>* scn_shapes;
};