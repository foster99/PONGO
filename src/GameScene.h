#ifndef _GAMESCENE_INCLUDE
#define _GAMESCENE_INCLUDE

#include "Scene.h"

class GameScene : public Scene
{
public:
	
	void init() override;
	void render() override;
	void update(int deltaTime) override;

private:


};
#endif

