#include "Ball.h"

Ball::~Ball()
{
	this->Entity::~Entity();
}

Ball::Ball(Model* model)
{
	this->Entity::Entity(model);

	program = new ShaderProgram();
}

void Ball::init()
{
	this->Entity::init();

	// ball inits
}

void Ball::render()
{
	// CAMBIAR COSAS DEL SHADER (UNIFORMS)

	this->Entity::render();
}

void Ball::update(int deltaTime)
{
	// update ball
}
