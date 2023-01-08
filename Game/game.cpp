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
		glm::vec4 trans = glm::transpose(GetRotate()) * glm::vec4(delta.x, delta.y, delta.z, 1);
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
		if (button == 1)
		{
			if (route_3D_bezier_1D.OnCurvePoint(pickedShape)) {
				shapes[pickedShape]->MyTranslate(glm::vec3(-GetXrel() / 20.0f, 0, 0), 0);
				shapes[pickedShape]->MyTranslate(glm::vec3(0, GetYrel() / 20.0f, 0), 0);

				if (route_3D_bezier_1D.HasLeft(pickedShape)) {
					shapes[pickedShape - 1]->MyTranslate(glm::vec3(-GetXrel() / 20.0f, 0, 0), 0);
					shapes[pickedShape - 1]->MyTranslate(glm::vec3(0, GetYrel() / 20.0f, 0), 0);
				}
				if (route_3D_bezier_1D.HasRight(pickedShape)) {
					shapes[pickedShape + 1]->MyTranslate(glm::vec3(-GetXrel() / 20.0f, 0, 0), 0);
					shapes[pickedShape + 1]->MyTranslate(glm::vec3(0, GetYrel() / 20.0f, 0), 0);
				}
			}
			else {
				glm::vec4 control_point = shapes[pickedShape]->GetTranslate()[3];
				glm::vec4 center;

				if (pickedShape % 3 == 1) {
					center = shapes[pickedShape - 1]->GetTranslate()[3];
				}
				else {
					center = shapes[pickedShape + 1]->GetTranslate()[3];
				}

				// Move the control point to center of rotation
				vec4 move_to_center = center - control_point;
				shapes[pickedShape]->MyTranslate(glm::vec3(move_to_center.x, move_to_center.y, move_to_center.z), 0);

				// Rotate the control point in the center of rotation
				shapes[pickedShape]->MyRotate(GetXrel() / 20.0f, glm::vec3(0, 0, 1), 0);
				shapes[pickedShape]->MyRotate(GetYrel() / 20.0f, glm::vec3(0, 0, 1), 0);

				// Move the control point to the new position
				vec4 return_from_center = shapes[pickedShape]->GetRotate() * -move_to_center;
				shapes[pickedShape]->MyTranslate(glm::vec3(return_from_center.x, return_from_center.y, return_from_center.z), 0);
			}
			
			route_3D_bezier_1D.UpdateCurveByShapes();
		}
		else
		{
			shapes[pickedShape]->MyRotate(GetXrel() / 2.0f, glm::vec3(0, 1, 0), 0);
			shapes[pickedShape]->MyRotate(GetYrel() / 2.0f, glm::vec3(1, 0, 0), 0);
			WhenRotate();
		}
	}
}
