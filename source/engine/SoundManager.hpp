 #ifndef SOUNDMANAGER_HPP
 #define SOUNDMANAGER_HPP
 
#include <SDL2/SDL_mixer.h>
#include <string>
#include <map>
#include <vector>
#include "../Player.hpp"
namespace glPortal {
	class SoundManager
	{
		public:
			enum SoundType
			{
				PRIMARY = 1,
				SECONDARY = 2,
			};

			struct SoundInfo
			{
				Mix_Chunk* chunk;
				SoundType type;
			};

			static void Init();
			static void Destroy();
			
			static void PlayMusic(const std::string& filename);
			static void PlaySound(const std::string& filename,Player* p,SoundType type);
			
			static void Update(Player* p);
		private:
			static Mix_Music *music;
			static std::map<int ,SoundInfo> sounds;
	};
}
 #endif
