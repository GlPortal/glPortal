#ifndef __GAME_HPP
#define __GAME_HPP

//#define DEFAULT_WIDTH 800
//#define DEFAULT_HEIGHT 600
#define FRAMETIME 17 // Delay between updates (FPS = 1/FRAMETIME)
#define FRAMETIME_SECONDS 0.017f // FRAMETIME in seconds. Used for updating game logic

#include <GL/glew.h>
#include <GL/glext.h>
#include "Player.hpp"
#include "Window.hpp"

#include <vector>
#include <cstring>
#include <GL/glut.h>
#include "Box.hpp"
#include "Map.hpp"

class Game {
public:
  void start();
  void respawn();
  void nextLevel();
  void drawPortals();
  void drawOverlay();
  void draw();
  void loadTextures();
  //Refactoring Methods
  void setPlayerMap(Player player, Map map);
  void setHeightWidth(int height, int width);
  void setFade(float fade);
  void setNmapEnabled(bool nmap_enabled);
  void setKey(unsigned char key);
  void unsetKey(unsigned char key);
  void unsetFade();
  void setCurrentLevel(int current_level);
  Map getMap();
  Player getPlayer();
  void setPlayer(Player player);
  bool isPaused();
  void unpause();
  void pause();
  void togglePause();
  //Refactoring Methods End
private:
  void renderBitmapString(int x, int y, char *string);
  int width, height;
  int mousex, mousey; 
  bool keystates[256];
  bool paused;	
  bool nmap_enabled; 
  float fade;  	
  Player player;
  Map map;
  Window window;
  int current_level;
};

#endif
