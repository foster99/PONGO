#include "MenuScene.h"

void MenuScene::init()
{
	this->Scene::init();

	PONGO	= new Model();
	PRESS	= new Model();
	SPACE	= new Model();
	TO		= new Model();
	PLAY	= new Model();
	UPC		= new Model();
	FIB		= new Model();

	cam = new Camera();
	cam->initMenu(1);
	cam->setCurrentMenu(fixedCamera_01);
	
	PONGO->loadFromFile("models/PONGO.obj", (*defaultShaderProgram));
	PRESS->loadFromFile("models/PRESS.obj", (*defaultShaderProgram));
	SPACE->loadFromFile("models/SPACE.obj", (*defaultShaderProgram));
	TO->loadFromFile("models/TO.obj", (*defaultShaderProgram));
	PLAY->loadFromFile("models/PLAY.obj", (*defaultShaderProgram));
	UPC->loadFromFile("models/UPC.obj", (*defaultShaderProgram));
	FIB->loadFromFile("models/FIB.obj", (*defaultShaderProgram));
}

void MenuScene::render()
{
	float     scaleFactor;
	glm::vec3 centerModelBase;
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	glm::mat4 viewMatrix = cam->getViewMatrix();


	renderAxis();

	//// Render world
	//world->setViewMatrix(viewMatrix);
	//world->setProjMatrix(projection);
	//world->render();

	Model* modelitos[7] = {PONGO, PRESS, SPACE, TO, PLAY, UPC, FIB};
	


	defaultShaderProgram->use();
	defaultShaderProgram->setUniformMatrix4f("projection", projection);
	defaultShaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	// PONGO

	// ModelMatrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * PI / 16.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 2.5f, -12.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(3.5f / PONGO->getHeight()));
	modelMatrix = glm::translate(modelMatrix, - PONGO->getCenter());
	// NormalMatrix
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	// Set uniforms
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	// Render
	PONGO->render(*defaultShaderProgram);

	float x_disp = 1.5f;
	float pressSpaceToPlayScale = 1.5f / PRESS->getHeight();

	// PRESS

	// ModelMatrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -12.f));
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * PI / 16.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-10.f + x_disp, -3.75f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(pressSpaceToPlayScale));
	modelMatrix = glm::translate(modelMatrix, -PRESS->getCenter());
	// NormalMatrix
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	// Set uniforms
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	// Render
	PRESS->render(*defaultShaderProgram);


	// SPACE

	// ModelMatrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -12.f));
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * PI / 16.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.75f + x_disp, -3.75f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(pressSpaceToPlayScale));
	modelMatrix = glm::translate(modelMatrix, -SPACE->getCenter());
	// NormalMatrix
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	// Set uniforms
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	// Render
	SPACE->render(*defaultShaderProgram);


	// TO

	// ModelMatrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -12.f));
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * PI / 16.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(2.7f + x_disp, -3.75f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(pressSpaceToPlayScale));
	modelMatrix = glm::translate(modelMatrix, -TO->getCenter());
	// NormalMatrix
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	// Set uniforms
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	// Render
	TO->render(*defaultShaderProgram);


	// PLAY

	// ModelMatrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.f, -12.f));
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * PI / 16.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(7.25f + x_disp, -3.75f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(pressSpaceToPlayScale));
	modelMatrix = glm::translate(modelMatrix, -PLAY->getCenter());
	// NormalMatrix
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	// Set uniforms
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	// Render
	PLAY->render(*defaultShaderProgram);



	// UPC

	// ModelMatrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(13.f, 7.5f, -12.f));
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * -PI / 20.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.f / UPC->getHeight()));
	modelMatrix = glm::translate(modelMatrix, -UPC->getCenter());
	// NormalMatrix
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	// Set uniforms
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix* modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	// Render
	UPC->render(*defaultShaderProgram);


	// FIB

	// ModelMatrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-11.5f, 7.5f, -12.f));
	modelMatrix = glm::rotate(modelMatrix, glm::sin(currentTime / 2000.f) * -PI / 20.f, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f / FIB->getHeight()));
	modelMatrix = glm::translate(modelMatrix, -FIB->getCenter());
	// NormalMatrix
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	// Set uniforms
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix* modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);
	// Render
	FIB->render(*defaultShaderProgram);
}

void MenuScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);
}

void MenuScene::nextMenu()
{
	cam->nextMenu();
}

void MenuScene::prevMenu()
{
	cam->prevMenu();
}
