 #ifndef SOUNDMANAGER_HPP
 #define SOUNDMANAGER_HPP
 
#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>
#include <vector>

namespace glPortal {
	class SoundManager
	{
		public:
			static void Init();
			static void Destroy();
			
			static void PlayMusic(const std::string& filename);
			static void PlaySound(const std::string& filename);
			
			static void Update();
		private:
			static Mix_Music *music;
			static std::map<int ,Mix_Chunk*> sounds;
	};
}
 #endif
