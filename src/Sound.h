#include <string>
#include "irrKlang.h"


class Sound
{
	public:
		Sound();
		Sound(std::string filename_);
		Sound(std::string filename, bool loopSound);

		void setFile(std::string filename);
		void setMode(bool loop);
		void play();
		void drop();

	private:
		irrklang::ISoundEngine* engine;
		std::string path = "";
		bool loopSound = false;
		bool playing;

};
