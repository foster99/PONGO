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
	tracked = false;
	initialized = false;
	speed = 0.5;
	position = ogPos = vec2(-5, 5);
}

void Slide::update(int deltaTime)
{
	// UPDATES EN UN FUTUR QUANT FOWOSTER DIGUI NOMES HAN DE CANVIAR DIRECTION I SPEED
	// SET POSTITION CANVIARA RESPECTE AQUESTES DADES
	if (orientation == vertical) {
		if (!trackPlayerVertical())
			updateVertical(deltaTime);
	}
	else {
		if (!trackPlayerHorizontal())
			updateHorizontal(deltaTime);
	}
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
	modelMatrix = translate(modelMatrix, vec3(position,0));

	if(orientation == horizontal)
		modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 1, 1));
	else
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 4, 1));

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

	position.y += speed * direction.y;

	if (position.y < (ogPos.y - size.y - limits.y * tileSize ) || (position.y + len) > (ogPos.y + len + limits.y * tileSize)) {
		position.y -= speed * direction.y;
		direction.y *= -1;
	}
}

void Slide::updateHorizontal(int deltaTime)
{
	float len = (size.x * tileSize);

	position.x += speed * direction.x;

	if (position.x < (ogPos.x - size.x - limits.x * tileSize) || (position.x + len) >(ogPos.x + len + limits.x * tileSize)) {
		position.x -= speed * direction.x;
		direction.x *= -1;
	}
}

void Slide::setSize(int ts, int orient)
{
	tileSize = ts;
	if (orient == vertical) {
		size = ivec2(1, 4);
		orientation = vertical;
		direction = vec2(0, 1);
	}
	else {
		size = ivec2(4, 1);
		orientation = horizontal;
		direction = vec2(1,0);
	}
}

void Slide::setLimits(int head, int tail)
{
	limits = ivec2(head, tail);
}

void Slide::setPosition(vec2 position)
{
	this->Entity::setPosition(position);
	
	if (!initialized) {
		ogPos = position;
		initialized = true;
	}
}

void Slide::setSpeed(vec2 speed)
{
	this->Entity::setSpeed(speed);
}

void Slide::setDirection(vec2 direction)
{
	this->Entity::setDirection(direction);
}

bool Slide::trackPlayerVertical()
{
	return false;
}

bool Slide::trackPlayerHorizontal()
{
	return false;
}
