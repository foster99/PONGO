#include "OptionsScene.h"
#include "Game.h"

void OptionsScene::init()
{
	this->Scene::init();

	GAME_PAUSED = new Model();
	EXIT_GAME = new Model();
	ARROW_R = new Model();
	ARROW_L = new Model();

	GAME_PAUSED->loadFromFile("models/GamePaused.obj", (*defaultShaderProgram));
	EXIT_GAME->loadFromFile("models/ExitGame.obj", (*defaultShaderProgram));
	ARROW_R->loadFromFile("models/RIGHT_ARROW.obj", (*defaultShaderProgram));
	ARROW_L->loadFromFile("models/LEFT_ARROW.obj", (*defaultShaderProgram));

	menu = pauseGame;

	cam = new Camera();
	cam->initMenu(Nmenus);
	cam->setCurrentMenu(fixedCamera_01);
}

void OptionsScene::render()
{
	glm::mat4 modelMatrix, viewMatrix = cam->getViewMatrix();
	glm::mat3 normalMatrix;

	float lateral_displacement = 0.5f * glm::sin(currentTime / 250.f);


	renderAxis();


	defaultShaderProgram->use();
	defaultShaderProgram->setUniformMatrix4f("projection", projection);
	defaultShaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	// RENDER WORLD
	//world->setViewMatrix(viewMatrix);
	//world->setProjMatrix(projection);
	////world->render();


	// RIGHT ARROW
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, 2.f*PI/float(Nmenus), glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(18.f + lateral_displacement, 0.f, -16.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f,1.f,0.25f) * 3.0f / ARROW_R->getHeight());
	modelMatrix = glm::translate(modelMatrix, -ARROW_R->getCenter());
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	ARROW_R->render(*defaultShaderProgram);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(18.f + lateral_displacement, 0.f, -16.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 1.f, 0.25f) * 3.0f / ARROW_R->getHeight());
	modelMatrix = glm::translate(modelMatrix, -ARROW_R->getCenter());
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	ARROW_R->render(*defaultShaderProgram);


	// LEFT ARROW
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, 2.f*PI/float(Nmenus), glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-(18.f + lateral_displacement), 0.f, -16.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 1.f, 0.25f) * 3.0f / ARROW_L->getHeight());
	modelMatrix = glm::translate(modelMatrix, -ARROW_L->getCenter());
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	ARROW_L->render(*defaultShaderProgram);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-(18.f + lateral_displacement), 0.f, -16.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f, 1.f, 0.25f) * 3.0f / ARROW_L->getHeight());
	modelMatrix = glm::translate(modelMatrix, -ARROW_L->getCenter());
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	ARROW_L->render(*defaultShaderProgram);


	// GAME_PAUSED
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * PI / 19.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -12.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(9.0f / GAME_PAUSED->getHeight()));
	modelMatrix = glm::translate(modelMatrix, -GAME_PAUSED->getCenter());
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	GAME_PAUSED->render(*defaultShaderProgram);


	// EXIT GAME
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * PI / 19.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, 12.f));
	modelMatrix = glm::rotate(modelMatrix, 2.f * PI / float(Nmenus), glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(9.0f / EXIT_GAME->getHeight()));
	modelMatrix = glm::translate(modelMatrix, -EXIT_GAME->getCenter());
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	EXIT_GAME->render(*defaultShaderProgram);

}

void OptionsScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);
}

void OptionsScene::restartOptions()
{
	menu = pauseGame;
	cam->setCurrentMenu(fixedCamera_01);
}

int OptionsScene::getMenu()
{
	return cam->getCurrentMenu();
}

void OptionsScene::nextMenu()
{
	menu = (menu + 1) % Nmenus;
	cam->nextMenu();
	Game::instance().playGotaSound();
}

void OptionsScene::prevMenu()
{
	Game::instance().playGotaSound();
	cam->prevMenu();
}
