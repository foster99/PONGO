#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

class Tile {
private:
	int a;
};

using namespace std;


// Class Level renders a very simple room with textures


class Level
{

public:

	// Constructors
	static Level *createLevel(const glm::vec3 &levelSize, ShaderProgram &program, const string &floorFile, const string &wallFile);
	Level(const glm::vec3 &levelSize, ShaderProgram &program, const string &floorFile, const string &wallFile);
	
	// Destructors
	~Level();
	void free();

	// Rendering
	void render() const;

private:

	void prepareArrays(ShaderProgram &program);
	void loadTileMap();
	void loadTile(char tile, int i, int j);

private:

	// TileMap
	glm::vec2 tileSize;
	glm::vec2 mapSizeInTiles;
	glm::vec2 mapSizeInPixels;
	vector<vector<Tile>> map;

	// OpenGL stuff
	GLuint vao[2];
	GLuint vbo[2];
	GLint posLocation[2], texCoordLocation[2];

	// World stuff
	int levelID;
	glm::vec3 size;
	Texture floor, wall;

};


#endif // _LEVEL_INCLUDE


