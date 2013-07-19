#ifndef __MAIN_HPP
#define __MAIN_HPP

#define DEFAULT_WIDTH 1680
#define DEFAULT_HEIGHT 1050
#define FRAMETIME 17 // Delay between updates (FPS = 1/FRAMETIME)
#define FRAMETIME_SECONDS 0.017f // FRAMETIME in seconds. Used for updating game logic
#define RADDEG 57.29577951308232088 // 180/PI

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <cstdlib> 
#include "Player.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "Window.hpp"

// Main functions
void update(int);
void respawn();
void nextLevel();
void draw();
void registerCallbacks();
void setup(int *, char **);
// Callback functions
void mouse_moved(int, int);
void mouse_pressed(int, int, int, int);
void key_down(unsigned char, int, int);
void key_up(unsigned char, int , int);
void resize(int, int);
void window_status(int state);
void resetKeyStates();
int width, height;	
int mousex, mousey; 
bool keystates[256];
bool nmap_enabled;

Player player;
Map map;
Game game;
Window window;
int current_level;

#endif
