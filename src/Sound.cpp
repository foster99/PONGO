#include "Sound.h"
#include "irrKlang.h"

Sound::Sound()
{
	engine = NULL;
	playing = false;
	engine = irrklang::createIrrKlangDevice();
}

Sound::Sound(std::string filename)
{
	engine = irrklang::createIrrKlangDevice();
	this->setFile(filename);
}

Sound::Sound(std::string filename, bool loopSound)
{
	engine = irrklang::createIrrKlangDevice();
	this->setFile(filename);
	this->setMode(loopSound);
	playing = false;
}

void Sound::setFile(std::string filename)
{
	this->path = "./sounds/" + filename;
}

void Sound::setMode(bool loopSound)
{
	this->loopSound = loopSound;
}

void Sound::play()
{
	if ((loopSound && !playing) || !loopSound) {
		if (!engine) engine = irrklang::createIrrKlangDevice();
		engine->play2D(path.c_str(), loopSound);
		playing = true;
	}
}

void Sound::drop()
{
	if (playing) {
		engine->drop();
		engine = NULL;
		playing = false;
	}
}
