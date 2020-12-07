#include "Ball.h"
#include "GameScene.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

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

	setSpeed(vec2(1.00f));
	setDirection(vec2(-1.f, 1.f));
	setPosition(spawnPoint);

	// INICIALIZAR PARTICULAS EN PELOTA
	billboard = Billboard::createBillboard(glm::vec2(10.f, 10.f), (*program), "images/bee.png");
	billboard->setType(BILLBOARD_Y_AXIS);

	ParticleSystem::Particle particle;
	particle.lifetime = 1e10f;
	particles = new ParticleSystem();
	particles->init(glm::vec2(1.f,1.f), (*program), "images/particle.png",0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	currentTime = 0.0f;
}

void Ball::render()
{
	// CAMBIAR COSAS DEL SHADER (UNIFORMS)

	float scale_factor = float(scene->getLevel()->getTileSize()) / model->getHeight();
	// Compute ModelMatrix
	mat4 modelMatrix = mat4(1.0f);
	modelMatrix = translate(modelMatrix, vec3(position, 0.f));
	modelMatrix = scale(modelMatrix, vec3(scale_factor, scale_factor, scale_factor/2.f));
	modelMatrix = translate(modelMatrix, -model->getCenter());

	// Compute NormalMatrix
	mat3 normalMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));

	// Set uniforms
	program->use();
	program->setUniform1b("bLighting", true);
	program->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	program->setUniformMatrix4f("projection", projMatrix);
	program->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	program->setUniformMatrix3f("normalmatrix", normalMatrix);

	this->Entity::render();

	//// Render billboard
	//program->setUniform1b("bLighting", false);
	//modelMatrix = glm::mat4(1.0f);
	//program->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	//normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	//program->setUniformMatrix3f("normalmatrix", normalMatrix);
	//billboard->render(vec3(position,11), scene->getCameraPosition());

	// Render particles
	/*glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);*/

	program->setUniform1b("bLighting", false);
	modelMatrix = glm::mat4(1.0f);
	program->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));
	program->setUniformMatrix3f("normalmatrix", normalMatrix);
	particles->render(scene->getCameraPosition());

	/*glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);*/
}

void Ball::update(int deltaTime)
{
	this->Entity::update(deltaTime);
	// update ball

	// UPDATE PARTICULAS
	currentTime += deltaTime;

	particles->update(deltaTime / 1000.f);
}

void Ball::setPosition(vec2 position, int time)
{
	this->Entity::setPosition(position, time);
}

void Ball::setSpeed(vec2 speed)
{
	this->Entity::setSpeed(speed);
}

void Ball::setDirection(vec2 direction)
{
	this->Entity::setDirection(direction);
}

void Ball::setSpawnPoint(vec2 coords)
{
	spawnPoint = coords;
}

void Ball::displacePosition(vec2 displacement)
{
	position += displacement;
}

void Ball::locateInSpawnPoint()
{
	setPosition(spawnPoint);
}

void Ball::spawnParticles()
{
	int nParticlesToSpawn = 18;
	ParticleSystem::Particle particle;
	float angleStep = 360.f/(float)nParticlesToSpawn;

	particle.lifetime = 0.7f;
	for (int i = 0; i < nParticlesToSpawn; i++)
	{
		float currAngle = (angleStep * i) * (PI / 180.f);
		particle.position = vec3(position, 0) + vec3(5 * direction.x,0,0) + glm::vec3(cos(currAngle), 0.f, sin(currAngle));
		
		//particle.speed = vec3(15.f*cos(currAngle)*direction.x,10*direction.y,10*sin(currAngle));
		particle.speed = vec3(10*direction.y, 10.f * cos(currAngle) * direction.x,10*sin(currAngle));
		
		//particle.speed = 1.5f * glm::normalize(0.5f * particle.position + glm::vec3(3.f, 3.f, 3.f));
		particles->addParticle(particle);
	}
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
