#include "GameScene.h"

void GameScene::init()
{
	this->Scene::init();

	models.resize(5, new Model());

	for (Model* model : models)
		model->loadFromFile("models/sphere.obj", (*defaultShaderProgram));

	level = new Level(1);
}

void GameScene::render()
{
	float     scaleFactor;
	glm::vec3 centerModelBase;
	glm::vec3 transl = vec3(0);
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	glm::mat4 viewMatrix = cam->getViewMatrix();

	renderAxis();

	world->setViewMatrix(viewMatrix);
	world->setProjMatrix(projection);
	//world->render();

	float ratio;
	for (Model* model : models)
	{
		ratio = model->getHeight();
		scaleFactor = ratio / model->getHeight();
		centerModelBase = model->getCenter() - glm::vec3(0.f, -model->getHeight() / 2.f, 0.f);

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.5f * fabs(sinf(3.f * currentTime / 1000.f)), 0.f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
		modelMatrix = glm::translate(modelMatrix, -centerModelBase);
		modelMatrix = glm::translate(modelMatrix, transl);

		// Compute Normal Matrix

		normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));

		defaultShaderProgram->use();
		defaultShaderProgram->setUniformMatrix4f("projection", projection);
		defaultShaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
		defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);

		model->render(*defaultShaderProgram);
		transl = transl + vec3(ratio,0,0);
	}

	level->setViewMatrix(viewMatrix);
	level->setProjMatrix(projection);
	level->Level::render();
}

void GameScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);
}

void GameScene::addCube()
{
	Model* m = new Model();
	m->loadFromFile("models/sphere.obj", (*defaultShaderProgram));
	models.push_back(m);
}
