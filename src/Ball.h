#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE

#include "Entity.h"

using namespace std;
using namespace glm;


class Ball : public Entity
{
private:

	// BALL atributtes

public:

	~Ball();
	Ball(Model* model);

	void init() override;
	void render() override;
	void update(int deltaTime) override;
};
#endif