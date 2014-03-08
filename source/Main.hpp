#ifndef __MAIN_HPP
#define __MAIN_HPP

#define FRAMETIME 17 // Delay between updates (FPS = 1/FRAMETIME)
#define RADDEG 57.29577951308232088 // approx. 180/PI

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <SDL2/SDL.h>
#include <cstdlib> 
#include "Player.hpp"
#include "GameMap.hpp"
#include "map/MapFileParser.hpp"
#include "Game.hpp"
#include "Window.hpp"

using namespace glPortal::engine;

// Main functions
void update(int);
void respawn();
void nextLevel();
void loop();
void draw();
void registerCallbacks();
void setup(int *, char **);
// Callback functions
void mouse_moved(int, int);
void mouse_pressed(SDL_MouseButtonEvent);
void key_down(SDL_KeyboardEvent);
void key_up(SDL_KeyboardEvent);
void resize(int, int);
void resetKeyStates();
int width, height;	
int mousex, mousey; 
bool keystates[256];
bool nmap_enabled = true;

Player player;
GameMap gameMap;
Game game;
Window window;
int current_level;

#endif
