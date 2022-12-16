#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "../res/includes/glm/glm.hpp"
#include "../Game/game.h"

using namespace std;
using namespace glm;

class RubiksCube
{
public:
    // Methods
    RubiksCube(Game* scn, int size);

    // Variables
    int size;
    std::vector<Shape*>* scn_shapes;
    std::vector < std::vector<std::vector<std::pair<int, vec3>>>> cube_structure;
};