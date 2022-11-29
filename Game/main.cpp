#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"

#include "assignment.h"
#include <string>

using namespace std;

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 800;
	const int DISPLAY_HEIGHT = 800;
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;

	Game *scn = new Game(CAMERA_ANGLE,(float)DISPLAY_WIDTH/DISPLAY_HEIGHT,NEAR,FAR);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Init(display);
	
	scn->Init();

	display.SetScene(scn);

	// new code here
	string file_name = "../scenes/scene.txt";

	SceneData scene_data = SceneData();
	scene_data.read_scene(file_name);
    scene_data.find_pixel_size(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	cout << scene_data.FindIntersectionWithSphere(vec3(-1, 0, 0), vec4(0, 0, 0, 1)) << endl;

	while(!display.CloseWindow())
	{
		scn->Draw(1,0,scn->BACK,true,false);
		scn->Motion();
		display.SwapBuffers();
		display.PollEvents();

	}
	delete scn;
	return 0;
}
