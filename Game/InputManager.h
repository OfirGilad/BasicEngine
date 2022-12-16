#pragma once   //maybe should be static class
#include "display.h"
#include "game.h"
#include <iostream>
#include <fstream>


	void mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		if(action == GLFW_PRESS )
		{
			Game *scn = (Game*)glfwGetWindowUserPointer(window);
			double x2,y2;
			glfwGetCursorPos(window,&x2,&y2);
			scn->Picking((int)x2,(int)y2);
		}
	}
	
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
		//scn->MyTranslate(glm::vec3(0,0,xoffset),0);

		scn->MyTranslate(glm::vec3(0, 0, yoffset), 0);
	}
	
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
				case GLFW_KEY_ESCAPE:			
					glfwSetWindowShouldClose(window,GLFW_TRUE);
				break;
				//case GLFW_KEY_SPACE:
				//	if(scn->IsActive())
				//		scn->Deactivate();
				//	else
				//		scn->Activate();
				//break;

				case GLFW_KEY_UP:
					scn->MoveCamera(0,scn->zTranslate,0.4f);
					break;
				case GLFW_KEY_DOWN:
					//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
					//cout<< "down: "<<endl;
					scn->MoveCamera(0,scn->zTranslate,-0.4f);
					break;

				// New Keys
				case GLFW_KEY_R:
					scn->rubiks_cube.CASE_R(scn);
					std::cout << "R Pressed" << std::endl;
					break;
				case GLFW_KEY_L:
					scn->rubiks_cube.CASE_L(scn);
					std::cout << "L Pressed" << std::endl;
					break;
				case GLFW_KEY_U:
					scn->rubiks_cube.CASE_U(scn);
					std::cout << "U Pressed" << std::endl;
					break;
				case GLFW_KEY_D:
					scn->rubiks_cube.CASE_D(scn);
					std::cout << "D Pressed" << std::endl;
					break;
				case GLFW_KEY_B:
					scn->rubiks_cube.CASE_B(scn);
					std::cout << "B Pressed" << std::endl;
					break;
				case GLFW_KEY_F:
					scn->rubiks_cube.CASE_F(scn);
					std::cout << "F Pressed" << std::endl;
					break;
				case GLFW_KEY_SPACE:
					scn->rubiks_cube.CASE_SPACE();
					std::cout << "Space Pressed" << std::endl;
					break;
				case GLFW_KEY_Z:
					scn->rubiks_cube.CASE_Z(scn);
					std::cout << "Z Pressed" << std::endl;
					break;
				case GLFW_KEY_A:
					scn->rubiks_cube.CASE_A(scn);
					std::cout << "A Pressed" << std::endl;
					break;

			default:
				break;
			}
		}
	}
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->UpdatePosition((float)xpos,(float)ypos);
		if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
		else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
		}

	}

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->Resize(width,height);
		
	}

	void Init(Display &display)
	{
		display.AddKeyCallBack(key_callback);
		display.AddMouseCallBacks(mouse_callback,scroll_callback,cursor_position_callback);
		display.AddResizeCallBack(window_size_callback);
	}
