#pragma once
#include "scene.h"
#include "assignment.h"

class Game : public Scene
{
public:
	
	Game();
	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);

	// New Functions
	std::vector<Shape*>* GetShapes() override { return &shapes; }
	void AddBezier1DShape(Shape* bezier_1D_line, int parent) override;
	void MouseProccessing(int button) override;
	void MouseScrolling(glm::vec3 delta, int mode) override;

	// New Variables
    int segNum;
    int res;
    int mode;
    int viewport;
    Route3DBezier1D route_3D_bezier_1D;
};

