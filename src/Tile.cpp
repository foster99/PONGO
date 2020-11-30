#include "Tile.h"
#include "Scene.h"

Tile::Tile() :
	coords(ivec2(0)),
	type(undefined),
	deadly(false),
	solid(false)
{
	// Empty
}


Tile::Tile(ivec2 coords, char type, bool solid, bool deadly)
{
	this->coords	= coords;
	this->type		= type;
	this->solid		= solid;
	this->deadly	= deadly;
}

bool Tile::isUndefined() const
{
	return type == undefined;
}
