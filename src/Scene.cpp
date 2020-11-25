#include <iostream>
#include <cmath>
#include "Scene.h"
#include "Game.h"

#define PI 3.14159f


// CONSTRUCTORS

Scene::Scene() :
	world(nullptr),
	cam(nullptr),
	defaultShaderProgram(nullptr),
	currentTime(0.f),
	view(glm::mat4(1)),
	projection(glm::mat4(1)),
	selectedCamera(0)
{
	this->defaultShaderProgram = new ShaderProgram();
}

Scene::~Scene()
{
	if(world != nullptr)
		delete world;

	if (cam != nullptr)
		delete cam;

	if (defaultShaderProgram != nullptr)
		delete defaultShaderProgram;
}



// DEFAULT SCENE METHODS

void Scene::init()
{
	initDefaultShaders();

	axis = new Model();
	axis->loadFromFile("models/axis.obj", (*defaultShaderProgram));

	cam = new Camera();
	cam->initMenu(4);
	cam->setCurrentMenu(fixedCamera_01);
	
	currentTime = 0.0f;
	projection	= glm::perspective(75.f / 180.f * PI, float(Game::instance().WINDOW_WIDTH) / float(Game::instance().WINDOW_HEIGHT), 0.1f, 1000.f);

	world = new World(defaultShaderProgram);
	world->setViewMatrix(view);
	world->setProjMatrix(projection);
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	world->setCurrentTime(currentTime);
	cam->update();
}



// SHADER METHODS

void Scene::initDefaultShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");

	if (!vShader.isCompiled())
		throw "Vertex Shader Error";


	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
		throw "Fragment Shader Error";

	defaultShaderProgram->init();
	defaultShaderProgram->addShader(vShader);
	defaultShaderProgram->addShader(fShader);
	defaultShaderProgram->link();

	if (!defaultShaderProgram->isLinked())
		throw "Shader Linking Error";

	defaultShaderProgram->bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
}


void Scene::displayText(vec2 position, vec3 color, string text)
{
	const char* string		= text.c_str();
	int			text_length = strlen(string);

	glColor3f(color.x, color.y, color.z);
	glRasterPos2f(position.x - 0.99f, position.y - 0.95f);

	for (int i = 0; i < text_length; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
}

void Scene::renderAxis()
{
	if (!cam->isFree()) return;

	float scaleFactor;
	mat4 modelMatrix;
	mat3 normalMatrix;
	vec3 desiredPosition = cam->getFreeCameraNormalicedTarget();
	mat4 viewMatrix = cam->getViewMatrix();

	defaultShaderProgram->use();
	scaleFactor = 0.015f / axis->getHeight();

	modelMatrix = mat4(1.0f);
	modelMatrix = translate(modelMatrix, desiredPosition);
	modelMatrix = scale(modelMatrix, vec3(scaleFactor));

	normalMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));
	defaultShaderProgram->setUniformMatrix4f("projection", projection);
	defaultShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);


	axis->render(*(defaultShaderProgram));

	string coords = "Coords: (" + to_string(desiredPosition.x) + ", " + to_string(desiredPosition.y) + ", " + to_string(desiredPosition.z) + ")";
	displayText(vec2(0), vec3(1, 0, 0), coords);
}



void Scene::loadShaders(const string& filename, ShaderProgram* program)
{
	string VSpath = "shaders/" + filename + ".vert";
	string FSpath = "shaders/" + filename + ".frag";

	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, VSpath);

	if (!vShader.isCompiled())
		throw "Vertex Shader Error";


	fShader.initFromFile(FRAGMENT_SHADER, FSpath);
	if (!fShader.isCompiled())
		throw "Fragment Shader Error";

	program->init();
	program->addShader(vShader);
	program->addShader(fShader);
	program->link();

	if (!program->isLinked())
		throw "Shader Linking Error";

	program->bindFragmentOutput("outColor");

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