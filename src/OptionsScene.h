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

	void restartOptions();
	int getMenu();
	void nextMenu();
	void prevMenu();

	static constexpr int Nmenus = 2;
	static constexpr int pauseGame = 0;
	static constexpr int exitGame = 1;

private:

	int menu;

	ShaderProgram* menuShaderProgram;
	Model* GAME_PAUSED;
	Model* EXIT_GAME;
	Model* ARROW_R;
	Model* ARROW_L;
};
#endif