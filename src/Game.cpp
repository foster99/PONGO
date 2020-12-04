#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <iostream>

void Game::init()
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	modeHist.push(startMenu);

	// TODO: LOAD SOUNDS

	// SCENE INITIALIZATION

	try {
		startMenuScene.init();
		optionsScene.init();
		gameScene.init();
	}
	catch (const char* msg)
	{
		int i = 0; // jaja mira lo que hay en el mensaje xddddd. 
	}
}

bool Game::update(int deltaTime)
{
	// get GOD MODE

	updateFreeCamera();

	switch (currMode())
	{
	case startMenu:
		startMenuScene.update(deltaTime);
		break;

	case playing:
		gameScene.update(deltaTime);
		break;

	case options:
		optionsScene.update(deltaTime);
		break;

	case exitGame: break;
	}

	return currMode() != exitGame;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	switch (currMode())
	{
	case startMenu:
		startMenuScene.render();
		break;

	case playing:
		gameScene.render();
		break;

	case options:
		optionsScene.render();
		break;

	case exitGame: break;
	}
}



// Input callback methods

void Game::keyPressed(int key)
{

	// Camera selection
	switch (key - '0')
	{
	case Scene::fpsCamera:
	case Scene::fixedCamera_01:
	case Scene::fixedCamera_02:
	case Scene::fixedCamera_03:
	case Scene::fixedCamera_04:
	case Scene::fixedCamera_05:
		switch (currMode())
		{
		case startMenu:	startMenuScene.setCamera(key - '0');	break;
		case playing:	gameScene.setCamera(key - '0');			break;
		case options:	optionsScene.setCamera(key - '0');		break;
		}
		break;

	default: break;
	}


	switch (currMode())
	{
	case startMenu:		keyPressed_StartMenu(key, false);	break;
	case playing:		keyPressed_playing(key, false);		break;
	case options:		keyPressed_options(key, false);		break;
	default:												break;
	}

	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	switch (currMode())
	{
	case startMenu:		keyPressed_StartMenu(key, true);	break;
	case playing:		keyPressed_playing(key, true);		break;
	case options:		keyPressed_options(key, true);		break;
	default:												break;
	}

	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}



// Key pressed management (for each MODE)

void Game::keyPressed_StartMenu(int key, bool specialKey)
{
	if (!specialKey)
	{
		switch (key)
		{
		case ESC:	setMode(options);	break;
		case 'e':	setMode(exitGame);	break;
		case ENTER:
		case ' ':	setMode(playing);	break;
		default:						break;
		}
	}
	else if (specialKey)
	{
		switch (key)
		{
		case GLUT_KEY_RIGHT:	startMenuScene.nextMenu();	break;
		case GLUT_KEY_LEFT:		startMenuScene.prevMenu();	break;
		default:											break;
		}
	}
}

void Game::keyPressed_playing(int key, bool specialKey)
{
	if (!specialKey)
	{
		switch (key)
		{
		case ESC:	setMode(options);				break;
		case 'G':
		case 'g':	toggleGodMode();				break;
		case ' ':	gameScene.playerPressedSpace(); break;
		case 'm':	gameScene.addCube();			break;
		default:	break;
		}
	}
	else if (specialKey)
	{
		switch (key)
		{
		case GLUT_KEY_UP:	// nextMode();		break
		case GLUT_KEY_DOWN:	// previousMode();	break;
		default:								break;
		}
	}
}

void Game::keyPressed_options(int key, bool specialKey)
{
	if (!specialKey)
	{
		switch (key)
		{
		case ESC:				rollbackMode();		break;
		case 'e':				setMode(startMenu);	break;
		case ENTER:
			//switch (optionsScene.getCurrTex())
			//{
			//case 0:	setMode(credits);		break;
			//case 1:	setMode(instructions);	break;
			//case 2:	setMode(startMenu);		break;
			//default:						break;
			//}
			//
			// <Play options sounds>
			break;
		default:	break;
		}
	}
	else if (specialKey)
	{
		switch (key)
		{
		case GLUT_KEY_RIGHT:	optionsScene.nextMenu();	break;
		case GLUT_KEY_LEFT:		optionsScene.prevMenu();	break;
		default:											break;
		}
	}
}



// Key getters

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}



// GOD MODE FUNCTIONS

bool Game::isInGodMode()
{
	return gameIsInGodMode;
}

void Game::toggleGodMode()
{
	gameIsInGodMode = !gameIsInGodMode;
}



// SCENE SET UP FUNCTIONS

void Game::setUp_playing(int level)
{
	/*SET UP OPTIONS SCENE*/
}

void Game::setUp_options()
{
	/*SET UP OPTIONS SCENE*/
}

void Game::setUp_StartMenu()
{
	/*SET UP OPTIONS SCENE*/
}

void Game::updateFreeCamera()
{
	Scene* scene;

	switch (currMode())
	{
	case startMenu: scene = &startMenuScene;	break;
	case playing:	scene = &gameScene;			break;
	case options:	scene = &optionsScene;		break;
	}

	if (scene->cameraIsFree())
	{
		scene->setMouseAngles(mouseX, mouseY);
		glutWarpPointer(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
		mouseX = WINDOW_WIDTH / 2;
		mouseY = WINDOW_HEIGHT / 2;
	}
}



// GAME STATUS CONTROL

Mode Game::currMode()
{
	return modeHist.top();
}

void Game::setMode(Mode newMode)
{
	if (currMode() == startMenu && newMode == playing) {}
		// aqui falta empesar el juego

	if (newMode == startMenu)
		modeHist = {};

	modeHist.push(newMode);
}

void Game::rollbackMode()
{
	if (!modeHist.empty()) modeHist.pop();
}
