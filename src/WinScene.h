#ifndef _WINSCENE_INCLUDE
#define _WINSCENE_INCLUDE

#include "Scene.h"

using namespace std;
using namespace glm;

class WinScene : public Scene
{
public:

	void init() override;
	void render() override;
	void update(int deltaTime) override;

	static constexpr int Nmenus = 2;
	static constexpr int pauseGame = 0;
	static constexpr int exitGame = 1;

private:

	float totalTime;
	float displacement;

	ShaderProgram* menuShaderProgram;
	Model* youWinModel;
};
#endif