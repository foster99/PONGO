#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Level.h"


using namespace std;


Level *Level::createLevel(const glm::vec3 &levelSize, ShaderProgram &program, const string &floorFile, const string &wallFile)
{
	Level *level = new Level(levelSize, program, floorFile, wallFile);
	
	return level;
}


Level::Level(const glm::vec3 &levelSize, ShaderProgram &program, const string &floorFile, const string &wallFile)
{
	size = levelSize;
	if(!floor.loadFromFile(floorFile.c_str(), TEXTURE_PIXEL_FORMAT_RGB))
		cout << "Could not load floor texture!!!" << endl;
	if(!wall.loadFromFile(wallFile.c_str(), TEXTURE_PIXEL_FORMAT_RGB))
		cout << "Could not load wall texture!!!" << endl;
	prepareArrays(program);
	
	levelID = 1;
	loadTileMap();
}

Level::~Level()
{
}


void Level::render() const
{
	glEnable(GL_TEXTURE_2D);

	floor.use();
	glBindVertexArray(vao[0]);
	glEnableVertexAttribArray(posLocation[0]);
	glEnableVertexAttribArray(texCoordLocation[0]);
	glDrawArrays(GL_QUADS, 0, 4);

	wall.use();
	glBindVertexArray(vao[1]);
	glEnableVertexAttribArray(posLocation[1]);
	glEnableVertexAttribArray(texCoordLocation[1]);
	glDrawArrays(GL_QUADS, 0, 4 * 4);

	glDisable(GL_TEXTURE_2D);
}

void Level::free()
{
	glDeleteBuffers(1, &vbo[0]);
	glDeleteVertexArrays(1, &vao[0]);
	
	glDeleteBuffers(1, &vbo[1]);
	glDeleteVertexArrays(1, &vao[1]);
}

