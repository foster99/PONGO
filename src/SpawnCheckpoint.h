#ifndef _SPAWN_INCLUDE
#define _SPAWN_INCLUDE

#include "Entity.h"
#include "ParticleSystem.h"

class SpawnCheckpoint : public Entity
{
public:
	~SpawnCheckpoint();
	SpawnCheckpoint(GameScene* scene, Model* model, ShaderProgram* prog);

	void init() override;
	void update(int deltaTime) override;
	void render() override;

	void setPosition(vec2 position, int time = -1) override;
	void setSpeed(vec2 speed) override;
	void setDirection(vec2 direction) override;
	void setSecondModel(Model* mod);

	vector<vector<ivec2>> occupiedTiles() override;
	vec4 getBoundingBox() override;	// vec4 -> xmin, xmax, ymin, ymax

	void collided();
	void spawnParticles();

	void noRender();
private:
	bool renderable;
	bool triggered;
	int currentTime;
	ParticleSystem* particles;
	Model* secondModel;
};

#endif
