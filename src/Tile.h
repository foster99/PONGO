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

	static constexpr char undefined = ' ';
	static constexpr char cube		= '#';
	static constexpr char spike		= '*';
	
	static constexpr char pinchoU	= '^';
	static constexpr char pinchoD	= 'v';
	static constexpr char pinchoR	= '>';
	static constexpr char pinchoL	= '<';

	static constexpr char ropeH		= '=';
	static constexpr char ropeV		= '"';

	static constexpr char start		= 'S';
	static constexpr char end		= 'E';

	
	Tile();

	Tile(	vec2 coords,
			int chunk,
			char type,
			bool solid,
			bool deadly);

	vec2	coords;
	int		chunk;
	char	type;
	bool	solid;
	bool	deadly;

	bool isUndefined() const;
};
#endif
