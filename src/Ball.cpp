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

	setSpeed(vec2(1.00f));
	setDirection(vec2(-1.f, 1.f));
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

contourPointList Ball::listOfContourPoints()
{
	vec4 BB = Ball::getBoundingBox();

	float xmin =  BB[0];
	float xmax =  BB[1];
	float ymax = -BB[2];
	float ymin = -BB[3];

	contourPointList points;
	float step = 0.5f;

	for (float x = xmin; x < xmax; x += step)
		points.down.push_back(vec2(x, ymax));

	for (float x = xmin; x < xmax; x += step)
		points.up.push_back(vec2(x, ymin));

	for (float y = ymin; y < ymax; y += step)
		points.left.push_back(vec2(xmin, y));

	for (float y = ymin; y < ymax; y += step)
		points.right.push_back(vec2(xmax, y));

	points.down.push_back(vec2(xmax, ymin));
	points.up.push_back(vec2(xmax, ymax));
	points.left.push_back(vec2(xmin, ymax));
	points.right.push_back(vec2(xmax, ymax));

	return points;
}

contourTileList Ball::listOfContourTiles()
{
	contourPointList ballContourPoints = listOfContourPoints();
	contourTileList ballContourTiles;

	for (vec2 point : ballContourPoints.down)
		ballContourTiles.down.push_back(scene->getLevel()->getTile(scene->toTileCoordsNotInverting(point)));
	
	for (vec2 point : ballContourPoints.up)
		ballContourTiles.up.push_back(scene->getLevel()->getTile(scene->toTileCoordsNotInverting(point)));
	
	for (vec2 point : ballContourPoints.right)
		ballContourTiles.right.push_back(scene->getLevel()->getTile(scene->toTileCoordsNotInverting(point)));
	
	for (vec2 point : ballContourPoints.left)
		ballContourTiles.left.push_back(scene->getLevel()->getTile(scene->toTileCoordsNotInverting(point)));

	return ballContourTiles;
}

vec4 Ball::getBoundingBox()
{
	float displacement = float(scene->getLevel()->getTileSize()) / 2.f;

	vec4 BB(position.x, position.x, position.y, position.y);

	BB[0] -= displacement;
	BB[1] += displacement;
	BB[2] -= displacement;
	BB[3] += displacement;

	return BB;
}

vector<vector<ivec2>> Ball::occupiedTiles()
{
	vec4 BB = Ball::getBoundingBox();

	float xmin = BB[0];
	float xmax = BB[1];
	float ymin = BB[2];
	float ymax = BB[3];

	vector<vector<ivec2>> tiles(2, vector<ivec2>(2));
	tiles[0][0] = scene->toTileCoords(vec2(xmin, ymax));
	tiles[0][1] = scene->toTileCoords(vec2(xmax, ymax));
	tiles[1][0] = scene->toTileCoords(vec2(xmin, ymin));
	tiles[1][1] = scene->toTileCoords(vec2(xmax, ymin));

	return tiles;
}
