#ifndef _WORLD_INCLUDE
#define _WORLD_INCLUDE


#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AssimpModel.h"
using namespace std;
using namespace glm;


class World
{
public:
	
	typedef AssimpModel Model;
	static constexpr bool  renderOrigin		= false;
	static constexpr float defaultWorldSize = 200.f;

	// Constructors
	World(const vec3& worldSize = vec3(defaultWorldSize));
	World(ShaderProgram* program, const vec3& worldSize = vec3(defaultWorldSize));

	// Destructors
	~World();
	virtual void free();

	// Rendering
	virtual void render() const;
	ShaderProgram* getShader();
	
	void setProjMatrix(const mat4 &PM);
	void setViewMatrix(const mat4 &VM);
	void setCurrentTime(float time);

protected:
	
	ShaderProgram* defaultWorldShaderProgram;
	mat4& viewMatrix;
	mat4& projMatrix;

private:

	void loadModels();
	void loadTexures();
	void prepareArrays();

	float currentTime;

	// OpenGL stuff
	GLuint vao[2];
	GLuint vbo[2];
	GLint posLocation[2], texCoordLocation[2];



	// World stuff
	vec3 worldSize;
	Texture floor, wall;
	
	Model* environment_model;
	Model* origin;
};
#endif
