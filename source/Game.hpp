#ifndef __GAME_HPP
#define __GAME_HPP

#define FRAMETIME 17 // Delay between updates (FPS = 1/FRAMETIME)
#define FRAMETIME_SECONDS 0.017f // FRAMETIME in seconds. Used for updating game logic

#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/freeglut.h>

#include <vector>
#include <cstring>
#include <cstdlib> 

#include "Player.hpp"
#include "Window.hpp"
#include "engine/gui/GameScreen.hpp"
#include "engine/tools/Timer.hpp"
#include "GameMap.hpp"
#include "map/MapFileParser.hpp"

#include "engine/Box.hpp"

using namespace glPortal::engine;
using namespace glPortal::engine::tools;
using namespace glPortal::map;

class Game {
public:
  Game();
  void start();
  void respawn();
  void nextLevel();
  void drawPortals();
  void drawOverlay();
  void draw();
  void loadTextures();
  //Refactoring Methods
  void setPlayerMap(Player player, GameMap gameMap);
  void setWindow(Window &window);
  void setHeightWidth(int height, int width);
  void setFade(float fade);
  void setNmapEnabled(bool nmap_enabled);
  void setKey(unsigned char key);
  void unsetKey(unsigned char key);
  void unsetFade();
  void resetFade();
  void fadeOut();
  void setCurrentLevel(int current_level);
  GameMap getMap();
  Player getPlayer();
  void setPlayer(Player player);
  bool isPaused();
  void unpause();
  void pause();
  void togglePause();
  //Refactoring Methods End
private:
  int width, height;
  int mousex, mousey; 
  bool keystates[256];
  bool paused;	
  bool nmap_enabled; 
  float fade;  	
  Player player;
  GameMap gameMap;
  Window window;
  int current_level;
  Timer timer;
};

#endif
