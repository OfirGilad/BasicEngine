#include "assignment.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
using namespace Eigen;

void SceneData::read_scene(string file_name) {
    int index = -1;
    string text_line, text_argument;
    vector<string> scene_line;
    ifstream scene_file(file_name);

    cout << "File data:" << endl;

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(scene_file, text_line)) {
        cout << text_line << endl;

        std::stringstream text_line_stream(text_line);

        while (getline(text_line_stream, text_argument, ' ')) {
            if (index == 4) {
                scene_data.push_back(scene_line);
                scene_line.clear();
            }
            scene_line.push_back(text_argument);
            index = (index + 1) % 5;
        } 
    }
    for (int i = 0; i < scene_data.size(); i++) {
        if (scene_data[i][0] == "e") {
            eye = Vector3f(stof(scene_data[i][1]), stof(scene_data[i][2]), stof(scene_data[i][3]));
            bonus_mode_flag = stof(scene_data[i][4]);
        }
        if (scene_data[i][0] == "a") {
            ambient = Vector4f(stof(scene_data[i][1]), stof(scene_data[i][2]), stof(scene_data[i][3]), stof(scene_data[i][4]));
        }
    }    
}