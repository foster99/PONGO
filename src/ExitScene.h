#ifndef _EXITSCENE_INCLUDE
#define _EXITSCENE_INCLUDE

#include "Scene.h"

using namespace std;
using namespace glm;

class ExitScene : public Scene
{
public:

	void init() override;
	void render() override;
	void update(int deltaTime) override;

	static constexpr int Nmenus = 1;

private:

	int menu;

	ShaderProgram* menuShaderProgram;
	Model* EXIT_GAME;
};
#endif