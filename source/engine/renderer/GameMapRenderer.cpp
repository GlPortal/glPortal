#include "GameMapRenderer.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include "../Resources.hpp"
#include "../StringConverter.hpp"
#include "../../Exception.hpp"
#include "../../Portal.hpp"


using namespace std;

namespace glPortal {
  namespace engine{
    namespace renderer{
      GameMapRenderer::GameMapRenderer(GameMap* gameMap){
	this->gameMap = gameMap;
      }

      void GameMapRenderer::render(){
	// Update light position
	glLightfv(GL_LIGHT0, GL_POSITION, this->gameMap->lightpos);

	// Enable normal mapping
	bool nMap = true;
	Resources::inst().enableProgram(nMap? PID_NMAP : PID_PPL);
  
	// Draw walls
	TEXTURE_ID current_type = TID_NONE;
	std::vector<Box>::iterator it;
  
	glBegin(GL_QUADS);
	for(it = this->gameMap->walls.begin(); it < this->gameMap->walls.end(); it++) {
	  if(it->type != current_type) {
	    glEnd(); // Batches boxes of same material
	    current_type = it->type;
	    Resources::inst().bindTexture(it->type);
	    glBegin(GL_QUADS);
	  }
	  this->gameMap->drawBox(*it);
	}
	glEnd(); 
  
	std::vector<Model>::iterator objectsIterator;
	glBegin(GL_QUADS);
	for(objectsIterator = this->gameMap->objects.begin(); objectsIterator < this->gameMap->objects.end(); objectsIterator++) {
	  objectsIterator->draw();
	}
	glEnd();
	
	// Draw acid waste
	Resources::inst().bindTexture(TID_ACID);
	glBegin(GL_QUADS);
	for(it = this->gameMap->acid.begin(); it < this->gameMap->acid.end(); it++) {
	  this->gameMap->drawBox(*it);
	}
	glEnd();

	Resources::inst().disableProgram();

	// Draw cake
	this->gameMap->drawCake();

	// Draw lamp
	this->gameMap->drawLamp();
      }

      /**
       * Draws only visible portion of map from a given portal
       *
       * @param portal Portal currently viewing through
       * @param nMapTrue if normal mapping is enabled
       */
      void GameMapRenderer::drawFromPortal(const Portal& portal) {
	// Update light position
	bool nMap = true;
	glLightfv(GL_LIGHT0, GL_POSITION, this->gameMap->lightpos);

	// Enable normal mapping
	Resources::inst().enableProgram(nMap? PID_NMAP : PID_PPL);

	// Draw walls
	std::vector<Box>::iterator it;
	TEXTURE_ID current_type = TID_NONE;
	glBegin(GL_QUADS);
	for(it = this->gameMap->walls.begin(); it < this->gameMap->walls.end(); it++) {
	  if(portal.dir == PD_FRONT && it->z2 > portal.position.z
	     || portal.dir == PD_BACK  && it->z1 < portal.position.z
						   || portal.dir == PD_RIGHT && it->x2 > portal.position.x
	     || portal.dir == PD_LEFT  && it->x1 < portal.position.x) {
	    if(it->type != current_type) {
	      glEnd();
	      current_type = it->type;
	      Resources::inst().bindTexture(it->type);
	      glBegin(GL_QUADS);
	    }
	    this->gameMap->drawBox(*it);
	  }
	}
	glEnd();

	// Draw acid waste
	Resources::inst().bindTexture(TID_ACID);
	glBegin(GL_QUADS);
	for(it = this->gameMap->acid.begin(); it < this->gameMap->acid.end(); it++) {
	  if(portal.dir == PD_FRONT && it->z2 > portal.position.z
	     || portal.dir == PD_BACK  && it->z1 < portal.position.z
						   || portal.dir == PD_RIGHT && it->x2 > portal.position.x
	     || portal.dir == PD_LEFT  && it->x1 < portal.position.x) {
	    this->gameMap->drawBox(*it);
	  }
	}
	glEnd();

	Resources::inst().disableProgram();

	// Draw cake
	this->gameMap->drawCake();

	// Draw lamp
	this->gameMap->drawLamp();
      }

    }
  }
}

