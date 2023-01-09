#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle ,float relationWH, float near1, float far1) : Scene(angle,relationWH,near1,far1)
{ 	
}

void Game::Init()
{		

	AddShader("../res/shaders/pickingShader");	
	AddShader("../res/shaders/basicShader");
	
	//AddTexture("../res/textures/box0.bmp",false);

	//AddShape(Plane,-1,TRIANGLES);
	
	pickedShape = 0;
	
	//SetShapeTex(0,0);
	//MoveCamera(0,zTranslate,10);
	pickedShape = -1;
	
	//ReadPixel(); //uncomment when you are reading from the z-buffer

    // Creating a 3D route with by manipulating 1D Bezier curve
    route_3D_bezier_1D = Route3DBezier1D();
    route_3D_bezier_1D.Create_Route3DBezier1D(this, segNum, res, mode);
}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
    route_3D_bezier_1D.AnimateCubeMovement(isActive);

	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if(isActive)
	{
	}
}

Game::~Game(void)
{
}

// New Functions
void Game::AddBezier1DShape(Shape* bezier_1D_line, int parent)
{
	chainParents.push_back(parent);
	shapes.push_back(bezier_1D_line);
}

void Game::MouseScrolling(glm::vec3 delta, int mode)
{
	if (pickedShape == -1) {
		MyTranslate(delta, mode);
	}
	else {
		glm::mat4 rot_inverse = glm::inverse(GetRotate());
		glm::vec4 trans = rot_inverse * glm::vec4(delta.x, delta.y, delta.z, 1);
		shapes[pickedShape]->MyTranslate(glm::vec3(trans.x, trans.y, trans.z), mode);

		route_3D_bezier_1D.UpdateCurveByShapes();
	}
}

