#include <iostream>
#include <cmath>
#include "Scene.h"
#include "Game.h"

#define PI 3.14159f


Scene::Scene()
{
	this->level = nullptr;
	this->cam = nullptr;
	this->selectedCamera = 0;
	this->currentTime = 0.f;
	this->view = glm::mat4(1);
	this->projection = glm::mat4(1);
}

Scene::~Scene()
{
	if(level != NULL)
		delete level;

	if (cam != NULL)
		delete cam;
}

void Scene::init()
{
	initShaders();

	cam = new Camera();
	cam->initMenu(4);
	cam->setCurrentMenu(fixedCamera_01);
	
	currentTime = 0.0f;
	level		= Level::createLevel(glm::vec3(16, 4, 32), texProgram, "images/floor.png", "images/wall.png");
	projection	= glm::perspective(75.f / 180.f * PI, float(Game::instance().WINDOW_WIDTH) / float(Game::instance().WINDOW_HEIGHT), 0.1f, 100.f);
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	cam->update();
}



// SHADER METHODS

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




// CAMERA METHODS

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

