 #ifndef SOUNDMANAGER_HPP
 #define SOUNDMANAGER_HPP
 
#include <SDL2/SDL_mixer.h>
#include <string>
#include <vector>

namespace glPortal {
	class SoundManager
	{
		public:
			SoundManager();
			~SoundManager();
			
			void PlayMusic(const std::string& filename);
			void PlaySound(const std::string& filename);
		private:
			Mix_Music *music;
			std::vector<Mix_Music*> sounds;
	};
}
 #endif
