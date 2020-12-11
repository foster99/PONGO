#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Entity.h"
#include "Billboard.h"
#include "ParticleSystem.h"

using namespace std;
using namespace glm;

class GameScene;
struct contourPointList;
struct contourTileList;

class Ball : public Entity
{
private:

	// BALL atributtes
	float currentTime;
	vec2 spawnPoint;

	ParticleSystem* particles;
	Billboard* billboard;

	bool verticalDirBlocked;
	bool horizontalDirBlocked;

public:


	~Ball();
	Ball(GameScene* scene, Model* model, ShaderProgram* prog);

	void init() override;
	void render() override;
	void update(int deltaTime) override;

	void setPosition(vec2 position, int time = -1) override;
	void setSpeed(vec2 speed);
	void setDirection(vec2 direction);
	void setSpawnPoint(vec2 coords);
	void displacePosition(vec2 displacement);
	void locateInSpawnPoint();
	void spawnParticles();

	bool isOnDeathTiles();

	bool horizontalIsBlocked();
	bool verticalIsBlocked();
	void blockHorizontalDirection();
	void blockVerticalDirection();
	void restartDirectionBlocks();

	contourPointList listOfContourPoints();
	contourTileList listOfContourTiles();

	vec4 getBoundingBox() override;
	vector<vector<ivec2>> occupiedTiles() override;
};
#endif