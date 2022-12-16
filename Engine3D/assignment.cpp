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

    scn_shapes = scn->getShapes();
    //(*scn_shapes)[0]->MyScale(glm::vec3(0.5, 0.5, 0.5));
    //(*scn_shapes)[0]->MyTranslate(glm::vec3(1, 0, 0), 0);
    //(*scn_shapes)[0]->MyRotate(60, glm::vec3(0, 1, 0), 0);

    float distance = float(size) - 0.5f;
    int index = 0;

    for (float i = -distance; i <= distance; i+=2)
    {
        std::vector<std::vector<std::pair<int, vec3>>> sub_struct1;

        for (float j = -distance; j <= distance; j+=2)
        {
            std::vector<std::pair<int, vec3>> sub_struct2;

            for (float k = -distance; k <= distance; k+=2)
            {
                scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
                scn->SetShapeTex(index, 0);
                (*scn_shapes)[index]->MyTranslate(glm::vec3(i, j, k), 0);

                sub_struct2.push_back(std::make_pair(index, vec3(0, 0, 0)));

                index++;
            }
            sub_struct1.push_back(sub_struct2);
        }
        cube_structure.push_back(sub_struct1);
    }
    std::cout << "Check Stucture" << std::endl;
}
