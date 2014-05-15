#ifndef __GAME_HPP
#define __GAME_HPP

#include <GL/glew.h>
#include <GL/glext.h>

#include <vector>
#include <cstring>
#include <cstdlib> 

#include "Player.hpp"
#include "Window.hpp"
#include "engine/object/Model.hpp"
#include "engine/gui/GameScreen.hpp"
#include "engine/tools/Timer.hpp"
#include "GameMap.hpp"
#include "map/MapFileParser.hpp"
#include "engine/Box.hpp"
#include "engine/Environment.hpp"
#include "engine/renderer/GameMapRenderer.hpp"

#define RADDEG 57.29577951308232088 // 180/PI
#define DEGRAD 0.017453292519943296 // PI/180
#define KEY_BUFFER 256
#define PLAYER_MOVESPEED 330
#define GRAVITY 16.0
#define MAXSPEED 50.0
#define JUMPPOWER 7.0
#define JETPACK_ACC 24.0

using namespace glPortal::engine;
using namespace glPortal::engine::object;
using namespace glPortal::engine::tools;
using namespace glPortal::engine::renderer;
using namespace glPortal::map;
using namespace glPortal::util;

class Game {
public:
  Game();
  ~Game();
  static bool DEBUG;
  void start();
  void respawn();
  void nextLevel();
  void drawPortals();
  void drawOverlay();
  void draw();
  void setView();
  void update();
  void loadTextures();
  bool normalMapIsActive();
  //Input
  void mousePressed(int button);
  void setKey(SDL_Keysym);
  void unsetKey(SDL_Keysym);
  //Portals
  bool portalsActive();
  void drawShots();
  Portal *getPortals() { return portals; } /**< Gets the list of portals */
  Shot *getShots() { return shots; } /**< Gets the list of shots */
  //Refactoring Methods
  void setWindow(Window &window);
  bool isPaused();
  void unpause();
  void pause();
  void togglePause();
  //Refactoring Methods End
private:
  ConfigFileParser * config;
  GameMapRenderer  * mapRenderer;
  float sensitivity;
  bool keystates[KEY_BUFFER];
  bool paused;
  bool nmap_enabled;
  float fade;
  Player player;
  GameMap gameMap;
  Window window;
  Timer timer;
  Model *barrel;
  Portal portals[2];  /**< Array of player's portal */
  Shot shots[2];		/**< Array of shots */
  bool jetpack;
  int currentLevel;
  vector<string> mapList;
};

#endif
