#ifndef SOUND_HPP
#define SOUND_HPP

#include "FMOD/fmod.h"

namespace glPortal {

class Sound {
private:
  static bool on; //is sound on?
  static bool possible; //is it possible to play sound?
  static char * currentSound; //currently played sound
  //FMOD-specific stuff
  static FMOD_RESULT result;
  static FMOD_SYSTEM * fmodsystem;
  static FMOD_SOUND * sound;
  static FMOD_CHANNEL * channel;
public:
  static void init();

  //sound control
  static void setVolume (float v); //sets the actual playing sound's volume
  static void load (const char * filename); //loads a soundfile
  static void unload (void); //frees the sound object
  static void play (bool pause = false); //plays a sound (may be started paused; no argument for unpaused)

  //getters
  static bool getSound (void); //checks whether the sound is on

  //setters
  static void setPause (bool pause); //pause or unpause the sound
  static void setSound (bool sound); //set the sound on or off

  //toggles
  static void toggleSound (void); //toggles sound on and off
  static void togglePause (void); //toggle pause on/off
};

} /* namespace glPortal */

#endif /* SOUND_HPP */
