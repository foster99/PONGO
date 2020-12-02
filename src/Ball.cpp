#include "Ball.h"
#include "Scene.h"

Ball::~Ball()
{
	this->Entity::~Entity();
}

Ball::Ball(GameScene* scene, Model* model, ShaderProgram* prog)
{
	this->Entity::Entity(scene, model,prog);
}

void Ball::init()
{
	this->Entity::init();

	// estas haciendo el spawnpoint para k la pelota empiece flotando y poder comprobar las colisiones mientras las programas
	spawnPoint = vec2(0);
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
