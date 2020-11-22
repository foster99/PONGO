#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include <glm/glm.hpp>
#include <utility> // pair and make_pair

class Entity
{
private:

	glm::vec2 center;

public:

	// Given two entities returs if they collided and so, its collission point according to its Bounding Boxes too.
	static std::pair<bool, glm::vec2> collisionPoint(const Entity& e1, const Entity& e2);

};
#endif

