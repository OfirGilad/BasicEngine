#pragma once
#include "../res/includes/glm/glm.hpp"

class MovableGLM 
{
public:
	MovableGLM();
	glm::mat4 MakeTrans(const glm::mat4 &prevTransformations) const;
	glm::mat4 MakeTrans() const;
	void MyTranslate(glm::vec3 delta, int mode);
	void MyRotate(float angle, const glm::vec3 &vec, int mode);
	void MyScale(glm::vec3 scale);
	void ZeroTrans();

	// New Functions
	glm::mat4 GetTranslate() { return trans; }
	glm::mat4 GetRotate() { return rot; }
	glm::mat4 GetScale() { return scl; }

	void SetTranslate(glm::mat4 trans) { this->trans = trans; }
	void SetRotate(glm::mat4 rot) { this->rot = rot; }
	void SetScale(glm::mat4 scl) { this->scl = scl; }
private:
	glm::mat4 trans;
	glm::mat4 rot;
	glm::mat4 scl;
};