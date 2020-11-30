#include "Tile.h"

Tile::Tile(vec2 coords, char type, bool solid, bool deadly)
{
	this->coords	= coords;
	this->type		= type;
	this->solid		= solid;
	this->deadly	= deadly;
}
