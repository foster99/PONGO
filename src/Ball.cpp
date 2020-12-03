#include "Ball.h"
#include "GameScene.h"

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

	setSpeed(vec2(0.5f));
	setDirection(vec2(1.f, 1.f));
	setPosition(vec2(30.f, -30.f));
}

void Ball::render()
{
	// CAMBIAR COSAS DEL SHADER (UNIFORMS)

	// Compute ModelMatrix
	mat4 modelMatrix = mat4(1.0f);
	modelMatrix = translate(modelMatrix, vec3(position, 0.f));
	modelMatrix = scale(modelMatrix, vec3(float(scene->getLevel()->getTileSize()) / model->getHeight()));
	modelMatrix = translate(modelMatrix, -model->getCenter());

	// Compute NormalMatrix
	mat3 normalMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));

	// Set uniforms
	program->use();
	program->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	program->setUniformMatrix4f("projection", projection);
	program->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	program->setUniformMatrix3f("normalmatrix", normalMatrix);

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

vec4 Ball::getBoundingBox()
{
	float displacement = float(scene->getLevel()->getTileSize()) / 2.f;

	vec4 BB(position.x, position.x, position.y, position.y);

	BB[0] -= displacement;
	BB[1] += displacement;
	BB[2] += displacement;
	BB[3] -= displacement;

	return BB;
}

vector<ivec2> Ball::occupiedTiles()
{
	vec4 BB = Ball::getBoundingBox();

	vector<ivec2> coords(4);
	coords[0] = scene->toTileCoords(vec2(BB[0], BB[2]));
	coords[1] = scene->toTileCoords(vec2(BB[0], BB[3]));
	coords[2] = scene->toTileCoords(vec2(BB[1], BB[2]));
	coords[3] = scene->toTileCoords(vec2(BB[1], BB[3]));

	return coords;
}
