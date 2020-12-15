#include "ExitScene.h"

void ExitScene::init()
{
	this->Scene::init();

	EXIT_GAME = new Model();
	EXIT_GAME->loadFromFile("models/definitiveExit.obj", (*defaultShaderProgram));

	cam = new Camera();
	cam->initMenu(Nmenus);
	cam->setCurrentMenu(fixedCamera_01);
}

void ExitScene::render()
{
	glm::mat4 modelMatrix, viewMatrix = cam->getViewMatrix();
	glm::mat3 normalMatrix;

	renderAxis();

	defaultShaderProgram->use();
	defaultShaderProgram->setUniformMatrix4f("projection", projection);
	defaultShaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	// EXIT GAME
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * PI / 19.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -32.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(30.0f / EXIT_GAME->getHeight()));
	modelMatrix = glm::translate(modelMatrix, -EXIT_GAME->getCenter());
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	EXIT_GAME->render(*defaultShaderProgram);
}

void ExitScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);
}