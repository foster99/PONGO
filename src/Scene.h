#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Level.h"


#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

typedef int camera;

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

	void setCamera(camera c);
	void updateCamera();

private:
	void initShaders();


public: // CONST EXPR DECLARATIONS

	static constexpr camera defaultCamera  = 0;
	static constexpr camera fpsCamera	   = 1;
	static constexpr camera fixedCamera_02 = 2;
	static constexpr camera fixedCamera_03 = 3;
	static constexpr camera fixedCamera_04 = 4;
	static constexpr camera fixedCamera_05 = 5;



private:
	
	camera selectedCamera;
	
	Level *level;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection, modelview;

};


#endif // _SCENE_INCLUDE

