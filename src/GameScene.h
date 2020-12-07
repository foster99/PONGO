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

	void restartLevel(int levelID);
	
	ivec2 toTileCoords(vec2 coords);
	ivec2 toTileCoordsNotInverting(vec2 coords);

	bool onCountDown(int deltaTime);
	void loadCountDownModels();
	void renderCountDown();

	void clearPositionHistories();
	void killBall();
	bool ballIsOnHorizontalSlideScope(Slide* slide);
	bool ballIsOnVerticalSlideScope(Slide* slide);
	bool collidingBoundingBoxes(vec4 BB1, vec4 BB2);
	bool ballAndSlideAreColliding(Slide* slide);
	void checkCollision_Ball_Slide();
	bool checkCollision_Ball_World(int time);
	bool checkCollision_Slide_World(Slide* slide, int time);

	void checkCollisionsAndUpdateEntitiesPositions(int deltaTime);
	
	int getChunkOfCoords(vec2 coords);
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
	vec4 getPlayerBBox();
	int getPlayerChunk();

	void setSpawnPoint(vec2 coords);
	void locateBallInSpawnPoint();

private:

	int countdown;
	int currentChunk;
	bool dead;

	int levelID;
	Level* level;
	ShaderProgram* gameShaderProgram;

	// BALL THINGS
	ShaderProgram*	ballShader;
	Model*			ballModel;
	Ball*			ball;

	// NUMBER THINGS
	ShaderProgram* numbersShader;
	Model* model_1;
	Model* model_2;
	Model* model_3;

};
#endif

