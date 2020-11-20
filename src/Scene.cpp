#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"

// FOWOSTER SALU2

#define PI 3.14159f


Scene::Scene()
{
	level = NULL;
}

Scene::~Scene()
{
	if(level != NULL)
		delete level;
}


void Scene::init()
{
	initShaders();
	level = Level::createLevel(glm::vec3(16, 4, 32), texProgram, "images/floor.png", "images/wall.png");
	projection = glm::perspective(45.f / 180.f * PI, float(CAMERA_WIDTH) / float(CAMERA_HEIGHT), 0.1f, 100.f);
	selectedCamera = defaultCamera;

	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	updateCamera();
}

void Scene::render()
{
	glm::mat4 modelview;

	switch (selectedCamera)
	{
		//            glm::lookAt(     position,           target,                worldUp);
	case defaultCamera:
		modelview = glm::mat4(1.f);
		break;
	case fpsCamera:
		modelview = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
		break;
	case fixedCamera_02:
		modelview = glm::lookAt(glm::vec3(10, 16, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		break;
	case fixedCamera_03:
		modelview = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		break;
	case fixedCamera_04:
		modelview = glm::lookAt(glm::vec3(10, 20, 20), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		break;
	case fixedCamera_05:
		modelview = glm::lookAt(glm::vec3(10, 45, 43), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		break;
	}

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	level->render();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::updateCamera() {

}

void Scene::setCamera(camera c) {
	selectedCamera = c;
}

