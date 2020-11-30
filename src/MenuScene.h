#ifndef _MENUSCENE_INCLUDE
#define _MENUSCENE_INCLUDE

#include "Scene.h"

class MenuScene : public Scene
{
public:

	void init() override;
	void render() override;
	void update(int deltaTime) override;

	void nextMenu();
	void prevMenu();

private:

	ShaderProgram* menuShaderProgram;

	Model* PONGO;
	Model* PRESS;
	Model* SPACE;
	Model* TO;
	Model* PLAY;
	Model* UPC;
	Model* FIB;
};
#endif