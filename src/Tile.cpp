#include "Tile.h"
#include "Scene.h"

Tile::Tile() :
	coords(vec2(0)),
	chunk(0),
	type(undefined),
	deadly(false),
	solid(false)
{
	// Empty
}


Tile::Tile(vec2 coords, int chunk, char type, bool solid, bool deadly)
{
	this->coords	= coords;
	this->chunk		= chunk;
	this->type		= type;
	this->solid		= solid;
	this->deadly	= deadly;
}

bool Tile::isUndefined() const
{
	return type == undefined;
}
