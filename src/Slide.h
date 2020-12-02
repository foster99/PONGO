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

	void setPosition(vec2 position);
	void setSpeed(vec2 speed);
	void setDirection(vec2 direction);

	// ESPERAR A AVANCES
	void trackPlayer();

	static constexpr int vertical = 0;
	static constexpr int horizontal = 1;

private:
	vec3 ogPos, currentPos, posTrackedPlayer;
	ivec2 size, limits;
	float speed;
	int tileSize, orientation;
	bool chasing;

	void updateVertical(int deltaTime);
	void updateHorizontal(int deltaTime);
};

#endif
