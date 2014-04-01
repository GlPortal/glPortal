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
	drawCake();

	// Draw lamp
	drawLamp();
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
	drawCake();

	// Draw lamp
	drawLamp();
      }

      /**
       * Draws the cake model at cake position
       */
      void GameMapRenderer::drawCake() {
	glPushMatrix();
	glTranslatef(this->gameMap->endPos.x, this->gameMap->endPos.y, this->gameMap->endPos.z);
	glEnable(GL_TEXTURE_2D);
	Resources::inst().bindTexture(TID_CAKE);
	Resources::inst().drawModel(MID_CAKE);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
      }

      /**
       * Draws the lamp model at light position
       */
      void GameMapRenderer::drawLamp() {
	glPushMatrix();
	glTranslatef(this->gameMap->lightpos[0], this->gameMap->lightpos[1], this->gameMap->lightpos[2]);
	Box lightBox(-0.8f, 1.f, -0.2f, 0.8f, 0.95f, 0.2f);

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	this->gameMap->drawBox(lightBox);
	glEnd();
	glEnable(GL_TEXTURE_2D);

	glPopMatrix();
      }


      void GameMapRenderer::renderAvatar(Vector3f position){
	float x, y, z;
	x = position.x; y = position.y + 0.7f; z = position.z;
	glBegin(GL_QUADS);        // Draw The Cube Using quads
	glColor3f(0.0f,1.0f,0.0f);    // Color Blue
	glVertex3f(x + 0.3f, y + 0.7f, z - 0.3f);    // Top Right Of The Quad (Top)
	glVertex3f(x - 0.3f, y + 0.7f, z - 0.3f);    // Top Left Of The Quad (Top)
	glVertex3f(x - 0.3f, y + 0.7f, z + 0.3f);    // Bottom Left Of The Quad (Top)
	glVertex3f(x + 0.3f, y + 0.7f, z + 0.3f);    // Bottom Right Of The Quad (Top)
	glColor3f(1.0f,0.5f,0.0f);    // Color Orange
	glVertex3f(x + 0.3f, y - 0.7f, z + 0.3f);    // Top Right Of The Quad (Bottom)
	glVertex3f(x - 0.3f, y - 0.7f, z + 0.3f);    // Top Left Of The Quad (Bottom)
	glVertex3f(x - 0.3f, y - 0.7f, z - 0.3f);    // Bottom Left Of The Quad (Bottom)
	glVertex3f(x + 0.3f, y - 0.7f, z - 0.3f);    // Bottom Right Of The Quad (Bottom)
	glColor3f(1.0f,0.0f,0.0f);    // Color Red    
	glVertex3f(x + 0.3f, y + 0.7f, z + 0.3f);    // Top Right Of The Quad (Front)
	glVertex3f(x - 0.3f, y + 0.7f, z + 0.3f);    // Top Left Of The Quad (Front)
	glVertex3f(x - 0.3f, y - 0.7f, z + 0.3f);    // Bottom Left Of The Quad (Front)
	glVertex3f(x + 0.3f, y - 0.7f, z + 0.3f);    // Bottom Right Of The Quad (Front)
	glColor3f(1.0f,1.0f,0.0f);    // Color Yellow
	glVertex3f(x + 0.3f, y - 0.7f, z - 0.3f);    // Top Right Of The Quad (Back)
	glVertex3f(x - 0.3f, y - 0.7f, z - 0.3f);    // Top Left Of The Quad (Back)
	glVertex3f(x - 0.3f, y + 0.7f, z - 0.3f);    // Bottom Left Of The Quad (Back)
	glVertex3f(x + 0.3f, y + 0.7f, z - 0.3f);    // Bottom Right Of The Quad (Back)
	glColor3f(0.0f,0.0f,1.0f);    // Color Blue
	glVertex3f(x - 0.3f, y + 0.7f, z + 0.3f);    // Top Right Of The Quad (Left)
	glVertex3f(x - 0.3f, y + 0.7f, z - 0.3f);    // Top Left Of The Quad (Left)
	glVertex3f(x - 0.3f, y - 0.7f, z - 0.3f);    // Bottom Left Of The Quad (Left)
	glVertex3f(x - 0.3f, y - 0.7f, z + 0.3f);    // Bottom Right Of The Quad (Left)
	glColor3f(1.0f,0.0f,1.0f);    // Color Violet
	glVertex3f(x + 0.3f, y + 0.7f, z - 0.3f);    // Top Right Of The Quad (Right)
	glVertex3f(x + 0.3f, y + 0.7f, z + 0.3f);    // Top Left Of The Quad (Right)
	glVertex3f(x + 0.3f, y - 0.7f, z + 0.3f);    // Bottom Left Of The Quad (Right)
	glVertex3f(x + 0.3f, y - 0.7f, z - 0.3f);    // Bottom Right Of The Quad (Right)
	glColor3f(1.0f,1.0f,1.0f);    // Color Violet
	glEnd();            // End Drawing The Cube
	glFlush();
      }
    }
  }
}

