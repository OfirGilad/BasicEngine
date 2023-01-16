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

			// If S mode is on - Enable Picking
			if (scn->route_3D_bezier_1D.S_mode) {
				scn->Picking((int)x2, (int)y2);
			}
		}
	}
	
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
		//scn->MyTranslate(glm::vec3(0,0,xoffset),0);

		scn->MouseScrolling(glm::vec3(0, 0, yoffset), 0);
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
					scn->route_3D_bezier_1D.NumberOfSegmentsToDisplay(2);
					break;
				case GLFW_KEY_3: // Display 3 segments cubic Bezier curve, each segment is a cubic Bezier.
					scn->route_3D_bezier_1D.NumberOfSegmentsToDisplay(3);
					break;
				case GLFW_KEY_4: // Display 4 segments cubic Bezier curve, each segment is a cubic Bezier.
					scn->route_3D_bezier_1D.NumberOfSegmentsToDisplay(4);
					break;
				case GLFW_KEY_5: // Display 5 segments cubic Bezier curve, each segment is a cubic Bezier.
					scn->route_3D_bezier_1D.NumberOfSegmentsToDisplay(5);
					break;
				case GLFW_KEY_6: // Display 6 segments cubic Bezier curve, each segment is a cubic Bezier.
					scn->route_3D_bezier_1D.NumberOfSegmentsToDisplay(6);
					break;
				case GLFW_KEY_SPACE: // ‘Space’ will start animation where the cube moves along the Bezier curve forwardand backward.
					if (scn->IsActive()) {
						scn->Deactivate();
					}
					else {
						scn->Activate();
					}
					break;
				case GLFW_KEY_LEFT: // Move to Previous left.
					if (!scn->route_3D_bezier_1D.S_mode) {
						scn->SetPickedShape(scn->route_3D_bezier_1D.PreviousShape());
					}
					break;
				case GLFW_KEY_RIGHT: // Rotate the Scene right.
					if (!scn->route_3D_bezier_1D.S_mode) {
						scn->SetPickedShape(scn->route_3D_bezier_1D.NextShape());
					}
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
				case GLFW_KEY_C: // Continuity state: begins and ends when the user presses ‘C’. In this state the angle created by p_2, p_3 and p_1 the next segment must be conserved.
					if (!scn->route_3D_bezier_1D.C_state) {
						cout << "Continuity state: On" << endl;
						scn->route_3D_bezier_1D.C_state = true;
					}
					else {
						cout << "Continuity state: Off" << endl;
						scn->route_3D_bezier_1D.C_state = false;
					}
					break;
				case GLFW_KEY_S: // Unpicked mode, when 'S' is pressed.In this mode the scene will move by moving the mouse(right button for translationand left button for rotation).
					if (!scn->route_3D_bezier_1D.S_mode) {
						cout << "Unpicked mode: On" << endl;
						cout << "Notice: Scene movement is enabled!" << endl;
						scn->route_3D_bezier_1D.S_mode = true;
					}
					else {
						scn->SetPickedShape(scn->route_3D_bezier_1D.picked_shape_index);
						cout << "Unpicked mode: Off" << endl;
						cout << "Warning: Scene movement is disabled!" << endl;
						scn->route_3D_bezier_1D.S_mode = false;
					}
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
		// When the first control point is translated the cube will follow it (in case the cube is covering the control point)
		if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
		// Rotate a control point that doesn’t locate on the curve (p_1, p_2) around the adjacent control point on the curve (p_0, p_3).
		// If the user presses on control points p_0 with the left mouse button it will cause p_2 of the previous Bezier path, to be on the same line of p_0 and p_1.
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
