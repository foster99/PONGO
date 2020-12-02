#ifndef _GAMESCENE_INCLUDE
#define _GAMESCENE_INCLUDE

#include "Scene.h"

class GameScene : public Scene
{
public:
	
	void init() override;
	void render() override;
	void update(int deltaTime) override;

	void checkCollisionsAndUpdateEntitiesPositions();

	mat4 lookAtCurrentChunk();
	void addCube();

	void initBall();

	Level* getLevel();

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

