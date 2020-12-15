#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
using namespace std;
using namespace glm;


class Camera
{
public:

	Camera();

	void initMenu(int numberOfMenus);
	void update(int deltaTime);
	
	void toogleFreeCamera();
	void setFreeCamera(bool b);
	void setCurrentMenu(int men);
	void addMouseAngles(float mx, float my);

	int getCurrentMenu();

	void nextMenu();
	void prevMenu();

	vec3 getFreeCameraNormalicedTarget();
	vec3 getCameraPosition();

	bool isFree();
	mat4 getViewMatrix();

	static constexpr int  transitionTotalTime = 1000;

private:

	bool isNext;
	int transitionTime;

	// Free Camera
	bool freeCamera;
	const float cameraMovingSpeed = 1.0f;
	vec3 freeCameraTarget;
	vec3 freeCameraPosition;
	vec3 direction;
	vec3 right;
	vec3 up;
	vec2 alpha;
	void updateFreeCamera();


	// MENU ELEMENTS
	vector<vec3> menuTargets;
	vec3 menuPosition;
	int currentMenu;
	int previousMenu;


};

#endif