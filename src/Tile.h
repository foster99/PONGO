#ifndef _TILE_INCLUDE
#define _TILE_INCLUDE

#include <glm/glm.hpp>
#include "AssimpModel.h"
using namespace std;
using namespace glm;

typedef AssimpModel Model;

class Tile
{
public:

	static constexpr char undefined	= ' ';
	static constexpr char cube	= '#';
	static constexpr char spike = '*';
	static constexpr char rope	= 'R';
	static constexpr char start	= 'S';
	static constexpr char end	= 'E';

	
	Tile();

	Tile(	ivec2 coords,
			char type,
			bool solid,
			bool deadly);

	ivec2	coords;
	char	type;
	bool	solid;
	bool	deadly;

	bool isUndefined() const;
};
#endif
