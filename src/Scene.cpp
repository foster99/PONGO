#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

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

	cam = new Camera();
	cam->initMenu(4);

	level = Level::createLevel(glm::vec3(16, 4, 32), texProgram, "images/floor.png", "images/wall.png");
	projection = glm::perspective(75.f / 180.f * PI, float(Game::instance().WINDOW_WIDTH) / float(Game::instance().WINDOW_HEIGHT), 0.1f, 100.f);
	cam->setCurrentMenu(fixedCamera_01);

	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	cam->update();
}

void Scene::render()
{
	glm::mat4 modelview;

	modelview = cam->getViewMatrix();

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

void Scene::setCamera(camera c)
{
	if (c == Scene::fpsCamera)
	{
		cam->setFreeCamera(true);
		return;
	}
	
	cam->setFreeCamera(false);
	cam->setCurrentMenu(c - 1);
}

bool Scene::cameraIsFree()
{
	return cam->isFree();
}

void Scene::setMouseAngles(int x, int y)
{
	//  (float)deltaTime
	float horizontalAngle = mouseSpeed * float(Game::instance().WINDOW_WIDTH / 2 - x);
	float verticalAngle = mouseSpeed * float(Game::instance().WINDOW_HEIGHT / 2 - y);

	cam->addMouseAngles(horizontalAngle, verticalAngle);
}

