#ifndef _MENUSCENE_INCLUDE
#define _MENUSCENE_INCLUDE

#include "Scene.h"

class MenuScene : public Scene
{
public:

	void init() override;
	void render() override;
	void update(int deltaTime) override;

private:


};
#endif