void Level::prepareArrays(ShaderProgram &program)
{
	vector<float> vertices;

	// Floor
	vertices.push_back(-size.x/2.f); vertices.push_back(-size.y/2.f); vertices.push_back(-size.z/2.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(-size.x/2.f);  vertices.push_back(-size.y/2.f); vertices.push_back(size.z/2.f);
	vertices.push_back(0.f); vertices.push_back(size.z/4.f);

	vertices.push_back(size.x/2.f);  vertices.push_back(-size.y/2.f); vertices.push_back(size.z/2.f);
	vertices.push_back(size.x/4.f); vertices.push_back(size.z/4.f);

	vertices.push_back(size.x/2.f);  vertices.push_back(-size.y/2.f); vertices.push_back(-size.z/2.f);
	vertices.push_back(size.x/4.f); vertices.push_back(0.f);

	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);
	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation[0] = program.bindVertexAttribute("position", 3, 5*sizeof(float), 0);
	texCoordLocation[0] = program.bindVertexAttribute("texCoord", 2, 5*sizeof(float), (void *)(3*sizeof(float)));

	vertices.clear();
	// Front wall
	vertices.push_back(-size.x/2.f); vertices.push_back(-size.y/2.f); vertices.push_back(-size.z/2.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(size.x/2.f); vertices.push_back(-size.y/2.f); vertices.push_back(-size.z/2.f);
	vertices.push_back(size.x/4.f); vertices.push_back(0.f);

	vertices.push_back(size.x/2.f); vertices.push_back(size.y/2.f); vertices.push_back(-size.z/2.f);
	vertices.push_back(size.x/4.f); vertices.push_back(size.y/4.f);

	vertices.push_back(-size.x/2.f); vertices.push_back(size.y/2.f); vertices.push_back(-size.z/2.f);
	vertices.push_back(0.f); vertices.push_back(size.y/4.f);

	// Back wall
	vertices.push_back(-size.x/2.f); vertices.push_back(-size.y/2.f); vertices.push_back(size.z/2.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(-size.x/2.f); vertices.push_back(size.y/2.f); vertices.push_back(size.z/2.f);
	vertices.push_back(0.f); vertices.push_back(size.y/4.f);

	vertices.push_back(size.x/2.f); vertices.push_back(size.y/2.f); vertices.push_back(size.z/2.f);
	vertices.push_back(size.x/4.f); vertices.push_back(size.y/4.f);

	vertices.push_back(size.x/2.f); vertices.push_back(-size.y/2.f); vertices.push_back(size.z/2.f);
	vertices.push_back(size.x/4.f); vertices.push_back(0.f);

	// Left wall
	vertices.push_back(-size.x/2.f); vertices.push_back(-size.y/2.f); vertices.push_back(-size.z/2.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(-size.x/2.f); vertices.push_back(size.y/2.f); vertices.push_back(-size.z/2.f);
	vertices.push_back(0.f); vertices.push_back(size.y/4.f);

	vertices.push_back(-size.x/2.f); vertices.push_back(size.y/2.f); vertices.push_back(size.z/2.f);
	vertices.push_back(size.z/4.f); vertices.push_back(size.y/4.f);

	vertices.push_back(-size.x/2.f); vertices.push_back(-size.y/2.f); vertices.push_back(size.z/2.f);
	vertices.push_back(size.z/4.f); vertices.push_back(0.f);

	// Right wall
	vertices.push_back(size.x/2.f); vertices.push_back(-size.y/2.f); vertices.push_back(-size.z/2.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(size.x/2.f); vertices.push_back(-size.y/2.f); vertices.push_back(size.z/2.f);
	vertices.push_back(size.z/4.f); vertices.push_back(0.f);

	vertices.push_back(size.x/2.f); vertices.push_back(size.y/2.f); vertices.push_back(size.z/2.f);
	vertices.push_back(size.z/4.f); vertices.push_back(size.y/4.f);

	vertices.push_back(size.x/2.f); vertices.push_back(size.y/2.f); vertices.push_back(-size.z/2.f);
	vertices.push_back(0.f); vertices.push_back(size.y/4.f);

	glGenVertexArrays(1, &vao[1]);
	glBindVertexArray(vao[1]);
	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 20 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation[1] = program.bindVertexAttribute("position", 3, 5*sizeof(float), 0);
	texCoordLocation[1] = program.bindVertexAttribute("texCoord", 2, 5*sizeof(float), (void *)(3*sizeof(float)));
}

bool diff(const std::string& line1, const std::string& line2)
{
	int length = line2.length();
	return line1.compare(0, length, line2) != 0;
}

void Level::loadTileMap()
{
	// CALCULAR EL PATH
	string levelFilePath;
	if (levelID < 10)	levelFilePath = "levels/LEVEL_0" + to_string(levelID) + ".txt";
	else				levelFilePath = "levels/LEVEL_"  + to_string(levelID) + ".txt";



	// INICIALIZAR EL LECTOR
	ifstream fin;
	string line;
	stringstream sstream;
	int i, j, i0, i1, j0, j1;
	fin.open(levelFilePath.c_str());
	if (!fin.is_open())	throw "Could not load the file.";



	// START LOADING LEVEL
	getline(fin, line);
	if (diff(line, "LEVEL")) throw "LEVEL NOT FOUND";
	getline(fin, line);	// Blank line



	// LOAD HORIZONTAL SLIDES
	getline(fin, line);	// Blank line
	getline(fin, line);
	if (diff(line, "START  - HORIZONTAL SLIDES"))
		throw std::string("Error parsing " + line).c_str();
	
	getline(fin, line);
	while (diff(line, "END    - HORIZONTAL SLIDES"))
	{
		sstream.str(line);
		sstream >> i >> j0 >> j1;
		//slides.push_back(Slide(Slide::Horizontal, i, j0, j1));
		getline(fin, line);
	}



	// LOAD VERTICAL SLIDES
	getline(fin, line); // Blank line
	getline(fin, line);
	if (diff(line, "START  - VERTICAL SLIDES"))
		throw std::string("Error parsing " + line).c_str();

	getline(fin, line);
	while (diff(line, "END    - VERTICAL SLIDES"))
	{
		sstream.str(line);
		sstream >> j >> i0 >> i1;
		//slides.push_back(Slide(Slide::Vertical, j, i0, i1));
		getline(fin, line);
	}
	

	// LOAD LEVEL PARAMETERS
	getline(fin, line);	// Blank line
	getline(fin, line);
	if (diff(line, "START  - TILEMAP"))
		throw std::string("Error parsing " + line).c_str();
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize;

	getline(fin, line);
	sstream.str(line);
	sstream >> chunkSize.y >> chunkSize.x;

	getline(fin, line);
	sstream.str(line);
	sstream >> mapSizeInChunks.y >> mapSizeInChunks.x;

	mapSizeInTiles = mapSizeInChunks * chunkSize;

	// INICIALIZAR Y LEER TILE MAP
	char tile;
	map = vector<vector<Tile>>(mapSizeInTiles.y, vector<Tile>(mapSizeInTiles.x));
	
	getline(fin, line); // Blank line
	for (int i = 0; i < mapSizeInTiles.y; i++)
	{
		for (int j = 0; j < mapSizeInTiles.x; j++)
		{
			fin.get(tile);
			loadTile(tile, i, j);
		}

		fin.get(tile);

		#ifndef _WIN32
			fin.get(tile);
		#endif
	}

	getline(fin, line); // Blank line
	getline(fin, line);
	if (diff(line, "END    - TILEMAP"))
		throw std::string("Error parsing " + line).c_str();

	fin.close();
}

void Level::loadTile(char tile, int i, int j)
{

}





























