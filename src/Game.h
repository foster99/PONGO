#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include <stack>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Sound.h"
#include "Scene.h"

typedef unsigned short Mode;

// Game is a singleton (a class with a single instance) that represents our whole application
class Game
{

private:

	// GAME STATUS CONTROL VARIABLES
	stack<Mode> modeHist;				// Defines the mode history (startmenu, playing, credits, ...)
	static constexpr Mode exitGame		= Mode(0);
	static constexpr Mode startMenu		= Mode(1);
	static constexpr Mode playing		= Mode(2);
	static constexpr Mode options		= Mode(3);

	static constexpr int ESC = 27;
	static constexpr int ENTER = 13;

public:

	Game() {}
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();

	// GAME STATUS CONTROL
	Mode currMode();
	void setMode(Mode newMode);
	void rollbackMode();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	// Key pressed management (for each MODE)
	void keyPressed_StartMenu(int key, bool specialKey);
	void keyPressed_playing(int key, bool specialKey);
	void keyPressed_options(int key, bool specialKey);

	// Key getters
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	
	// GodMode Public Functions
	bool isInGodMode();

	static constexpr int WINDOW_WIDTH = 640;
	static constexpr int WINDOW_HEIGHT = 480;

private:

	// GodMode Private Variables/Functions
	bool gameIsInGodMode;
	void toggleGodMode();

	// Scenes
	Scene gameScene;
	Scene optionsScene;
	Scene startMenuScene;

	void setUp_playing(int level);
	void setUp_options();
	void setUp_StartMenu();

	int mouseX, mouseY;


	bool keys[256], specialKeys[256]; 

};


#endif // _GAME_INCLUDE

/*
 
	CODIFICACION DE RAQUETAS:

	3   0123456789
	4   |       | 
	5	| ####  |		-->	H: 5 0 8
		|       |
		|       |


	HORIZONTAL SLIDE:
	I	J0	J1

	VERTICAL SLIDE:
	J	I0	I1

*/





