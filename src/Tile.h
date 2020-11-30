#ifndef _TILE_INCLUDE
#define _TILE_INCLUDE

#include <glm/glm.hpp>
#include "AssimpModel.h"
using namespace std;
using namespace glm;

class Tile
{
public:
	
	Tile(	vec2 coords,
			char type,
			bool solid,
			bool deadly);

	vec2	coords;
	char	type;
	bool	solid;
	bool	deadly;


};
#endif
