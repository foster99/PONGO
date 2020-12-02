#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include <glm/glm.hpp>
#include <utility>
#include "AssimpModel.h"
using namespace std;
using namespace glm;

class GameScene;
typedef AssimpModel Model;

class Entity
{
protected:

	vec2 position;
	vec2 speed;
	vec2 direction;

	Model*			model	= nullptr;
	ShaderProgram*	program = nullptr;
	GameScene*		scene	= nullptr;

	mat4 viewMatrix;
	mat4 projection;

public:

	~Entity();

	Entity();
	Entity(GameScene* scene, Model* model, ShaderProgram* prog);

	virtual void init();
	virtual void render();
	virtual void update(int deltaTime) = 0;

	vec2 getPosition();
	vec2 getSpeed();
	vec2 getDirection();

	virtual void setPosition(vec2 position);
	virtual void setSpeed(vec2 speed);
	virtual void setDirection(vec2 direction);

	void setViewMatrix(mat4 view);
	void setProjMatrix(mat4 proj);

	// Given two entities returs if they collided and so, its collission point according to its Bounding Boxes too.
	static std::pair<bool, glm::vec2> collisionPoint(const Entity& e1, const Entity& e2);

};
#endif

