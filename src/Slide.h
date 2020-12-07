#ifndef _SLIDE_INCLUDE
#define _SLIDE_INCLUDE

#include "Entity.h"

using namespace std;
using namespace glm;

class Slide : public Entity
{
public:
	~Slide();
	Slide(GameScene* scene, Model* model, ShaderProgram* prog);

	void init() override;
	void update(int deltaTime) override;
	void render() override;

	void setSize(int tileSize, int orient);
	void setLimits(int head, int tail);

	void setPosition(vec2 position, int time=-1) override;
	void setSpeed(vec2 speed) override;
	void setDirection(vec2 direction) override;

	bool isBlocked();
	void unlockNextMovement();
	void blockNextMovement();
	void goBackALittle();
	bool checkTrackedPos();


	vector<vector<ivec2>> occupiedTiles() override;
	vec4 getBoundingBox() override;	// vec4 -> xmin, xmax, ymin, ymax

	bool isVertical();
	bool isHorizontal();

	bool trackPlayerVertical();
	bool trackPlayerHorizontal();

	static constexpr int vertical = 0;
	static constexpr int horizontal = 1;

	static constexpr int chase = 0;
	static constexpr int escape = 1;

	static constexpr float minmunTrackDistance = 1.5;
	static constexpr float distanceError = 0.005;

private:


	bool blocked = false;

	vec2 trackedPos;
	ivec2 size;
	int tileSize, orientation, mode;
};

#endif
