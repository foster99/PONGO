#include "WinScene.h"

void WinScene::init()
{
	this->Scene::init();

	youWinModel = new Model();
	youWinModel->loadFromFile("models/youWin.obj", (*defaultShaderProgram));

	totalTime = 25000.f;

	cam = new Camera();
	cam->initMenu(1);
	cam->setCurrentMenu(fixedCamera_01);
}

void WinScene::render()
{
	if (totalTime < 0.f) return;

	glm::mat4 modelMatrix, viewMatrix = cam->getViewMatrix();
	glm::mat3 normalMatrix;

	renderAxis();

	defaultShaderProgram->use();
	defaultShaderProgram->setUniformMatrix4f("projection", projection);
	defaultShaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	// EXIT GAME
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 85.f * displacement, -32.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(30.0f / youWinModel->getHeight()));
	modelMatrix = glm::translate(modelMatrix, -youWinModel->getCenter());
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	youWinModel->render(*defaultShaderProgram);
}

void WinScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);

	totalTime -= float(deltaTime);

	displacement = smoothstep(25000.f, 0.f, totalTime) - 0.5f;
}