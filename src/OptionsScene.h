#ifndef _OPTIONSSCENE_INCLUDE
#define _OPTIONSSCENE_INCLUDE

#include "Scene.h"

using namespace std;
using namespace glm;

class OptionsScene : public Scene
{
public:

	void init() override;
	void render() override;
	void update(int deltaTime) override;

	void nextMenu();
	void prevMenu();

private:

	ShaderProgram* menuShaderProgram;
	Model* GAME_PAUSED;
	Model* EXIT_GAME;
};
#endif