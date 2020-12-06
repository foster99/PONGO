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

	void goBackALittle();

	vector<vector<ivec2>> occupiedTiles() override;
	vec4 getBoundingBox() override;	// vec4 -> xmin, xmax, ymin, ymax

	bool isVertical();
	bool isHorizontal();

	// ESPERAR A AVANCES
	bool trackPlayerVertical();
	bool trackPlayerHorizontal();

	static constexpr int vertical = 0;
	static constexpr int horizontal = 1;

	static constexpr int chase = 0;
	static constexpr int escape = 1;

private:
	vec2 ogPos, playerPos;
	ivec2 size, limits;
	float speed;
	int tileSize, orientation, mode;
	bool tracked, initialized;

	void updateVertical(int deltaTime);
	void updateHorizontal(int deltaTime);
};

#endif
