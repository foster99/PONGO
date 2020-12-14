#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include <vector>
#include <stack>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Trail.h"
#include "Tile.h"
#include "World.h"
#include "Ball.h"
#include "Slide.h"
#include "SpawnCheckpoint.h"
#include "WallCheckpoint.h"
#include "Texture.h"
#include "ShaderProgram.h"

using namespace std;

class GameScene;

class Level : public World
{
public:

	static constexpr char verticalSlideChase		= '|';
	static constexpr char verticalSlideEscape		= '!';
	static constexpr char horizontalSlideChase		= '_';
	static constexpr char horizontalSlideEscape		= '-';

	static constexpr char blank						= '.';
	static constexpr char spawnPoint				= 'S';

	static constexpr char wallCheckVR = ')';
	static constexpr char wallCheckVL = '(';
	static constexpr char wallCheckHR = '}';
	static constexpr char wallCheckHL = '{';

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
	void renderSpawns() const;

	// Trail
	void setTrail(bool state);
	bool ballIsOnTrail();
	bool checkIfBallCollidedWithTrail();
	void addPointToTrail(vec2 pos);
	void openThisSnakeDoor(Tile* tile);
	vector<vector<Tile*>> snake_door_list;


	// TileMap Info
	int	  getTileSize();
	ivec2 getChunkSize();
	ivec2 getMapSizeInTiles();
	ivec2 getMapSizeInChunks();

	Tile* getTile(ivec2 coords);
	Tile* getTile(int i, int j);

	Tile* loadTile(char tile, int i, int j);

	// Chunks Info
	Tile* getFirstTileOfChunk(int k);

	// Slides
	Slide* whichSlideIsCollidingWithTheBall();
	vector<Slide*>& getSlides();

	// CheckPoints
	void checkIfBallCollidedWithACheckpoint();
	void addSpawnPoint(vec2 pos);
	void removeSpawnPoint();
	void checkTrigger(ivec2 tilePos);
	void finishWallChecks();

	//PURE TESTING REMOVE LATER
	void changeSpawnPoint();

private:

	GameScene* scene;

	// Trail things
	Trail* trail;
	bool onTrail;

	// TileMap
	void loadTileMap();
	void fillVerticalRight(WallCheckpoint* wall);
	void fillVerticalLeft(WallCheckpoint* wall);
	void fillHorizontalRight(WallCheckpoint* wall);
	void fillHorizontalLeft(WallCheckpoint* wall);
	//Tile* loadTile(char tile, int i, int j);

	int tileSize;
	ivec2 chunkSize;
	ivec2 mapSizeInTiles;
	ivec2 mapSizeInChunks;
	vector<vector<Tile>> map;
	vector<Tile*> firstTileOfChunk;

	vec2 startPoint;
	vec2 winPoint;

	// Game Models
	void loadModels();
	Model* cubeModel;
	Model* snakeModel;
	Model* voidCubeModel;
	Model* ropeModel;
	Model* pinchoModel;
	Model* bolaPinchoModel;
	Model* buttonONModel;
	Model* buttonOFFModel;

	void loadShaders();
	ShaderProgram* cubeShader;
	ShaderProgram* ropeShader;
	ShaderProgram* buttonShader;

	// World stuff
	int levelID;
	vec3 size;
	Texture floor, wall;
	stack<vec2> spawnPositions;

	// Entities
	vector<Slide*> slides;
	vector<SpawnCheckpoint*> spawns;
	vector<WallCheckpoint*> wallChecks;
};


#endif // _LEVEL_INCLUDE


