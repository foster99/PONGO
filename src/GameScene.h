#ifndef _GAMESCENE_INCLUDE
#define _GAMESCENE_INCLUDE

#include "Scene.h"
#include "ParticleSystem.h"

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
	
	static constexpr int nTicks = 10;

	void init() override;
	void render() override;
	void update(int deltaTime) override;

	void restartLevel();
	
	// Initial CountDown
	bool onCountDown(int deltaTime);
	void loadCountDownModels();
	void renderCountDown();
	mat4 countDownViewMatrix();

	// Collisions
	void clearPositionHistories();
	bool ballIsOnHorizontalSlideScope(Slide* slide);
	bool ballIsOnVerticalSlideScope(Slide* slide);
	bool collidingBoundingBoxes(vec4 BB1, vec4 BB2);
	bool ballAndSlideAreColliding(Slide* slide);
	void checkCollision_Ball_Slide();
	bool checkCollision_Ball_World(int nTicks, int deltaTime);
	bool checkCollision_Slide_World(Slide* slide, int nTicks, int deltaTime);
	void checkCollisionsAndUpdateEntitiesPositions(int deltaTime);
	
	// Rope
	bool ballIsOnHorizontalRope();
	bool ballIsOnVerticalRope();
	void blockBallsHorizontalDirection();
	void blockBallsVerticalDirection();

	// Camera
	void updateViewMatrix();
	
	// Transition
	static constexpr int offsetX = 2;
	static constexpr int offsetY = 1;
	bool weAreInTransition();
	void transitionControlUpdate();
	mat4 transitionMatrix();
	
	// Chunk Camera
	int getChunkOfCoords(vec2 coords);
	void updateCurrentChunk();
	mat4 lookAtCurrentChunk();

	// Win Level
	static constexpr int lastLevel = 5;
	void winLevel();
	mat4 winAnimationViewMatrix();
	void loadNextLevel();
	void loadPrevLevel();
	void winGame();
	bool gameEnded();
	bool onWinLevelTime();

	// Ball Control
	bool pressed;
	void initBall();
	void killBall();
	void playerPressedSpace();
	void playerReleasedSpace();
	void setSpawnPoint(vec2 coords);
	void locateBallInSpawnPoint();

	// Getters
	int getLevelID();
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

	// Aux Methods
	// Retorna las coordenadas de la tile en formato (J, I)
	ivec2 toTileCoords(vec2 coords);
	// Retorna las coordenadas de la tile en formato (J, -I)
	ivec2 toTileCoordsNotInverting(vec2 coords);

	// Particles

	//PURE TESTING REMOVE LATER
	void changeSpawnPoint();

private:

	float speedDivisor;
	ShaderProgram* gameShaderProgram;

	// Chunk Camera
	int currentChunk;
	bool transitionR;
	bool transitionL;
	bool transitionD;
	bool transitionU;

	// Initial Countdown
	bool countdownStarted;
	int countdown;

	// Win variables
	bool endOfGame;
	bool win;
	bool teleportSound;
	int wintime;

	// Death Variables
	bool dead;
	int deadtime;

	// Level info
	int levelID;
	Level* level;

	// BALL THINGS
	ParticleSystem* deathParticles;
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

