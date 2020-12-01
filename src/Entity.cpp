#include "Entity.h"

Entity::~Entity()
{
    if (model != nullptr)
        delete model; 
    if (program != nullptr)
        delete program;
}

Entity::Entity()
    : Entity(nullptr,nullptr)
{
    // Empty Code
}

Entity::Entity(Model* model, ShaderProgram* prog)
{
    this->model     = model;
    program = prog;
}

void Entity::init()
{

}

void Entity::render()
{
    model->render(*(program));
}

void Entity::setViewMatrix(mat4 view)
{
    viewMatrix = view;
}

void Entity::setProjMatrix(mat4 proj)
{
    projection = proj;
}

std::pair<bool, glm::vec2> Entity::collisionPoint(const Entity& e1, const Entity& e2)
{
    // get center point circle first 
    //glm::vec2 center = e1.center;

    //// get difference vector between both centers
    //glm::vec2 difference = e1.center - e2.center;
    //glm::vec2 clamped = glm::clamp(difference, - aabb_half_extents, aabb_half_extents);
    //// add clamped value to AABB_center and we get the value of box closest to circle
    //glm::vec2 closest = e2.center + clamped;
    //// retrieve vector between center circle and closest point AABB and check if length <= radius
    //difference = closest - e1.center;
    //return glm::length(difference) < e1.Radius;

    return std::pair<bool, glm::vec2>();
}
