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

	float oldPositionX;
	float oldPositionY;

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

	virtual vector<vector<ivec2>> occupiedTiles()  = 0;
	virtual vec4		  getBoundingBox() = 0;	// vec4 -> xmin, xmax, ymin, ymax

	vec2 getPosition();
	vec2 getSpeed();
	vec2 getDirection();
	vec2 getOldPosition();

	virtual void setPosition(vec2 position);
	virtual void setPositionX(float new_x);
	virtual void setPositionY(float y);
	virtual void setSpeed(vec2 speed);
	virtual void setDirection(vec2 direction);
	void invertDirectionX();
	void invertDirectionY();

	void rollbackPosition();
	void rollbackPositionX();
	void rollbackPositionY();

	void setViewMatrix(mat4 view);
	void setProjMatrix(mat4 proj);

	// Given two entities returs if they collided and so, its collission point according to its Bounding Boxes too.
	static std::pair<bool, glm::vec2> collisionPoint(const Entity& e1, const Entity& e2);

};
#endif

