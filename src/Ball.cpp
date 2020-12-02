#include "Ball.h"

Ball::~Ball()
{
	this->Entity::~Entity();
}

Ball::Ball(Model* model, ShaderProgram* prog)
{
	this->Entity::Entity(model,prog);
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

void Ball::setPosition(vec2 position)
{
	this->Entity::setPosition(position);
}

void Ball::setSpeed(vec2 speed)
{
	this->Entity::setSpeed(speed);
}

void Ball::setDirection(vec2 direction)
{
	this->Entity::setDirection(direction);
}
