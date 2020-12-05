#include "World.h"
#include "Scene.h"

#define PI 3.14159f

World::World(const glm::vec3& worldSize)
	: viewMatrix(mat4(1)), projMatrix(mat4(1))
{
	this->worldSize = worldSize;
	defaultWorldShaderProgram = new ShaderProgram();

	Scene::loadShaders("texture", defaultWorldShaderProgram);

	loadModels();
	loadTexures();
	prepareArrays();
}

World::World(ShaderProgram* program, const glm::vec3& worldSize)
	: viewMatrix(mat4(1)), projMatrix(mat4(1))
{
	this->worldSize = worldSize;
	this->defaultWorldShaderProgram = program;

	loadModels();
	loadTexures();
	prepareArrays();
}

World::~World()
{
	if (defaultWorldShaderProgram != nullptr)
		delete defaultWorldShaderProgram;
}

void World::free()
{
	glDeleteBuffers(1, &vbo[0]);
	glDeleteVertexArrays(1, &vao[0]);

	glDeleteBuffers(1, &vbo[1]);
	glDeleteVertexArrays(1, &vao[1]);
}

void World::render() const
{
	float scaleFactor;
	vec3 centerModelBase;
	mat4 modelMatrix;
	mat3 normalMatrix;

	// Render environment

	defaultWorldShaderProgram->use();
	scaleFactor = 1.f / environment_model->getHeight();
	centerModelBase = environment_model->getCenter() - vec3(0.f, -environment_model->getHeight() / 2.f, 0.f);

	modelMatrix = mat4(1.0f);
	//modelMatrix = translate(modelMatrix, vec3(0.f, 0.5f * fabs(sinf(3.f * currentTime / 1000.f)), 0.f));
	//modelMatrix = translate(modelMatrix, vec3(0.f, 0.f, -5.f));
	modelMatrix = rotate(modelMatrix, PI/2.f, vec3(0.f, 1.f, 0.f));
	modelMatrix = scale(modelMatrix, scaleFactor * worldSize);
	modelMatrix = translate(modelMatrix, -environment_model->getCenter());

	normalMatrix = glm::transpose(inverse(mat3(viewMatrix * modelMatrix)));

	defaultWorldShaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	defaultWorldShaderProgram->setUniformMatrix4f("projection", mat4(projMatrix));
	defaultWorldShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	defaultWorldShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);

	environment_model->render(*(defaultWorldShaderProgram));


	// Render Origin
	if (renderOrigin)
	{
		defaultWorldShaderProgram->use();
		scaleFactor = 0.5f / origin->getHeight();

		modelMatrix = mat4(1.0f);
		modelMatrix = scale(modelMatrix, vec3(scaleFactor));
		modelMatrix = translate(modelMatrix, -origin->getCenter());

		normalMatrix = transpose(inverse(mat3(viewMatrix * modelMatrix)));

		defaultWorldShaderProgram->setUniform4f("color", 1.0f, 0.0f, 0.0f, 1.0f);
		defaultWorldShaderProgram->setUniformMatrix4f("projection", mat4(projMatrix));
		defaultWorldShaderProgram->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
		defaultWorldShaderProgram->setUniformMatrix3f("normalmatrix", normalMatrix);

		
		origin->render(*(defaultWorldShaderProgram));
	}

	// SUPUESTAMENTE, LOS QUADS XD

	//glEnable(GL_TEXTURE_2D);

	//floor.use();
	//glBindVertexArray(vao[0]);
	//glEnableVertexAttribArray(posLocation[0]);
	//glEnableVertexAttribArray(texCoordLocation[0]);
	//glDrawArrays(GL_QUADS, 0, 4);

	//wall.use();
	//glBindVertexArray(vao[1]);
	//glEnableVertexAttribArray(posLocation[1]);
	//glEnableVertexAttribArray(texCoordLocation[1]);
	//glDrawArrays(GL_QUADS, 0, 4 * 4);

	//glDisable(GL_TEXTURE_2D);
}

ShaderProgram* World::getShader()
{
	return defaultWorldShaderProgram;
}

void World::setProjMatrix(const mat4& PM)
{
	projMatrix = PM;
}

void World::setViewMatrix(const mat4& VM)
{
	viewMatrix = VM;
}

void World::setCurrentTime(float time)
{
	currentTime = time;
}

void World::loadModels()
{
	environment_model = new Model();
	environment_model->loadFromFile("models/voidCube_guide.obj", (*defaultWorldShaderProgram));

	origin = new Model();
	origin->loadFromFile("models/sphere.obj", (*defaultWorldShaderProgram));
}

void World::loadTexures()
{
	const string floorTexureFile = "images/roof.png";
	const string wallTextureFile = "images/wall.png";

	if (!floor.loadFromFile(floorTexureFile.c_str(), TEXTURE_PIXEL_FORMAT_RGB))
		throw "Could not load floor texture!!!";

	if (!wall.loadFromFile(wallTextureFile.c_str(), TEXTURE_PIXEL_FORMAT_RGB))
		throw "Could not load wall texture!!!";
}

