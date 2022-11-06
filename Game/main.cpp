//#include "InputManager.h"
#include "display.h"
#include "game.h"
#include "glm\glm.hpp"

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 512; //1600;
	const int DISPLAY_HEIGHT = 512; //800;
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;

	Game *scn = new Game(CAMERA_ANGLE,(float)DISPLAY_WIDTH/DISPLAY_HEIGHT,NEAR,FAR);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	//Init(display);
	
	scn->Init();
	
	///////////////////////////////////////////
	//glViewport(0, 0, 256, 256);
	//glScissor(0, 0, 256, 256);

	//glViewport(256, 0, 256, 256);
	//glScissor(256, 0, 256, 256);

	//glViewport(256, 256, 256, 256);
	//glScissor(256, 256, 256, 256);

	//glViewport(0, 256, 256, 256);
	//glScissor(0, 256, 256, 256);
	///////////////////////////////////////////

	display.SetScene(scn);

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
