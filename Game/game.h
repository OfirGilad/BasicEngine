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

    int segNum;
    int res;
    int mode;
    int viewport;
    Route3DBezier1D route_3D_bezier_1D;
};

