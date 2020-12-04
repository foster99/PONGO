#ifndef _GAMESCENE_INCLUDE
#define _GAMESCENE_INCLUDE

#include "Scene.h"

struct contourPointList
{
	vector<vec2> up;
	vector<vec2> down;
	vector<vec2> right;
	vector<vec2> left;
};

struct contourTileList
{
	vector<Tile*> up;
	vector<Tile*> down;
	vector<Tile*> right;
	vector<Tile*> left;
};


class GameScene : public Scene
{
public:
	
	void init() override;
	void render() override;
	void update(int deltaTime) override;

	ivec2 toTileCoords(vec2 coords);
	ivec2 toTileCoordsNotInverting(vec2 coords);
	void checkCollisionsAndUpdateEntitiesPositions(int deltaTime);

	void updateCurrentChunk();
	mat4 lookAtCurrentChunk();

	void addCube();

	void initBall();
	void playerPressedSpace();

	Level* getLevel();
	vec2 getPlayerPos();
	vec2 getPlayerSpd();
	vec2 getPlayerDir();

private:

	int currentChunk;

	ShaderProgram* gameShaderProgram;
	vector<Model*> models;
	Level* level;

	// FOWOSTER TESTING
	ShaderProgram*	ballShader;
	Model*			ballModel;
	Ball*			ball;

	// TOMEU TESTING
	Slide* ent;
	Model* auxMod;
};
#endif

