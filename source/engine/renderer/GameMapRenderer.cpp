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
      GameMapRenderer::GameMapRenderer(GameMap* gameMap) {
	      this->gameMap = gameMap;
      }

      void GameMapRenderer::render() {
	// Update light position
	Vector3f lightPos = this->gameMap->light.position;
	float lightArray[4] = {lightPos.x, lightPos.y, lightPos.z, 1.f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightArray);

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
	Vector3f lightPos = this->gameMap->light.position;
	float lightArray[3] = {lightPos.x, lightPos.y, lightPos.z};
	glLightfv(GL_LIGHT0, GL_POSITION, lightArray);

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
        Vector3f position = this->gameMap->light.position;
	      glPushMatrix();
	      glTranslatef(position.x, position.y, position.z);
	      Box lightBox(-0.8f, 1.f, -0.2f, 0.8f, 0.95f, 0.2f);

	      glDisable(GL_TEXTURE_2D);
	      glBegin(GL_QUADS);
	      this->gameMap->drawBox(lightBox);
	      glEnd();
	      glEnable(GL_TEXTURE_2D);

	      glPopMatrix();
      }


      /**
       * Draws the inside of both portals as well as their oulines and stencils.
       */
      void GameMapRenderer::renderPortals(Portal * portals, Player & player) {
	if((portals[0].active && portals[1].active)) {
	  glEnable(GL_STENCIL_TEST);
	  for(int i = 0; i < 2; i++) {
	    int src = i;		// Source portal index
	    int dst = (i+1)%2;  // Destination portal index

	    glPushMatrix();
	    // Always write to stencil buffer
	    glStencilFunc(GL_NEVER, 1, 0xFF);
	    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
	    glStencilMask(0xFF);
	    glClear(GL_STENCIL_BUFFER_BIT);

	    portals[src].drawStencil();

	    glClear(GL_DEPTH_BUFFER_BIT);
	    // Only pass stencil test if equal to 1
	    glStencilMask(0x00);
	    glStencilFunc(GL_EQUAL, 1, 0xFF);

	    // Move camera to portal view
	    glTranslatef(portals[src].position.x, portals[src].position.y, portals[src].position.z);
	    glRotatef(portals[src].getFromRotation(), 0,1,0);
	    glRotatef(portals[dst].getToRotation(),   0,1,0);
	    glTranslatef(-portals[dst].position.x, -portals[dst].position.y, -portals[dst].position.z);

	    // Draw scene from portal view
	    drawFromPortal(portals[dst]);
	    renderAvatar(*player.position);
	    if(portals[0].active) portals[0].drawOutline(PC_BLUE);
	    if(portals[1].active) portals[1].drawOutline(PC_ORANGE);
	    glPopMatrix();
	  }
	  glDisable(GL_STENCIL_TEST);

	  // Draw portal stencils so portals wont be drawn over
	  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	  glClear(GL_DEPTH_BUFFER_BIT);
	  portals[0].drawStencil();
	  portals[1].drawStencil();
	  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
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

