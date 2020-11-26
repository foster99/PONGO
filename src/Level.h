#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "World.h"
#include "Ball.h"
#include "Texture.h"
#include "ShaderProgram.h"
using namespace std;



class Tile	{ private: int a; };

//class Slide : public Entity {};


// ARREGLAR COMPILACION + AJUSTAR CLASE LEVEL A LA HERENCIA

// Class Level renders a very simple room with textures


class Level : public World
{
public:

	// Constructors
	Level(int id = 1);
	
	// Destructors
	~Level();
	void free() override;

	// Rendering
	void render() const override;

private:

	// TileMap
	void loadTileMap();
	void loadTile(char tile, int i, int j);

	int tileSize;
	glm::vec2 chunkSize;
	glm::vec2 mapSizeInTiles;
	glm::vec2 mapSizeInChunks;
	vector<vector<Tile>> map;

	// World stuff
	int levelID;
	glm::vec3 size;
	Texture floor, wall;

	// Entities
	Ball* ball;
	vector<Slide*> slides;
};


#endif // _LEVEL_INCLUDE


