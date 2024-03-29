#include "Entity.h"
#include "GameScene.h"

Entity::~Entity()
{
    if (model != nullptr)
        delete model; 
    if (program != nullptr)
        delete program;
}

Entity::Entity()
    : Entity(nullptr, nullptr,nullptr)
{
    // Empty Code
}

Entity::Entity(GameScene* scene, Model* model, ShaderProgram* program)
    : viewMatrix(mat4(1.f)), projMatrix(mat4(1.f))
{
    this->model     = model;
    this->scene     = scene;
    this->program   = program;
}

void Entity::init()
{

}

void Entity::render()
{
    model->render(*(program));
}

void Entity::update(int deltaTime)
{
    viewMatrix = scene->Scene::getViewMatrix();
    projMatrix = scene->Scene::getProjMatrix();
}

vec2 Entity::getPosition()
{
    return position;
}

vec2 Entity::getSpeed()
{
    return speed;
}

vec2 Entity::getDirection()
{
    return direction;
}

vec2 Entity::getPreviousPosition()
{
    return positionHistory.front().position;
}

int Entity::getPreviousTick()
{
    if (positionHistory.empty()) return -1;
    return positionHistory.front().tick;
}

int Entity::getPreviousTime()
{
    if (positionHistory.empty()) return -1;
    return positionHistory.front().time;
}

int Entity::getCurrentChunk()
{
    return scene->getChunkOfCoords(position);
}

void Entity::setPosition(vec2 position, int tick)
{
    positionHistory.push_front(oldPosition(position, tick, scene->getCurrentTime()));
    this->position = position;
}

void Entity::setSpeed(vec2 speed)
{
    this->speed = speed;
}

void Entity::setDirection(vec2 direction)
{
    this->direction = normalize(direction);
}

void Entity::invertDirectionX()
{
    vec2 currentDirection = this->getDirection();
    this->setDirection(vec2(-1.f, 1.f) * currentDirection);
}

void Entity::invertDirectionY()
{
    vec2 currentDirection = this->getDirection();
    this->setDirection(vec2(1.f, -1.f) * currentDirection);
}

void Entity::clearHistory()
{
    while (positionHistory.size() > 1000)
        positionHistory.pop_back();
}

bool Entity::rollbackPosition()
{
    if (positionHistory.empty())return false;
    position = positionHistory.front().position;
    positionHistory.pop_front();

    if (positionHistory.empty()) return false;
    position = positionHistory.front().position;
    positionHistory.pop_front();

    return ! positionHistory.empty();
}

void Entity::setViewMatrix(const mat4& view)
{
    viewMatrix = view;
}

void Entity::setProjMatrix(const mat4& proj)
{
    projMatrix = proj;
}