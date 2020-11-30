#ifndef _SLIDE_INCLUDE
#define _SLIDE_INCLUDE

#include "Entity.h" 
using namespace std;
using namespace glm;

class Slide : public Entity
{
public:
	~Slide();
	Slide(Model* model);

	void init() override;
	void update(int deltaTime) override;
	void render() override;

	// ESPERAR A AVANCES
	void trackPlayer();

private:
	vec3 pos, size, posTrackedPlayer;
	float speed, modI, modJ;
	bool chasing;
};

#endif
