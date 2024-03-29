#include "WallCheckpoint.h"
#include "Level.h"
#include "Game.h"

WallCheckpoint::WallCheckpoint(Level* lev, char t)
{
	level = lev;
	type = t;
	triggered = false;

}

void WallCheckpoint::addWall(ivec2 tile)
{
	wallTiles.push_back(tile);
}

void WallCheckpoint::addTrigger(ivec2 tile)
{
	triggerTiles.push_back(tile);
}

bool WallCheckpoint::triggerInRange(ivec2 tile)
{
	if (!triggered) {
		
		bool found = false;
		int i = 0;
		int size = triggerTiles.size();

		while (!found && i < size) {
			if (tile == triggerTiles[i])
				found = true;
			i += 1;
		}

		if (!found)
			return false;

		size = wallTiles.size();

		for (i = 0; i < size; ++i) {
			level->loadTile('#', wallTiles[i].y, wallTiles[i].x);
			Game::instance().playPistonOutSound();
		}

		triggered = true;
		return true;
	}

	return false;
}

ivec2 WallCheckpoint::getWall(int p)
{
	if(p < wallTiles.size())
		return wallTiles[p];

	return ivec2(-1, -1);
}

ivec2 WallCheckpoint::getTrigger(int p)
{
	if (p < triggerTiles.size())
		return triggerTiles[p];

	return ivec2(-1, -1);
}

int WallCheckpoint::getWallSize()
{
	return wallTiles.size();
}

int WallCheckpoint::getTriggerSize()
{
	return triggerTiles.size();
}

char WallCheckpoint::getType()
{
	return type;
}

void WallCheckpoint::godTrigger()
{
	int size = wallTiles.size();

	for (int i = 0; i < size; ++i) {
		level->loadTile('#', wallTiles[i].y, wallTiles[i].x);
	}

}
