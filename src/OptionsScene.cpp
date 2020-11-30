#include "OptionsScene.h"

void OptionsScene::init()
{
	this->Scene::init();

	GAME_PAUSED = new Model();
	EXIT_GAME = new Model();

	GAME_PAUSED->loadFromFile("models/GamePaused.obj", (*defaultShaderProgram));
	EXIT_GAME->loadFromFile("models/ExitGame.obj", (*defaultShaderProgram));

	cam = new Camera();
	cam->initMenu(2);
	cam->setCurrentMenu(fixedCamera_01);
}

void OptionsScene::render()
{
	glm::mat4 modelMatrix, viewMatrix = cam->getViewMatrix();
	glm::mat3 normalMatrix;


	renderAxis();


	defaultShaderProgram->use();
	defaultShaderProgram->setUniformMatrix4f("projection", projection);
	defaultShaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	// RENDER WORLD
	//world->setViewMatrix(viewMatrix);
	//world->setProjMatrix(projection);
	////world->render();



	// GAME_PAUSED

	// ModelMatrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * PI / 19.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -12.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(9.0f / GAME_PAUSED->getHeight()));
	modelMatrix = glm::translate(modelMatrix, -GAME_PAUSED->getCenter());
	// NormalMatrix
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	// Set uniforms
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	// Render
	GAME_PAUSED->render(*defaultShaderProgram);



	// EXIT GAME

	// ModelMatrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * PI / 19.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, 12.f));
	modelMatrix = glm::rotate(modelMatrix, PI, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(9.0f / EXIT_GAME->getHeight()));
	modelMatrix = glm::translate(modelMatrix, -EXIT_GAME->getCenter());
	// NormalMatrix
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	// Set uniforms
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	// Render
	EXIT_GAME->render(*defaultShaderProgram);

}

void OptionsScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);
}

void OptionsScene::nextMenu()
{
	cam->nextMenu();
}

void OptionsScene::prevMenu()
{
	cam->prevMenu();
}
