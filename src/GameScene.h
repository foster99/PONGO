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

	void clearPositionHistories();

	bool ballIsOnHorizontalSlideScope(Slide* slide);
	bool ballIsOnVerticalSlideScope(Slide* slide);
	bool collidingBoundingBoxes(vec4 BB1, vec4 BB2);
	bool ballAndSlideAreColliding(Slide* slide);
	bool checkCollision_Ball_World(int time);
	bool checkCollision_Slide_World(Slide* slide, int time);

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
	vec3 getCameraPosition();
	mat4 getViewMatrix();
	mat4 getProjMatrix();
	vec3 getCameraChunkPosition();

private:

	int currentChunk;

	ShaderProgram* gameShaderProgram;
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