void Game::MouseProccessing(int button)
{
	if (pickedShape == -1)
	{
		if (button == 1)
		{
			MyTranslate(glm::vec3(-GetXrel() / 20.0f, 0, 0), 0);
			MyTranslate(glm::vec3(0, GetYrel() / 20.0f, 0), 0);
			WhenTranslate();
		}
		else
		{
			MyRotate(GetXrel() / 2.0f, glm::vec3(0, 1, 0), 0);
			MyRotate(GetYrel() / 2.0f, glm::vec3(1, 0, 0), 0);

			WhenRotate();
		}
	}
	else {
		glm::mat4 rot_inverse = glm::inverse(GetRotate());

		if (button == 1)
		{
			// If the point is part of the Bezier 1D line
			if (pickedShape != route_3D_bezier_1D.cube_shape_index) {
				if (route_3D_bezier_1D.OnCurvePoint(pickedShape)) {
					// Multiyply buy camera rotation
					glm::vec4 trans_x = rot_inverse * glm::vec4(-GetXrel() / 20.0f, 0, 0, 1);
					glm::vec4 trans_y = rot_inverse * glm::vec4(0, GetYrel() / 20.0f, 0, 1);

					//shapes[pickedShape]->MyTranslate(glm::vec3(-GetXrel() / 20.0f, 0, 0), 0);
					//shapes[pickedShape]->MyTranslate(glm::vec3(0, GetYrel() / 20.0f, 0), 0);

					shapes[pickedShape]->MyTranslate(glm::vec3(trans_x.x, trans_x.y, trans_x.z), 0);
					shapes[pickedShape]->MyTranslate(glm::vec3(trans_y.x, trans_y.y, trans_y.z), 0);

					if (route_3D_bezier_1D.C_state == true) {
						if (route_3D_bezier_1D.HasLeft(pickedShape)) {
							//shapes[pickedShape - 1]->MyTranslate(glm::vec3(-GetXrel() / 20.0f, 0, 0), 0);
							//shapes[pickedShape - 1]->MyTranslate(glm::vec3(0, GetYrel() / 20.0f, 0), 0);

							shapes[pickedShape - 1]->MyTranslate(glm::vec3(trans_x.x, trans_x.y, trans_x.z), 0);
							shapes[pickedShape - 1]->MyTranslate(glm::vec3(trans_y.x, trans_y.y, trans_y.z), 0);
						}
						if (route_3D_bezier_1D.HasRight(pickedShape)) {
							//shapes[pickedShape + 1]->MyTranslate(glm::vec3(-GetXrel() / 20.0f, 0, 0), 0);
							//shapes[pickedShape + 1]->MyTranslate(glm::vec3(0, GetYrel() / 20.0f, 0), 0);

							shapes[pickedShape + 1]->MyTranslate(glm::vec3(trans_x.x, trans_x.y, trans_x.z), 0);
							shapes[pickedShape + 1]->MyTranslate(glm::vec3(trans_y.x, trans_y.y, trans_y.z), 0);
						}
					}
				}
				else {
					if (route_3D_bezier_1D.C_state == true) {
						glm::vec4 center, line_with_center;
						glm::vec4 control_point = shapes[pickedShape]->GetTranslate()[3];
						glm::vec3 movement_direction;

						if (pickedShape % 3 == 1) {
							center = shapes[pickedShape - 1]->GetTranslate()[3];
							line_with_center = control_point - center;
							movement_direction = glm::normalize(glm::vec3(line_with_center.x, line_with_center.y, line_with_center.z));

							shapes[pickedShape]->MyTranslate(float(-GetXrel() / 20.0f) * movement_direction, 0);
							shapes[pickedShape]->MyTranslate(float(GetYrel() / 20.0f) * movement_direction, 0);
						}
						else {
							center = shapes[pickedShape + 1]->GetTranslate()[3];
							line_with_center = control_point - center;
							movement_direction = glm::normalize(glm::vec3(line_with_center.x, line_with_center.y, line_with_center.z));

							shapes[pickedShape]->MyTranslate(float(-GetXrel() / 20.0f) * movement_direction, 0);
							shapes[pickedShape]->MyTranslate(float(GetYrel() / 20.0f) * movement_direction, 0);
						}
					}
					else {
						// Multiyply by camera rotation
						glm::vec4 trans_x = rot_inverse * glm::vec4(-GetXrel() / 20.0f, 0, 0, 1);
						glm::vec4 trans_y = rot_inverse * glm::vec4(0, GetYrel() / 20.0f, 0, 1);

						//shapes[pickedShape]->MyTranslate(glm::vec3(-GetXrel() / 20.0f, 0, 0), 0);
						//shapes[pickedShape]->MyTranslate(glm::vec3(0, GetYrel() / 20.0f, 0), 0);

						shapes[pickedShape]->MyTranslate(glm::vec3(trans_x.x, trans_x.y, trans_x.z), 0);
						shapes[pickedShape]->MyTranslate(glm::vec3(trans_y.x, trans_y.y, trans_y.z), 0);
					}
				}
				route_3D_bezier_1D.UpdateCurveByShapes();
			}
			// If the cube was selected - Default case
			// If the Bezier curve was selected - Do nothing
			else if (pickedShape == route_3D_bezier_1D.cube_shape_index) {
				// Multiyply by camera rotation
				glm::vec4 trans_x = rot_inverse * glm::vec4(-GetXrel() / 20.0f, 0, 0, 1);
				glm::vec4 trans_y = rot_inverse * glm::vec4(0, GetYrel() / 20.0f, 0, 1);

				//shapes[pickedShape]->MyTranslate(glm::vec3(-GetXrel() / 20.0f, 0, 0), 0);
				//shapes[pickedShape]->MyTranslate(glm::vec3(0, GetYrel() / 20.0f, 0), 0);

				shapes[pickedShape]->MyTranslate(glm::vec3(trans_x.x, trans_x.y, trans_x.z), 0);
				shapes[pickedShape]->MyTranslate(glm::vec3(trans_y.x, trans_y.y, trans_y.z), 0);
			}
		}
		else {
			// If the point is part of the Bezier 1D line
			if (pickedShape < route_3D_bezier_1D.cube_shape_index) {
				if (route_3D_bezier_1D.OnCurvePoint(pickedShape)) {
					if (route_3D_bezier_1D.HasLeft(pickedShape) && route_3D_bezier_1D.HasRight(pickedShape)) {
						glm::vec4 center = shapes[pickedShape]->GetTranslate()[3];
						glm::vec4 left = shapes[pickedShape - 1]->GetTranslate()[3];
						glm::vec4 right = shapes[pickedShape + 1]->GetTranslate()[3];

						glm::vec4 move_to_center = center - left;
						glm::vec4 return_from_center = center - right;

						// Calculate the proportion between center-left and center-right
						float distance_control_left = sqrt(pow(move_to_center.x, 2) + pow(move_to_center.y, 2) + pow(move_to_center.z, 2));
						float distance_control_right = sqrt(pow(return_from_center.x, 2) + pow(return_from_center.y, 2) + pow(return_from_center.z, 2));
						float multiply_factor = distance_control_left / distance_control_right;

						// Move the left point to the control point to the center
						shapes[pickedShape - 1]->MyTranslate(glm::vec3(move_to_center.x, move_to_center.y, move_to_center.z), 0);

						// Move the left point to the new position (on the same line with the right point)
						shapes[pickedShape - 1]->MyTranslate(multiply_factor * glm::vec3(return_from_center.x, return_from_center.y, return_from_center.z), 0);
					}
				}
				else {
					int xrel = GetXrel();
					int yrel = GetYrel();
					glm::vec4 control_point = shapes[pickedShape]->GetTranslate()[3];
					glm::vec4 center;
					int c_state_second_control = 0;

					if (pickedShape % 3 == 1) {
						center = shapes[pickedShape - 1]->GetTranslate()[3];
						c_state_second_control = -2;
					}
					else {
						center = shapes[pickedShape + 1]->GetTranslate()[3];
						c_state_second_control = 2;
					}

					// Move the control point to center of rotation
					vec4 move_to_center = center - control_point;
					shapes[pickedShape]->MyTranslate(glm::vec3(move_to_center.x, move_to_center.y, move_to_center.z), 0);

					glm::vec4 rot = rot_inverse * glm::vec4(0, 0, 1, 0);

					//shapes[pickedShape]->MyRotate(xrel / 2.0f, glm::vec3(0, 0, 1), 0);
					//shapes[pickedShape]->MyRotate(yrel / 2.0f, glm::vec3(0, 0, 1), 0);
					
					// Rotate the control point in the center of rotation
					shapes[pickedShape]->MyRotate(xrel / 2.0f, glm::vec3(rot.x, rot.y, rot.z), 0);
					shapes[pickedShape]->MyRotate(yrel / 2.0f, glm::vec3(rot.x, rot.y, rot.z), 0);

					// Move the control point to the new position
					vec4 return_from_center = shapes[pickedShape]->GetRotate() * -move_to_center;
					shapes[pickedShape]->MyTranslate(glm::vec3(return_from_center.x, return_from_center.y, return_from_center.z), 0);

					//shapes[pickedShape]->MyRotate(-xrel / 2.0f, glm::vec3(0, 0, 1), 0);
					//shapes[pickedShape]->MyRotate(-yrel / 2.0f, glm::vec3(0, 0, 1), 0);

					// Reset rotation
					shapes[pickedShape]->MyRotate(-xrel / 2.0f, glm::vec3(rot.x, rot.y, rot.z), 0);
					shapes[pickedShape]->MyRotate(-yrel / 2.0f, glm::vec3(rot.x, rot.y, rot.z), 0);

					if (route_3D_bezier_1D.C_state == true) {
						int second_control_index = pickedShape + c_state_second_control;

						if (second_control_index > route_3D_bezier_1D.first_point_index && second_control_index < route_3D_bezier_1D.last_point_index) {
							control_point = shapes[second_control_index]->GetTranslate()[3];
							move_to_center = center - control_point;
							shapes[second_control_index]->MyTranslate(glm::vec3(move_to_center.x, move_to_center.y, move_to_center.z), 0);

							//shapes[second_control_index]->MyRotate(xrel / 2.0f, glm::vec3(0, 0, 1), 0);
							//shapes[second_control_index]->MyRotate(yrel / 2.0f, glm::vec3(0, 0, 1), 0);

							// Rotate the control point in the center of rotation
							shapes[second_control_index]->MyRotate(xrel / 2.0f, glm::vec3(rot.x, rot.y, rot.z), 0);
							shapes[second_control_index]->MyRotate(yrel / 2.0f, glm::vec3(rot.x, rot.y, rot.z), 0);

							// Move the control point to the new position
							return_from_center = shapes[second_control_index]->GetRotate() * -move_to_center;
							shapes[second_control_index]->MyTranslate(glm::vec3(return_from_center.x, return_from_center.y, return_from_center.z), 0);

							//shapes[second_control_index]->MyRotate(-xrel / 2.0f, glm::vec3(0, 0, 1), 0);
							//shapes[second_control_index]->MyRotate(-yrel / 2.0f, glm::vec3(0, 0, 1), 0);

							// Reset rotation
							shapes[second_control_index]->MyRotate(-xrel / 2.0f, glm::vec3(rot.x, rot.y, rot.z), 0);
							shapes[second_control_index]->MyRotate(-yrel / 2.0f, glm::vec3(rot.x, rot.y, rot.z), 0);
						}
					}
				}

				route_3D_bezier_1D.UpdateCurveByShapes();
			}
			// If the cube was selected - Default case
			// If the Bezier curve was selected - Do nothing
			else if (pickedShape == route_3D_bezier_1D.cube_shape_index) {
				glm::vec4 rot_x = rot_inverse * glm::vec4(0, 1, 0, 0);
				glm::vec4 rot_y = rot_inverse * glm::vec4(1, 0, 0, 0);

				//shapes[pickedShape]->MyRotate(GetXrel() / 2.0f, glm::vec3(0, 1, 0), 0);
				//shapes[pickedShape]->MyRotate(GetYrel() / 2.0f, glm::vec3(1, 0, 0), 0);

				shapes[pickedShape]->MyRotate(GetXrel() / 2.0f, glm::vec3(rot_x.x, rot_x.y, rot_x.z), 0);
				shapes[pickedShape]->MyRotate(GetYrel() / 2.0f, glm::vec3(rot_y.x, rot_y.y, rot_y.z), 0);
			}
		}
	}
}
