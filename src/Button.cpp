#include "Button.h"
#include "Level.h"
#include "GameScene.h"

Button::Button(Level* lev, ShaderProgram* prog, Model* rel, Model* pres, char type)
{
	level = lev;
	program = prog;
	models.push_back(rel);
	models.push_back(pres);
	mode = type;
}

void Button::collided()
{
	if (mode == Tile::pressedButton) {
		
		ivec2 currTilePos;
		for (int i = 0; i < (*switchableTiles).size(); ++i) {

			currTilePos = (*switchableTiles)[i];
			if (currTilePos != currTile) {
				char salu2 = level->getTile(currTilePos)->type;
				if (level->getTile(currTilePos)->type == Tile::switchable)
					level->loadTile(Tile::cube, currTilePos.y, currTilePos.x);

				else if (level->getTile(currTilePos)->type == Tile::cube)
					level->loadTile(Tile::switchable, currTilePos.y, currTilePos.x);
			}
		}
	}
}

void Button::trigger()
{
	if (mode == Tile::releasedButton) {
		mode = Tile::pressedButton;
		collided();
	}
	/*else
		mode = Level::releasedButton;*/
}

void Button::setVector(vector<ivec2>* vec)
{
	switchableTiles = vec;
}

void Button::render()
{
	glm::mat4 modelMatrix;
	glm::mat3 normalMatrix;

	int currModel = 0;
	if (mode == Tile::pressedButton)
		currModel = 1;

	float tileSize = level->getTileSize();
	float scaleFactor = tileSize / models[currModel]->getHeight();

	vec2 position = level->getTile(currTile)->coords;

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = translate(modelMatrix, vec3(position, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));
	modelMatrix = glm::translate(modelMatrix, -models[currModel]->getCenter());

	normalMatrix = glm::transpose(glm::inverse(glm::mat3(viewMatrix * modelMatrix)));

	program->use();
	program->setUniformMatrix4f("projection", projMatrix);
	program->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	program->setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	program->setUniformMatrix3f("normalmatrix", normalMatrix);

	models[currModel]->render(*(program));
}

void Button::setViewMatrix(const mat4& view)
{
	viewMatrix = view;
}

void Button::setProjMatrix(const mat4& proj)
{
	projMatrix = proj;
}

void Button::setTile(ivec2 tile)
{
	currTile = tile;
}

ivec2 Button::getTile()
{
	return currTile;
}
