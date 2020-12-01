#ifndef _SLIDE_INCLUDE
#define _SLIDE_INCLUDE

#include "Entity.h" 
using namespace std;
using namespace glm;

class Slide : public Entity
{
public:
	~Slide();
	Slide(Model* model, ShaderProgram* prog);

	void init() override;
	void update(int deltaTime) override;
	void render() override;

	void setSize(int tileSize, int orient);
	void setLimits(int head, int tail);

	// ESPERAR A AVANCES
	void trackPlayer();

	static constexpr int vertical = 0;
	static constexpr int horizontal = 1;

private:
	vec3 pos, posTrackedPlayer;
	ivec2 size, limits;
	float speed, modI, modJ;
	int tileSize, orientation;
	bool chasing;
};

#endif
