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

	speed = 0.5;
	currentPos = ogPos = vec3(-5, 5, 0);
}

void Slide::update(int deltaTime)
{
	// UPDATES O_o
	if (orientation == vertical)
		updateVertical(deltaTime);
	else
		updateHorizontal(deltaTime);
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
	modelMatrix = translate(modelMatrix, currentPos);
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

void Slide::updateVertical(int deltaTime)
{
	float len = (size.y * tileSize);

	currentPos.y += speed;

	if (currentPos.y < (ogPos.y - size.y - limits.x*tileSize ) || (currentPos.y + len) > (ogPos.y + len + limits.y * tileSize)) {
		currentPos.x -= speed;
		speed *= -1;
	}
}

void Slide::updateHorizontal(int deltaTime)
{

}

void Slide::setSize(int ts, int orient)
{
	tileSize = ts;
	if (orient == vertical) {
		size = ivec2(1, 4);
		orientation = vertical;
	}
	else {
		size = ivec2(4, 1);
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
