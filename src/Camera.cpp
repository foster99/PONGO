#include "Camera.h"
#include "Game.h"

#define PI 3.14159f

float degToRad(float alpha)
{
	return alpha * (PI / 180.f);
}

glm::vec2 degToRad(float alpha1, float alpha2)
{
	return glm::vec2(alpha1 * (PI / 180.f), alpha2 * (PI / 180.f));
}

Camera::Camera()
{
	// MENU ELEMENTS
	menuTargets = vector<glm::vec3>(0);
	menuPosition = glm::vec3(0);
	currentMenu = 0;

	// PLAYING ELEMENTS
	playingTargets = vector<glm::vec3>(0);
	playingPosition = vector<glm::vec3>(0);
	
	// FREE CAMERA INITS
	freeCameraPosition = glm::vec3(0);
	freeCameraTarget = glm::vec3(0);
	direction = glm::vec3(0);
	right = glm::vec3(0);
	up = glm::vec3(0);
	alpha = degToRad(0, -90);
	freeCamera = false;
}

void Camera::initMenu(int numberOfMenus)
{
	menuTargets = vector<glm::vec3>(numberOfMenus);
	menuPosition = glm::vec3(0,0,0);

	float step = 360.f / numberOfMenus;
	
	menuTargets[0]= glm::vec3(0, 0, -1);
	glm::vec4 prevPoint = glm::vec4(0, 0, -1, 1);
	glm::mat4 rotMat;

	rotMat = glm::rotate(degToRad(step), glm::vec3(0, 1, 0));

	for (int i = 1; i < numberOfMenus; ++i)
	{
		glm::vec4 actPoint = rotMat * prevPoint;

		menuTargets[i]= glm::vec3(actPoint.x, actPoint.y, actPoint.z);

		prevPoint = actPoint;
	}

	currentMenu = 0;

	freeCameraPosition = menuPosition;
	freeCameraTarget = menuTargets[0];
}

void Camera::update()
{
	if (freeCamera)
		updateFreeCamera();

}

void Camera::toogleFreeCamera()
{
	freeCamera = !freeCamera;
}

void Camera::setFreeCamera(bool b)
{
	freeCamera = b;
}

void Camera::setCurrentMenu(int men)
{
	if(menuTargets.size() >= men)
		currentMenu = men - 1;
}

void Camera::addMouseAngles(float mx, float my)
{
	alpha += glm::vec2(mx, my);
}

void Camera::nextMenu()
{
	currentMenu++;
	currentMenu %= menuTargets.size();
}

void Camera::prevMenu()
{
	currentMenu--;
	currentMenu %= menuTargets.size();
}

vec3 Camera::getFreeCameraNormalicedTarget()
{
	return freeCameraPosition + direction * 0.2f;
}

vec3 Camera::getCameraPosition()
{
	return freeCameraPosition;
}

void Camera::updateFreeCamera()
{
	// Direction Vector
	direction = glm::vec3(cos(alpha.y) * sin(alpha.x),
		sin(alpha.y),
		cos(alpha.y) * cos(alpha.x));

	// Right vector
	right = glm::vec3(sin(alpha.x - 3.14f / 2.0f),
		0,
		cos(alpha.x - 3.14f / 2.0f));
	// Up vector
	up = glm::cross(right, direction);

	// Position Update
	if (Game::instance().getKey('w'))
		freeCameraPosition += direction * cameraMovingSpeed;
	if (Game::instance().getKey('s'))
		freeCameraPosition -= direction * cameraMovingSpeed;
	if (Game::instance().getKey('a'))
		freeCameraPosition -= right * cameraMovingSpeed;
	if (Game::instance().getKey('d'))
		freeCameraPosition += right * cameraMovingSpeed;

	// Compute Target Point
	freeCameraTarget = freeCameraPosition + direction;
}

bool Camera::isFree()
{
	return freeCamera;
}

glm::mat4 Camera::getViewMatrix()
{
	//     glm::lookAt(     position,           target,                worldUp);

	if (freeCamera)
		return glm::lookAt(freeCameraPosition, freeCameraTarget, up);
	
	return glm::lookAt(menuPosition, menuTargets[currentMenu], glm::vec3(0, 1, 0));
}