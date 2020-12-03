#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Entity.h"

using namespace std;
using namespace glm;

class GameScene;

class Ball : public Entity
{
private:

	// BALL atributtes

public:

	vec2 spawnPoint;

	~Ball();
	Ball(GameScene* scene, Model* model, ShaderProgram* prog);

	void init() override;
	void render() override;
	void update(int deltaTime) override;

	void setPosition(vec2 position);
	void setSpeed(vec2 speed);
	void setDirection(vec2 direction);

	vec4 getBoundingBox() override;
	vector<ivec2> occupiedTiles() override;
};
#endif