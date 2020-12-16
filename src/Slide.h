#ifndef _SLIDE_INCLUDE
#define _SLIDE_INCLUDE

#include "Entity.h"

using namespace std;
using namespace glm;

class Slide : public Entity
{
public:
	~Slide();
	Slide(GameScene* scene, Model* model, ShaderProgram* prog, Model* model2);

	void init(int tileSize, int orient, vec2 pos, vec2 dir, vec2 speed, int mode);
	void init() override;
	void update(int deltaTime) override;
	void render() override;

	void setSize(int tileSize, int orient);
	void setMode(int mode);
	void setPosition(vec2 position, int time=-1) override;
	void setSpeed(vec2 speed) override;
	void setDirection(vec2 direction) override;

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

	static constexpr float minmunTrackDistance = 2.5f;
	static constexpr float distanceError = 0.005f;

private:
	Model* trackModel;

	bool blocked = false;

	vec2 trackedPos;
	ivec2 size;
	int tileSize, orientation, mode;
};

#endif
