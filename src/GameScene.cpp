#include "GameScene.h"

void GameScene::init()
{
	this->Scene::init();
}

void GameScene::render()
{
	level->render();
}

void GameScene::update(int deltaTime)
{
	this->Scene::update(deltaTime);
}
