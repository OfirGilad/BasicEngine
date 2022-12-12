#include "assignment.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "../Game/game.h"
#include "scene.h"

using namespace std;
using namespace glm;

RubiksCube::RubiksCube(Game* scn, int size) {
    this->size = size;

    scn->AddTexture("../res/textures/plane.png", false);
    scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
    scn->SetShapeTex(0, 0); 

    scn_shapes = scn->getShapes();
    //(*scn_shapes)[0]->MyScale(glm::vec3(0.5, 0.5, 0.5));
    //(*scn_shapes)[0]->MyTranslate(glm::vec3(0, 0, 1), 1);
}
