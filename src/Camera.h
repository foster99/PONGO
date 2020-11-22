#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
using namespace std;


class Camera
{
public:

	Camera();

	void initMenu(int numberOfMenus);
	void update();
	
	void toogleFreeCamera();
	void setFreeCamera(bool b);
	void setCurrentMenu(int men);
	void addMouseAngles(float mx, float my);

	bool isFree();
	glm::mat4 getViewMatrix();

private:

	// Free Camera
	bool freeCamera;
	const float cameraMovingSpeed = 0.2;
	glm::vec3 freeCameraTarget;
	glm::vec3 freeCameraPosition;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec2 alpha;
	void updateFreeCamera();


	// MENU ELEMENTS
	vector<glm::vec3> menuTargets;
	glm::vec3 menuPosition;
	int currentMenu;

	// PLAYING ELEMENTS
	vector<glm::vec3> playingTargets;
	vector<glm::vec3> playingPosition;

};

#endif