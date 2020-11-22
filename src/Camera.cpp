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
	rotAngles = glm::vec2(0);
	dispW = glm::vec4(0, 0, -0.2, 1);
	dispS = glm::vec4(0, 0, 0.2, 1);
	dispA = glm::vec4(-0.2, 0, 0, 1);
	dispD = glm::vec4(0.2, 0, 0, 1);
}

void Camera::update()
{
	if (freeCamera) {
		glm::mat4 moverPutoPunto = glm::mat4(1);
		if (changedAngles) {
			
			moverPutoPunto = glm::translate(moverPutoPunto, -freeCameraPosition);
			moverPutoPunto = glm::rotate(moverPutoPunto, rotAngles.x, glm::vec3(0, 1, 0));
			//moverPutoPunto = glm::rotate(moverPutoPunto, rotAngles.y, glm::vec3(1, 0, 0));
			moverPutoPunto = glm::translate(moverPutoPunto, freeCameraPosition);

			glm::vec4 aux = moverPutoPunto * glm::vec4(freeCameraTarget, 1);
			freeCameraTarget = glm::vec3(aux);
			changedAngles = false;
		}

		if (Game::instance().getKey('w')) {
			if (changedAngles)
				dispW = moverPutoPunto * dispW;

			freeCameraPosition += glm::vec3(dispW);
			freeCameraTarget += glm::vec3(dispW);
		}
		else if (Game::instance().getKey('s')) {
			if (changedAngles)
				dispS = moverPutoPunto * dispS;

			freeCameraPosition += glm::vec3(dispS);
			freeCameraTarget += glm::vec3(dispS);
		}

		if (Game::instance().getKey('a')) {
			if (changedAngles)
				dispA = moverPutoPunto * dispA;

			freeCameraPosition += glm::vec3(dispA);
			freeCameraTarget += glm::vec3(dispA);
		}
		else if (Game::instance().getKey('d')) {
			if (changedAngles)
				dispD = moverPutoPunto * dispD;

			freeCameraPosition += glm::vec3(dispD);
			freeCameraTarget += glm::vec3(dispD);
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

void Camera::addMouseAngles(float mx, float my)
{
	rotAngles = glm::vec2(mx, my);
	changedAngles = true;
}

glm::mat4 Camera::getView()
{
	//     glm::lookAt(     position,           target,                worldUp);
	if (freeCamera)
		return glm::lookAt(freeCameraPosition, freeCameraTarget, glm::vec3(0, 1, 0));
	
	return glm::lookAt(menuPosition, menuTargets[currentMenu], glm::vec3(0, 1, 0));
}