#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include <glm/glm.hpp>
#include <utility>
#include "AssimpModel.h"
using namespace std;
using namespace glm;

typedef AssimpModel Model;

class Entity
{
protected:

	Model*			model	= nullptr;
	ShaderProgram*	program = nullptr;
	mat4 viewMatrix;
	mat4 projection;

public:

	~Entity();

	Entity();
	Entity(Model* model, ShaderProgram* prog);

	virtual void init();
	virtual void render();
	virtual void update(int deltaTime) = 0;

	void setViewMatrix(mat4 view);
	void setProjMatrix(mat4 proj);
	// Given two entities returs if they collided and so, its collission point according to its Bounding Boxes too.
	static std::pair<bool, glm::vec2> collisionPoint(const Entity& e1, const Entity& e2);

};
#endif

