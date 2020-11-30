#include "Slide.h"

Slide::~Slide()
{
	this->Entity::~Entity();
}

Slide::Slide(Model* model)
{
	this->Entity::Entity(model);

	program = new ShaderProgram();
}

void Slide::init()
{
	this->Entity::init();

	// SI FALTA ALGO ABAIX
}

void Slide::update(int deltaTime)
{
	// UPDATES O_o	
}

void Slide::render()
{
	// SI FALTA ALGO PER MODIFICAR MODEL ADALT
	this->Entity::render();
}

void Slide::trackPlayer()
{

}
