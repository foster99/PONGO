#ifndef _TRAIL_INCLUDE
#define _TRAIL_INCLUDE


#include "Ball.h"

using namespace std;
using namespace glm;

class GameScene;

class Trail
{
private:

	static constexpr int totalTime = 300;

	GameScene*		scene;
	vector<vec2>	positions;
	
	int timeToChange;
	bool selectedList;
	vector<vec2>	future_positions_1;
	vector<vec2>	future_positions_2;
	
	Model*			trailCube;
	ShaderProgram*	trailShader;
	mat4 viewMatrix;
	mat4 projMatrix;

public:

	Trail(GameScene* scene);

	void update(int deltaTime);
	void render();
	void clear();
	void addPoint(vec2 point);
	void commitPositions();
	bool ballCollidedWithTrail();
};
#endif


