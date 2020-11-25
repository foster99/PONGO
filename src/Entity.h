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

public:

	~Entity();

	Entity();
	Entity(Model* model);

	virtual void init();
	virtual void render();
	virtual void update(int deltaTime) = 0;

	// Given two entities returs if they collided and so, its collission point according to its Bounding Boxes too.
	static std::pair<bool, glm::vec2> collisionPoint(const Entity& e1, const Entity& e2);

};
#endif