void World::prepareArrays()
{
	vector<float> vertices;

	// Floor
	vertices.push_back(-worldSize.x / 2.f);  vertices.push_back(-worldSize.y / 2.f); vertices.push_back(-worldSize.z / 2.f);
	vertices.push_back(0.f);            vertices.push_back(0.f);

	vertices.push_back(-worldSize.x / 2.f);  vertices.push_back(-worldSize.y / 2.f); vertices.push_back(worldSize.z / 2.f);
	vertices.push_back(0.f);            vertices.push_back(worldSize.z / 4.f);

	vertices.push_back(worldSize.x / 2.f);   vertices.push_back(-worldSize.y / 2.f); vertices.push_back(worldSize.z / 2.f);
	vertices.push_back(worldSize.x / 4.f);   vertices.push_back(worldSize.z / 4.f);

	vertices.push_back(worldSize.x / 2.f);   vertices.push_back(-worldSize.y / 2.f); vertices.push_back(-worldSize.z / 2.f);
	vertices.push_back(worldSize.x / 4.f);   vertices.push_back(0.f);

	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);
	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation[0] = defaultWorldShaderProgram->bindVertexAttribute("position", 3, 5 * sizeof(float), 0);
	texCoordLocation[0] = defaultWorldShaderProgram->bindVertexAttribute("texCoord", 2, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	vertices.clear();
	// Front wall
	vertices.push_back(-worldSize.x / 2.f); vertices.push_back(-worldSize.y / 2.f); vertices.push_back(-worldSize.z / 2.f);
	vertices.push_back(0.f);           vertices.push_back(0.f);

	vertices.push_back(worldSize.x / 2.f);  vertices.push_back(-worldSize.y / 2.f); vertices.push_back(-worldSize.z / 2.f);
	vertices.push_back(worldSize.x / 4.f);  vertices.push_back(0.f);

	vertices.push_back(worldSize.x / 2.f);  vertices.push_back(worldSize.y / 2.f);  vertices.push_back(-worldSize.z / 2.f);
	vertices.push_back(worldSize.x / 4.f);  vertices.push_back(worldSize.y / 4.f);

	vertices.push_back(-worldSize.x / 2.f); vertices.push_back(worldSize.y / 2.f);  vertices.push_back(-worldSize.z / 2.f);
	vertices.push_back(0.f);           vertices.push_back(worldSize.y / 4.f);

	// Back wall
	vertices.push_back(-worldSize.x / 2.f); vertices.push_back(-worldSize.y / 2.f); vertices.push_back(worldSize.z / 2.f);
	vertices.push_back(0.f);           vertices.push_back(0.f);

	vertices.push_back(-worldSize.x / 2.f); vertices.push_back(worldSize.y / 2.f);  vertices.push_back(worldSize.z / 2.f);
	vertices.push_back(0.f);           vertices.push_back(worldSize.y / 4.f);

	vertices.push_back(worldSize.x / 2.f);  vertices.push_back(worldSize.y / 2.f);  vertices.push_back(worldSize.z / 2.f);
	vertices.push_back(worldSize.x / 4.f);  vertices.push_back(worldSize.y / 4.f);

	vertices.push_back(worldSize.x / 2.f);  vertices.push_back(-worldSize.y / 2.f); vertices.push_back(worldSize.z / 2.f);
	vertices.push_back(worldSize.x / 4.f);  vertices.push_back(0.f);

	// Left wall
	vertices.push_back(-worldSize.x / 2.f); vertices.push_back(-worldSize.y / 2.f); vertices.push_back(-worldSize.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(-worldSize.x / 2.f); vertices.push_back(worldSize.y / 2.f); vertices.push_back(-worldSize.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(worldSize.y / 4.f);

	vertices.push_back(-worldSize.x / 2.f); vertices.push_back(worldSize.y / 2.f); vertices.push_back(worldSize.z / 2.f);
	vertices.push_back(worldSize.z / 4.f); vertices.push_back(worldSize.y / 4.f);

	vertices.push_back(-worldSize.x / 2.f); vertices.push_back(-worldSize.y / 2.f); vertices.push_back(worldSize.z / 2.f);
	vertices.push_back(worldSize.z / 4.f); vertices.push_back(0.f);

	// Right wall
	vertices.push_back(worldSize.x / 2.f); vertices.push_back(-worldSize.y / 2.f); vertices.push_back(-worldSize.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(worldSize.x / 2.f); vertices.push_back(-worldSize.y / 2.f); vertices.push_back(worldSize.z / 2.f);
	vertices.push_back(worldSize.z / 4.f); vertices.push_back(0.f);

	vertices.push_back(worldSize.x / 2.f); vertices.push_back(worldSize.y / 2.f); vertices.push_back(worldSize.z / 2.f);
	vertices.push_back(worldSize.z / 4.f); vertices.push_back(worldSize.y / 4.f);

	vertices.push_back(worldSize.x / 2.f); vertices.push_back(worldSize.y / 2.f); vertices.push_back(-worldSize.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(worldSize.y / 4.f);

	glGenVertexArrays(1, &vao[1]);
	glBindVertexArray(vao[1]);
	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 20 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation[1] = defaultWorldShaderProgram->bindVertexAttribute("position", 3, 5 * sizeof(float), 0);
	texCoordLocation[1] = defaultWorldShaderProgram->bindVertexAttribute("texCoord", 2, 5 * sizeof(float), (void*)(3 * sizeof(float)));

}