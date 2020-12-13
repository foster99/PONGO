#ifndef _WALLCHEK_INCLUDE
#define _WALLCHEK_INCLUDE

#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

class Level;

class WallCheckpoint
{
public:
	
	WallCheckpoint(Level* lev, char t);

	void addWall(ivec2 tile);
	void addTrigger(ivec2 tile);

	bool triggerInRange(ivec2 tile);

	ivec2 getWall(int p);
	ivec2 getTrigger(int p);

	int getWallSize();
	int getTriggerSize();

	char getType();
private:
	
	Level* level;

	char type;

	vector<ivec2> wallTiles;
	vector<ivec2> triggerTiles;
};
#endif