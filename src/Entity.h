#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include <glm/glm.hpp>
#include <utility>
#include <deque>
#include "AssimpModel.h"
using namespace std;
using namespace glm;

class GameScene;
typedef AssimpModel Model;

struct oldPosition
{
	int tick;
	int time;
	vec2 position;

	oldPosition(vec2 p, int tik, int tim)
	{
		tick = tik;
		time = tim;
		position = p;
	}
};

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
	mat4 projMatrix;

	deque<oldPosition> positionHistory;

	int currentTime;

public:

	~Entity();

	Entity();
	Entity(GameScene* scene, Model* model, ShaderProgram* prog);

	virtual void init();
	virtual void render();
	virtual void update(int deltaTime);

	virtual vector<vector<ivec2>> occupiedTiles()  = 0;
	virtual vec4 getBoundingBox() = 0;	// vec4 -> xmin, xmax, ymin, ymax

	vec2 getPosition();
	vec2 getSpeed();
	vec2 getDirection();
	vec2 getPreviousPosition();

	int getPreviousTick();
	int getPreviousTime();
	int getCurrentChunk();

	virtual void setPosition(vec2 position, int time = -1);
	virtual void setSpeed(vec2 speed);
	virtual void setDirection(vec2 direction);

	void invertDirectionX();
	void invertDirectionY();

	void clearHistory();
	bool rollbackPosition();

	void setViewMatrix(const mat4& view);
	void setProjMatrix(const mat4& proj);
};
#endif

