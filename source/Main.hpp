#ifndef __MAIN_HPP
#define __MAIN_HPP

#define FRAMETIME 17 // Delay between updates (FPS = 1/FRAMETIME)
#define RADDEG 57.29577951308232088 // approx. 180/PI

#include <GL/glew.h>
#include <GL/glext.h>
#include <SDL2/SDL.h>
#include <cstdlib> 
#include "Player.hpp"
#include "GameMap.hpp"
#include "map/MapFileParser.hpp"
#include "util/audioplayer/AudioPlayer.hpp"
#include "Game.hpp"
#include "Window.hpp"

using namespace glPortal::engine;

void loop(Game & game, Window & window);
void draw();
void registerCallbacks();
void setup(int argc, char * argv[]);
void resize(int, int);
void resetKeyStates();

#endif
