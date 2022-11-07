//#include "InputManager.h"
#include "display.h"
#include "game.h"
#include "glm\glm.hpp"
#include <stb_image.h>
#include "assignment.h"

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


	// lena as is
	int width, height, numComponents;
	unsigned char* data = stbi_load("../lena256.jpg", &width, &height, &numComponents, 4);
	
	// Canny Edge Detection
	unsigned char* data1 = Canny_Edge_Detector(data, &width, &height);
	
	// Halftone
	int width2 = width;
	int height2 = height;
	unsigned char* data2 = halftone(data, &width2, &height2);
	
	// Floyd-Steinberg Algorithm
	unsigned char* data3 = Floyd_Steinberg_Algorithm(data, &width, &height);


	scn->AddTexture(width2, height2, data2);
	scn->AddTexture(width, height, data3);
	scn->AddTexture(width, height, data1);
	scn->AddTexture(width, height, data);

	
	scn->Init();

	display.SetScene(scn);
	

	while(!display.CloseWindow())
	{
		scn->SetShapeTex(0, 0);
		scn->Draw(1,0,scn->BACK,false,false);
		scn->SetShapeTex(0, 1);
		scn->Draw(1, 0, scn->BACK, false, true);
		scn->SetShapeTex(0, 2);
		scn->Draw(1, 0, scn->BACK, true, false);
		scn->SetShapeTex(0, 3);
		scn->Draw(1, 0, scn->BACK, true, true);
		scn->Motion();
		display.SwapBuffers();
		display.PollEvents();		
	}
	delete scn;
	return 0;
}
