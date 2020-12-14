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
	static constexpr char winTile	= 'W';
	static constexpr char snake		= 'X';

	static constexpr char wallCheckVR = ')';
	static constexpr char wallCheckVL = '(';
	static constexpr char wallCheckHR = '}';
	static constexpr char wallCheckHL = '{';

	static constexpr char verticalSlideChase		= '|';
	static constexpr char verticalSlideEscape		= '!';
	static constexpr char horizontalSlideChase		= '_';
	static constexpr char horizontalSlideEscape		= '-';

	static constexpr char blank						= '.';
	static constexpr char spawnPoint				= 'S';

	static constexpr char releasedButton = '?';
	static constexpr char pressedButton = '�';
	static constexpr char switchable = '+';

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
