#ifndef __MAIN_HPP
#define __MAIN_HPP

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define FRAMETIME 17 // Delay between updates (FPS = 1/FRAMETIME)
#define FRAMETIME_SECONDS 0.017f // FRAMETIME in seconds. Used for updating game logic
#define RADDEG 57.29577951308232088 // 180/PI

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include "Player.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "Window.hpp"

// Main functions
void update(int);
void respawn();
void nextLevel();
void draw();
void drawOverlay();
void drawPortals();
GLuint createTexture(const char *filename);
void setup(int *, char **);
// Callback functions
void mouse_moved(int, int);
void mouse_pressed(int, int, int, int);
void key_down(unsigned char, int, int);
void key_up(unsigned char, int , int);
void resize(int, int);
void window_status(int state);

int width, height;	
int mousex, mousey; 
bool keystates[256];
bool paused;	
bool nmap_enabled;
float fade;  	

Player player;
Map map;
Game game;
Window window;
int current_level;

#endif
