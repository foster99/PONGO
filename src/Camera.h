#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
using namespace std;


class Camera
{
public:
	void initMenu(int i);
	void update();
	
	void toogleFreeCamera();
	void setCurrentMenu(int men);
	void addMouseAngles(float mx, float my);

	glm::mat4 getView();

private:

	bool freeCamera, changedAngles;
	glm::vec3 freeCameraPosition;
	glm::vec3 freeCameraTarget;
	glm::vec2 rotAngles;
	glm::vec4 dispW, dispS, dispA, dispD;

	// MENU ELEMENTS
	vector<glm::vec3> menuTargets;
	glm::vec3 menuPosition;
	int currentMenu;

	// PLAYING ELEMENTS
	vector<glm::vec3> playingTargets;
	vector<glm::vec3> playingPosition;

};

#endif