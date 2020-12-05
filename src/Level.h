#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Tile.h"
#include "World.h"
#include "Ball.h"
#include "Slide.h"
#include "Texture.h"
#include "ShaderProgram.h"

using namespace std;

// ARREGLAR COMPILACION + AJUSTAR CLASE LEVEL A LA HERENCIA

class GameScene;

class Level : public World
{
public:

	static constexpr char verticalSlide		= '|';
	static constexpr char horizontalSlide	= '-';
	static constexpr char blank				= '.';

	// Constructors
	Level(GameScene* scene, int id = 1);
	
	// Destructors
	~Level();
	void free() override;

	void update(int deltaTime);

	// Rendering
	void render() const override;
	void renderSlides() const;
	void renderTileMap() const;

	// TileMap Info
	int getTileSize();
	ivec2 getChunkSize();
	ivec2 getMapSizeInTiles();
	ivec2 getMapSizeInChunks();

	Tile* getTile(ivec2 coords);
	Tile* getTile(int i, int j);

	// Chunks Info
	Tile* getFirstTileOfChunk(int k);

	// Slides
	vector<Slide*>& getSlides();

private:

	GameScene* scene;

	// TileMap
	void loadTileMap();
	Tile* loadTile(char tile, int i, int j);

	int tileSize;
	ivec2 chunkSize;
	ivec2 mapSizeInTiles;
	ivec2 mapSizeInChunks;
	vector<vector<Tile>> map;
	vector<Tile*> firstTileOfChunk;

	// Game Models
	void loadModels();
	Model* cubeModel;

	void loadShaders();
	ShaderProgram* cubeShader;

	// World stuff
	int levelID;
	vec3 size;
	Texture floor, wall;

	// Entities
	vector<Slide*> slides;

};


#endif // _LEVEL_INCLUDE


