#include "Camera.h"
#include "Game.h"

#define PI 3.14159f

float degToRad(float alpha) {
	return alpha * (PI / 180.f);
}

void Camera::initMenu(int split)
{
	menuTargets = vector<glm::vec3>(split);
	menuPosition = glm::vec3(0,0,0);

	float step = 360.f / split;
	
	menuTargets[0]= glm::vec3(0, 0, -1);
	glm::vec4 prevPoint = glm::vec4(0, 0, -1, 1);
	glm::mat4 rotMat;

	rotMat = glm::rotate(degToRad(step), glm::vec3(0, 1, 0));

	for (int i = 1; i < split; ++i) {
		glm::vec4 actPoint = rotMat * prevPoint;

		menuTargets[i]= glm::vec3(actPoint.x, actPoint.y, actPoint.z);

		prevPoint = actPoint;
	}

	currentMenu = 0;
	
	// MAY CHANGE
	freeCameraPosition = menuPosition;
	freeCameraTarget = menuTargets[0];
	freeCamera = true;
}

void Camera::update()
{
	if (freeCamera) {
		if (Game::instance().getKey('w')) {
			freeCameraPosition += glm::vec3(0, 0, -0.2);
			freeCameraTarget += glm::vec3(0, 0, -0.2);
		}
		else if (Game::instance().getKey('s')) {
			freeCameraPosition += glm::vec3(0, 0, 0.2);
			freeCameraTarget += glm::vec3(0, 0, 0.2);
		}

		if (Game::instance().getKey('a')) {
			freeCameraPosition += glm::vec3(-0.2, 0, 0);
			freeCameraTarget += glm::vec3(-0.2, 0, 0);
		}
		else if (Game::instance().getKey('d')) {
			freeCameraPosition += glm::vec3(0.2, 0, 0);
			freeCameraTarget += glm::vec3(0.2, 0, 0);
		}
			
	}
}

void Camera::toogleFreeCamera()
{
	freeCamera = !freeCamera;
}

void Camera::setCurrentMenu(int men)
{
	if(menuTargets.size() > men)
		currentMenu = men;
}

glm::mat4 Camera::getModelView()
{
	//     glm::lookAt(     position,           target,                worldUp);
	if(freeCamera)
		return glm::lookAt(freeCameraPosition, freeCameraTarget, glm::vec3(0, 1, 0));
	
	return glm::lookAt(menuPosition, menuTargets[currentMenu], glm::vec3(0, 1, 0));
}