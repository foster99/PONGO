#ifndef _BUTTON_INCLUDE
#define _BUTTON_INCLUDE

#include <glm/glm.hpp>
#include <vector>
#include "AssimpModel.h"
#include "Tile.h"
#include "ShaderProgram.h"

typedef AssimpModel Model;

using namespace std;
using namespace glm;

class Level;

class Button
{
public:
	Button(Level* lev, ShaderProgram* prog, Model* rel, Model* pres, char type);

	void collided();
	void trigger();


	void setTile(ivec2 tile);
	void setVector(vector<ivec2>* vec);

	ivec2 getTile();

	// RENDERING

	void render();

	void setViewMatrix(const mat4& view);
	void setProjMatrix(const mat4& proj);

private:
	Level* level;

	ivec2 currTile;
	vector<ivec2>* switchableTiles;
	char mode;
	vector<Model*> models;

	// RENDER UTILITY
	ShaderProgram* program;
	mat4 viewMatrix;
	mat4 projMatrix;
};
#endif
