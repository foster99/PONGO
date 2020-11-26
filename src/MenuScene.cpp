#include "MenuScene.h"

void MenuScene::init()
{
	this->Scene::init();

	model = new Model();
	model->loadFromFile("models/PRESS.obj", (*defaultShaderProgram));

	//model2 = new Model();
	//model2->loadFromFile("models/PLAY.obj", (*defaultShaderProgram));
}

void MenuScene::render()
{
	float     scaleFactor;
	glm::vec3 centerModelBase;
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	glm::mat4 viewMatrix = cam->getViewMatrix();


	renderAxis();

	// Render world
	world->setViewMatrix(viewMatrix);
	world->setProjMatrix(projection);
	world->render();


	// Render loaded model

		// Compute Model Matrix
	scaleFactor		= 1.f / model->getHeight();
	centerModelBase = model->getCenter() - glm::vec3(0.f, -model->getHeight() / 2.f, 0.f);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.5f * fabs(sinf(3.f * currentTime / 1000.f)), 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -5.f));
	modelMatrix = glm::rotate(modelMatrix, currentTime / 1000.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	modelMatrix = glm::translate(modelMatrix, -centerModelBase);

		// Compute Normal Matrix
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));

	defaultShaderProgram->use();
	defaultShaderProgram->setUniformMatrix4f("projection", projection);
	defaultShaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);

		// render
	//odel->render(*(defaultShaderProgram));
}

void MenuScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);
}