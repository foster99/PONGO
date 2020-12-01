#include "Slide.h"
#include <glm/gtc/matrix_transform.hpp>

Slide::~Slide()
{
	this->Entity::~Entity();
}

Slide::Slide(Model* model, ShaderProgram* prog)
{
	this->Entity::Entity(model,prog);
}

void Slide::init()
{
	this->Entity::init();

	// SI FALTA ALGO ABAIX
	chasing = false;

	speed = 2;
	//pos = vec3(3, 3, 0);
	modI = modJ = 1;
}

void Slide::update(int deltaTime)
{
	// UPDATES O_o

}

void Slide::render()
{
	float     scaleFactor;
	glm::vec3 centerModelBase;
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;
	
	float ratio = model->getHeight();
	scaleFactor = ratio / model->getHeight();
	centerModelBase = model->getCenter() - glm::vec3(0.f, -model->getHeight() / 2.f, 0.f);

	modelMatrix = glm::mat4(1.0f);
	//modelMatrix = translate(modelMatrix, pos);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	modelMatrix = glm::translate(modelMatrix, -centerModelBase);

	// Compute Normal Matrix

	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));

	program->use();
	program->setUniformMatrix4f("projection", projection);
	program->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	program->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	program->setUniformMatrix3f("normalmatrix", normalMatrix);

	// SI FALTA ALGO PER MODIFICAR MODEL ADALT
	this->Entity::render();
}

void Slide::setSize(int ts, int orient)
{
	tileSize = ts;
	if (orientation == vertical) {
		size = ivec2(4, 1);
		orientation = vertical;
	}
	else {
		size = ivec2(1, 4);
		orientation = horizontal;
	}
}

void Slide::setLimits(int head, int tail)
{
	limits = ivec2(head, tail);
}

void Slide::trackPlayer()
{

}
