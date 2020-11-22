#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Level.h"
#include "Camera.h"


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
	bool cameraIsFree();
	void setMouseAngles(int x, int y);

private:
	void initShaders();


public: // CONST EXPR DECLARATIONS

	static constexpr camera fpsCamera	   = 0;
	static constexpr camera fixedCamera_01 = 1;
	static constexpr camera fixedCamera_02 = 2;
	static constexpr camera fixedCamera_03 = 3;
	static constexpr camera fixedCamera_04 = 4;
	static constexpr camera fixedCamera_05 = 5;

	static constexpr float mouseSpeed = 0.005;


private:
	
	camera selectedCamera;
	Camera* cam;
	Level *level;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection, modelview;

};


#endif // _SCENE_INCLUDE

