#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
using namespace std;



class Tile	{ private: int a; };

class Slide
{
	typedef int direction;

public:
	Slide(direction d, int fixed, int initial, int final) {

	}
	static constexpr direction Horizontal = 0;
	static constexpr direction Vertical = 0;

private: int a;
};



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
	int tileSize;
	glm::vec2 chunkSize;
	glm::vec2 mapSizeInTiles;
	glm::vec2 mapSizeInChunks;
	vector<vector<Tile>> map;

	// OpenGL stuff
	GLuint vao[2];
	GLuint vbo[2];
	GLint posLocation[2], texCoordLocation[2];

	// World stuff
	int levelID;
	glm::vec3 size;
	Texture floor, wall;

	// Entities
	vector<Slide> slides;

};


#endif // _LEVEL_INCLUDE


