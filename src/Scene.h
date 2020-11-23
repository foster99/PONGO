#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Level.h"
#include "Camera.h"
#include "AssimpModel.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{
public:

	typedef AssimpModel Model;
	typedef int camera;
	static constexpr camera fpsCamera = 0;
	static constexpr camera fixedCamera_01 = 1;
	static constexpr camera fixedCamera_02 = 2;
	static constexpr camera fixedCamera_03 = 3;
	static constexpr camera fixedCamera_04 = 4;
	static constexpr camera fixedCamera_05 = 5;
	static constexpr float mouseSpeed = 0.005;

	Scene();
	~Scene();

	virtual void init();
	virtual void update(int deltaTime);
	virtual void render() = 0;

	// Camera Methods
	bool cameraIsFree();
	void setCamera(camera c);
	void setMouseAngles(int x, int y);

protected:

	float currentTime;

	// Camera
	Camera* cam;
	camera selectedCamera;
	
	// Shaders
	ShaderProgram texProgram;
	void initShaders();

	// Rendering
	glm::mat4 view;
	glm::mat4 projection;

	// 3D
	Level* level;
};


#endif // _SCENE_INCLUDE

