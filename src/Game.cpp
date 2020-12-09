#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <iostream>
#include <random>

void Game::init()
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	modeHist.push(startMenu);

	loadSounds();

	// SCENE INITIALIZATION

	try {
		startMenuScene.init();
		optionsScene.init();
		gameScene.init();

		playBackgroundSong();
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

void Game::loadSounds()
{
	backgroundSong	= new Sound("backgroundSong.wav", true);
		
	buttonSound		= new Sound("button.wav", false);
	doorSound		= new Sound("door.wav", false);
	hitmarkerSound	= new Sound("hitmarker.wav", false);
	countdownSound	= new Sound("countdown.wav", false);
	deathSound		= new Sound("death.wav", false);

	levelSong[0]	= new Sound("levelSong_01.wav", true);
	levelSong[1]	= new Sound("levelSong_01.wav", true);
	levelSong[2]	= new Sound("levelSong_01.wav", true);
	levelSong[3]	= new Sound("levelSong_01.wav", true);
	levelSong[4]	= new Sound("levelSong_01.wav", true);

	gotaSound[0]	= new Sound("gota_00.wav", false);
	gotaSound[1]	= new Sound("gota_01.wav", false);
	gotaSound[2]	= new Sound("gota_02.wav", false);
	gotaSound[3]	= new Sound("gota_03.wav", false);
	gotaSound[4]	= new Sound("gota_04.wav", false);
	gotaSound[5]	= new Sound("gota_05.wav", false);
	gotaSound[6]	= new Sound("gota_06.wav", false);
	gotaSound[7]	= new Sound("gota_07.wav", false);
	gotaSound[8]	= new Sound("gota_08.wav", false);
	gotaSound[9]	= new Sound("gota_09.wav", false);
	gotaSound[10]	= new Sound("gota_10.wav", false);
	gotaSound[11]	= new Sound("gota_11.wav", false);
	gotaSound[12]	= new Sound("gota_12.wav", false);
	gotaSound[13]	= new Sound("gota_13.wav", false);
}

void Game::playBackgroundSong()
{
	backgroundSong->play();
}

void Game::playLevelSong()
{
	int level = gameScene.getLevelID();
	levelSong[level]->play();
}

void Game::playButtonSound()
{
	buttonSound->play();
}

void Game::playGotaSound()
{
	int min = 0, max = 13;
	int gota = min + (std::rand() % static_cast<int>(max - min + 1));

	gotaSound[gota]->play();
}

void Game::playDoorSound()
{
	doorSound->play();
}

void Game::playHitmarkerSound()
{
	hitmarkerSound->play();
}

void Game::playCountdownSound()
{
	countdownSound->play();
}

void Game::playDeathSound()
{
	deathSound->play();
}

void Game::stopBackgroundSong()
{
	backgroundSong->drop();
}

void Game::stopLevelSong()
{
	int level = gameScene.getLevelID();
	levelSong[level]->drop();
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
