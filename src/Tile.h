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
	static constexpr char opened_snake_door = ';';
	static constexpr char closed_snake_door = '@';

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


	static constexpr char switchable_no_solid	= '0';
	static constexpr char switchable_solid		= '1';

	static constexpr char buttonR_ON = '2';
	static constexpr char buttonL_ON = '3';
	static constexpr char buttonU_ON = '4';
	static constexpr char buttonD_ON = '5';

	static constexpr char buttonR_OFF = '6';
	static constexpr char buttonL_OFF = '7';
	static constexpr char buttonU_OFF = '8';
	static constexpr char buttonD_OFF = '9';

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
