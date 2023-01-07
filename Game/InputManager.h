#pragma once   //maybe should be static class
#include "display.h"
#include "game.h"


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
				//
				//case GLFW_KEY_UP:
				//	scn->MoveCamera(0,scn->zTranslate,0.4f);
				//	break;
				//case GLFW_KEY_DOWN:
				//	//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
				//	//cout<< "down: "<<endl;
				//	scn->MoveCamera(0,scn->zTranslate,-0.4f);
				//	break;
				
				// New Callbacks
				case GLFW_KEY_2: // Display 2 segments cubic Bezier curve, each segment is a cubic Bezier.
					break;
				case GLFW_KEY_3: // Display 3 segments cubic Bezier curve, each segment is a cubic Bezier.
					break;
				case GLFW_KEY_4: // Display 4 segments cubic Bezier curve, each segment is a cubic Bezier.
					break;
				case GLFW_KEY_5: // Display 5 segments cubic Bezier curve, each segment is a cubic Bezier.
					break;
				case GLFW_KEY_6: // Display 6 segments cubic Bezier curve, each segment is a cubic Bezier.
					break;
				case GLFW_KEY_SPACE: // ‘Space’ will start animation where the cube moves along the Bezier curve forwardand backward.
					if(scn->IsActive())
						scn->Deactivate();
					else
						scn->Activate();
					break;
				case GLFW_KEY_LEFT: // Rotate the Scene left.
					scn->MyRotate(2.f, glm::vec3(0, 1, 0), 0);
					break;
				case GLFW_KEY_RIGHT: // Rotate the Scene right.
					scn->MyRotate(-2.f, glm::vec3(0, 1, 0), 0);
					break;
				case GLFW_KEY_UP: // Rotate the Scene up.
					scn->MyRotate(2.f, glm::vec3(1, 0, 0), 0);
					break;
				case GLFW_KEY_DOWN: // Rotate the Scene down.
					scn->MyRotate(-2.f, glm::vec3(1, 0, 0), 0);
					break;
				case GLFW_KEY_R: // 'R' moves the camera to the right.
					scn->MoveCamera(0, scn->xTranslate, 0.4f);
					break;
				case GLFW_KEY_L: // 'L' moves the camera to the left.
					scn->MoveCamera(0, scn->xTranslate, -0.4f);
					break;
				case GLFW_KEY_U: // 'U' moves the camera upward.
					scn->MoveCamera(0, scn->yTranslate, 0.4f);
					break;
				case GLFW_KEY_D: // 'D' moves the camera downward
					scn->MoveCamera(0, scn->yTranslate, -0.4f);
					break;
				case GLFW_KEY_B: // 'B' moves the camera backward.
					scn->MoveCamera(0, scn->zTranslate, 0.4f);
					break;
				case GLFW_KEY_F: // 'F' moves the camera forward.
					scn->MoveCamera(0, scn->zTranslate, -0.4f);
					break;
				case GLFW_KEY_C: // Continuity state: begins and ends when the user presses ‘C’.
					if (!scn->route_3D_bezier_1D.C_state)
						scn->route_3D_bezier_1D.C_state = true;
					else
						scn->route_3D_bezier_1D.C_state = false;
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

		// Translate a control point inside the camera plane
		// When translating p_0 or p_3 the adjacent p_1 and p_2 will move as well. The curve will change appropriately.
		if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
		// Rotate a control point that doesn’t locate on the curve (p_1, p_2) around the adjacent control point on the curve (p_0, p_3).
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
