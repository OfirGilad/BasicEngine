#include "scene.h"
#include "glad/include/glad/glad.h"
#include <iostream>

	static void printMat(const glm::mat4 mat)
	{
		printf(" matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", mat[j][i]);
			printf("\n");
		}
	}

	Scene::Scene()
	{
		//
		glLineWidth(5);
	
		cameras.push_back(new Camera(60.0f,1.0,0.1f,100.0f));		
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
	}

	Scene::Scene(float angle,float relationWH,float near1, float far1)
	{
		//glLineWidth(5);
		cameras.push_back(new Camera(angle,relationWH,near1,far1));
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
	}

	void Scene::AddShapeFromFile(const std::string& fileName,int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(fileName,mode));
	}

	void Scene::AddShape(int type, int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(type,mode));
	}

	void Scene::AddShapeCopy(int indx,int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(*shapes[indx],mode));
	}

	void Scene::AddShader(const std::string& fileName)
	{
		shaders.push_back(new Shader(fileName));
	}

	void Scene::AddTexture(const std::string& textureFileName,bool for2D)
	{
		textures.push_back(new Texture(textureFileName));
	}

	void Scene::AddTexture(int width,int height, unsigned char *data)
	{
		textures.push_back(new Texture(width,height,data));
	}

	void Scene::AddCamera(glm::vec3& pos , float fov,float relationWH , float zNear, float zFar)
	{
		cameras.push_back(new Camera(fov,relationWH,zNear,zFar));
		cameras.back()->MyTranslate(pos,0);
	}

	void Scene::Draw(int shaderIndx,int cameraIndx,int buffer,bool toClear,bool debugMode)
	{
		glEnable(GL_DEPTH_TEST);
		glm::mat4 Normal = MakeTrans();
	
		glm::mat4 MVP = cameras[cameraIndx]->GetViewProjection()*glm::inverse(cameras[cameraIndx]->MakeTrans());
		int p = pickedShape;
		if(toClear)
		{
			if(shaderIndx>0)
				Clear(1,1,1,1);
			else
				Clear(0,0,0,0);
		}

		for (unsigned int i=0; i<shapes.size();i++)
		{
			if(shapes[i]->Is2Render())
			{
				glm::mat4 Model = Normal * shapes[i]->MakeTrans();
				
				if(shaderIndx > 0)
				{
					Update(MVP,Model,shapes[i]->GetShader());
					shapes[i]->Draw(shaders,textures,false);	
				}
				else 
				{ //picking
					Update(MVP,Model,0);
					shapes[i]->Draw(shaders,textures,true);
				}
			}
		}
		pickedShape = p;
	}

	void Scene::MoveCamera(int cameraIndx,int type,float amt)
	{
		switch (type)
		{
			case xTranslate:
				cameras[cameraIndx]->MyTranslate(glm::vec3(amt,0,0),0);
			break;
			case yTranslate:
				cameras[cameraIndx]->MyTranslate(glm::vec3(0,amt,0),0);
			break;
			case zTranslate:
				cameras[cameraIndx]->MyTranslate(glm::vec3(0,0,amt),0);
			break;
			case xRotate:
				cameras[cameraIndx]->MyRotate(amt,glm::vec3(1,0,0),0);
			break;
			case yRotate:
				cameras[cameraIndx]->MyRotate(amt,glm::vec3(0,1,0),0);
			break;
			case zRotate:
				cameras[cameraIndx]->MyRotate(amt,glm::vec3(0,0,1),0);
			break;
			default:
				break;
		}
	}

	void Scene::ShapeTransformation(int type,float amt)
	{
		if(glm::abs(amt)>1e-5)
		{
			switch (type)
			{
			case xTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(amt,0,0),0);
			break;
			case yTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(0,amt,0),0);
			break;
			case zTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(0,0,amt),0);
			break;
			case xRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(1,0,0),0);
			break;
			case yRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(0,1,0),0);
			break;
			case zRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(0,0,1),0);
			break;
			default:
				break;
			}
		}

	}

	void Scene::Resize(int width,int height)
	{
		
		cameras[0]->SetProjection(cameras[0]->GetAngle(),(float)width/height);
		glViewport(0,0,width,height);
		std::cout<<cameras[0]->GetRelationWH()<<std::endl;

		// Updating width and height
		this->width = width;
		this->height = height;
	}

	float Scene::Picking(int x,int y)
	{
		//pickedShape = 1;
		float depth;
		unsigned char data[]{ 0, 0, 0, 0 };
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glGetIntegerv(GL_VIEWPORT, 0);
		glReadPixels(x, this->height - y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		//glReadPixels(x, this->height - y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//int id = data[0] | data[1] << 8 | data[2] << 16;

		//std::cout << "CLICK" << std::endl;
		//std::cout << float(x)/400.f - 1.f << std::endl;
		//std::cout << (float(y)/400.f - 1.f) * -1.f << std::endl;
		//std::cout << 50 << std::endl;
		//std::cout << std::endl;

		glm::vec3 eye = glm::vec3(0, 0, 50);
		glm::vec3 hit_on_screen = glm::vec3((float(x) / 400.f - 1.f), ((float(y) / 400.f - 1.f) * -1.f), 0);
		glm::vec3 ray = glm::normalize(hit_on_screen - eye);

		//std::cout << ray.x << std::endl;
		//std::cout << ray.y << std::endl;
		//std::cout << ray.z << std::endl;
		//std::cout << std::endl;

		for (int i = 0; i < shapes.size(); i++) {
			glm::mat4 new_trans = GetRotate() * shapes[i]->MakeTrans();
			glm::vec3 center = glm::vec3(new_trans[3].x, new_trans[3].y, new_trans[3].z);
			//std::cout << center.x << std::endl;
			//std::cout << center.y << std::endl;
			//std::cout << center.z << std::endl;
			//std::cout << std::endl;
			
			//float dist = abs((center - hit_on_screen).x) + abs((center - hit_on_screen).y) + abs((center - hit_on_screen).z);
			glm::vec3 screen_to_object = glm::normalize(center - hit_on_screen);

			//std::cout << screen_to_object.x << std::endl;
			//std::cout << screen_to_object.y << std::endl;
			//std::cout << screen_to_object.z << std::endl;
			//std::cout << std::endl;
		}
		//std::cout << std::endl;

		return 0;
	}
	//return coordinates in global system for a tip of arm position is local system 
	void Scene::MouseProccessing(int button)
	{
		if (pickedShape == -1)
		{
			if (button == 1)
			{				

				MyTranslate(glm::vec3(-xrel/20.0f,0,0),0);
				MyTranslate(glm::vec3(0,yrel/20.0f,0),0);
				WhenTranslate();
			}
			else
			{
				//MyRotate(xrel/2.0f,glm::vec3(1,0,0),0);
				//MyRotate(yrel/2.0f,glm::vec3(0,0,1),0);

				MyRotate(xrel / 2.0f, glm::vec3(0, 1, 0), 0);
				MyRotate(yrel / 2.0f, glm::vec3(1, 0, 0), 0);

				WhenRotate();
			}
		}

	}

	void Scene::ZeroShapesTrans()
	{
		for (unsigned int i = 0; i < shapes.size(); i++)
		{
			shapes[i]->ZeroTrans();
		}
	}

	void Scene::ReadPixel()
	{
		glReadPixels(1,1,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
	}

	void Scene::UpdatePosition(float xpos, float ypos)
	{
		xrel = xold - xpos;
		yrel = yold - ypos;
		xold = xpos;
		yold = ypos;
	}

	void Scene::HideShape(int shpIndx)
	{
			shapes[shpIndx]->Hide();
	}

	void Scene::UnhideShape(int shpIndx)
	{
			
			shapes[shpIndx]->Unhide();
	}

	void Scene::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	Scene::~Scene(void)
{
	for (Shape* shp : shapes)
		{
			delete shp;
		}
	for (Camera* cam : cameras)
		{
			delete cam;
		}
	for(Shader* sdr: shaders)
		{
			delete sdr;
		}
	for(Texture* tex: textures)
		{
			delete tex;
		}

} 
	