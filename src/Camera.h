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

	glm::mat4 getModelView();

private:

	bool freeCamera;
	glm::vec3 freeCameraPosition;
	glm::vec3 freeCameraTarget;
	
	// MENU ELEMENTS
	vector<glm::vec3> menuTargets;
	glm::vec3 menuPosition;
	int currentMenu;

	// PLAYING ELEMENTS
	vector<glm::vec3> playingTargets;
	vector<glm::vec3> playingPosition;
};

#endif