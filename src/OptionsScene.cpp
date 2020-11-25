#include "OptionsScene.h"

void OptionsScene::init()
{
	this->Scene::init();

	model = new Model();
	model->loadFromFile("models/FIB.obj", (*defaultShaderProgram));
}

void OptionsScene::render()
{
	glm::mat4 modelMatrix, viewMatrix;
	glm::mat3 normalMatrix;

	defaultShaderProgram->use();
	defaultShaderProgram->setUniformMatrix4f("projection", projection);
	defaultShaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	viewMatrix = cam->getViewMatrix();

	// Render level
	modelMatrix = glm::mat4(1.0f);
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	world->render();


	// Render loaded model
	float scaleFactor = 1.f / model->getHeight();
	glm::vec3 centerModelBase = model->getCenter() - glm::vec3(0.f, -model->getHeight() / 2.f, 0.f);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.5f * fabs(sinf(3.f * currentTime / 1000.f)), 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -5.f));
	modelMatrix = glm::rotate(modelMatrix, currentTime / 1000.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	modelMatrix = glm::translate(modelMatrix, -centerModelBase);
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);

	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);

	model->render(*(defaultShaderProgram));
}

void OptionsScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);
}