#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Level.h"
#include "Camera.h"
#include "AssimpModel.h"

using namespace std;
using namespace glm;

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define PI 3.14159f


class Scene
{
public:

	typedef AssimpModel Model;
	typedef int camera;
	static constexpr camera fpsCamera      = 0;
	static constexpr camera fixedCamera_01 = 1;
	static constexpr camera fixedCamera_02 = 2;
	static constexpr camera fixedCamera_03 = 3;
	static constexpr camera fixedCamera_04 = 4;
	static constexpr camera fixedCamera_05 = 5;
	static constexpr float  mouseSpeed     = 0.005f;
	


	Scene();
	~Scene();

	virtual void init();
	virtual void update(int deltaTime);
	virtual void render() = 0;

	// Camera Methods
	bool cameraIsFree();
	int getCameraNumber();
	void setCamera(camera c);
	void setMouseAngles(int x, int y);

	static void loadShaders(const string& filename, ShaderProgram* program);
	static void displayText(vec2 position, vec3 color, string text);

	int getCurrentTime();
	mat4 getViewMatrix();
	mat4 getProjMatrix();

protected:

	float currentTime;

	// Camera
	Camera* cam;
	camera selectedCamera;
	
	// Shaders
	ShaderProgram* defaultShaderProgram;
	void initDefaultShaders();

	// Rendering
	mat4 view;
	mat4 projection;

	// 3D
	World* world;
	Model* axis;
	void renderAxis();
};


#endif // _SCENE_INCLUDE

