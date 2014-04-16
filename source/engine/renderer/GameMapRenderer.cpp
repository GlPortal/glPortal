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
  namespace engine {
    namespace renderer {
      GameMapRenderer::GameMapRenderer(GameMap* gameMap) {
        this->gameMap = gameMap;
      }

      void GameMapRenderer::render() {
        glEnable(GL_LIGHTING);
        // Update light position
        float posArray[4] = {0.f, 0.f, 0.f, 1.f};
        float colorArray[3] = {0.f, 0.f, 0.f};
        
        for(int i = 0; i < this->gameMap->lights.size(); i++) {
          Vector3f lightPos = this->gameMap->lights[i].position;
          posArray[0] = lightPos.x;
          posArray[1] = lightPos.y;
          posArray[2] = lightPos.z;
          Vector3f lightColor = this->gameMap->lights[i].color;
          colorArray[0] = lightColor.x;
          colorArray[1] = lightColor.y;
          colorArray[2] = lightColor.z;
          
          //Add more lights below to extend maximum
          if(i == 0) {
            glLightfv(GL_LIGHT0, GL_POSITION, posArray);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, colorArray);
          }
          if(i == 1) {
            glLightfv(GL_LIGHT1, GL_POSITION, posArray);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, colorArray);
          }
          if(i == 2) {
            glLightfv(GL_LIGHT2, GL_POSITION, posArray);
            glLightfv(GL_LIGHT2, GL_DIFFUSE, colorArray);
          }
          if(i == 3) {
            glLightfv(GL_LIGHT3, GL_POSITION, posArray);
            glLightfv(GL_LIGHT3, GL_DIFFUSE, colorArray);
          }
        }
        
        // Draw walls
        TEXTURE_ID current_type = TID_NONE;
        std::vector<Box>::iterator it;
        
        //Wall material
        float wall_diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float wall_emission[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, wall_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, wall_emission);
        
        glEnable(GL_TEXTURE_2D);
        for(it = this->gameMap->walls.begin(); it < this->gameMap->walls.end(); it++) {
          if(it->type != current_type) {
            current_type = it->type;
            Resources::inst().bindTexture(it->type);
          }
          glEnable(GL_TEXTURE_2D);
          this->gameMap->drawBox(*it);
          glDisable(GL_TEXTURE_2D);
        }
        glDisable(GL_TEXTURE_2D);
        
        std::vector<Model>::iterator objectsIterator;
        for(objectsIterator = this->gameMap->objects.begin(); objectsIterator < this->gameMap->objects.end(); objectsIterator++) {
          objectsIterator->draw();
        }

        //Acid material
        float acid_diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float acid_emission[4] = {0.0f, 1.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, acid_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, acid_emission);
        
        glEnable(GL_TEXTURE_2D);
        Resources::inst().bindTexture(TID_ACID);
        for(it = this->gameMap->acid.begin(); it < this->gameMap->acid.end(); it++) {
          this->gameMap->drawBox(*it);
        }
        glDisable(GL_TEXTURE_2D);

        // Draw cake
        drawCake();

        // Draw lamps
        for(int i = 0; i < this->gameMap->lights.size(); i++) {
          drawLamp(i);
        }
        glDisable(GL_LIGHTING);
      }

      /**
       * Draws only visible portion of map from a given portal
       *
       * @param portal Portal currently viewing through
       * @param nMapTrue if normal mapping is enabled
       */
      void GameMapRenderer::drawFromPortal(const Portal& portal) {
        glEnable(GL_LIGHTING);
        
        float lightArray[4] = {0.f, 0.f, 0.f, 1.f};
        
        for(int i = 0; i < this->gameMap->lights.size(); i++) {
          Vector3f lightPos = this->gameMap->lights[i].position;
          lightArray[0] = lightPos.x;
          lightArray[1] = lightPos.y;
          lightArray[2] = lightPos.z;
          
          //Add more lights below to extend maximum
          if(i == 0) {
            glLightfv(GL_LIGHT0, GL_POSITION, lightArray);
          }
          if(i == 1) {
            glLightfv(GL_LIGHT1, GL_POSITION, lightArray);
          }
          if(i == 2) {
            glLightfv(GL_LIGHT2, GL_POSITION, lightArray);
          }
          if(i == 3) {
            glLightfv(GL_LIGHT3, GL_POSITION, lightArray);
          }
        }

        // Draw walls
        std::vector<Box>::iterator it;
        TEXTURE_ID current_type = TID_NONE;
        
        //Wall material
        float wall_diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float wall_emission[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, wall_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, wall_emission);
        
        glEnable(GL_TEXTURE_2D);
        for(it = this->gameMap->walls.begin(); it < this->gameMap->walls.end(); it++) {
          if(portal.dir == PD_FRONT && it->end.z > portal.position.z
          || portal.dir == PD_BACK  && it->start.z < portal.position.z
          || portal.dir == PD_RIGHT && it->end.x > portal.position.x
          || portal.dir == PD_LEFT  && it->start.x < portal.position.x) {
            if(it->type != current_type) {
              current_type = it->type;
              Resources::inst().bindTexture(it->type);
            }
            this->gameMap->drawBox(*it);
          }
        }
        glDisable(GL_TEXTURE_2D);

        //Acid material
        float acid_diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float acid_emission[4] = {0.0f, 1.0f, 0.0f, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, acid_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, acid_emission);
        
        glEnable(GL_TEXTURE_2D);
        Resources::inst().bindTexture(TID_ACID);
        for(it = this->gameMap->acid.begin(); it < this->gameMap->acid.end(); it++) {
          if(portal.dir == PD_FRONT && it->end.z > portal.position.z
          || portal.dir == PD_BACK  && it->start.z < portal.position.z
          || portal.dir == PD_RIGHT && it->end.x > portal.position.x
          || portal.dir == PD_LEFT  && it->start.x < portal.position.x) {
            this->gameMap->drawBox(*it);
          }
        }
        glDisable(GL_TEXTURE_2D);

        // Draw cake
        drawCake();

        // Draw lamps
        for(int i = 0; i < this->gameMap->lights.size(); i++) {
          drawLamp(i);
        }
        glDisable(GL_LIGHTING);
      }

      /**
       * Draws the cake model at cake position
       */
      void GameMapRenderer::drawCake() {
        //Cake material
        float cake_diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float cake_emission[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cake_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, cake_emission);
        
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
      void GameMapRenderer::drawLamp(int light) {
        Vector3f position = this->gameMap->lights[light].position;
        
        //Lamp material
        float lamp_diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float lamp_emission[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lamp_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, lamp_emission);
        
        glPushMatrix();
          glTranslatef(position.x, position.y, position.z);
          Vector3f start(-0.8f, -0.025f, -0.2f);
          Vector3f end(0.8f, 0.025f, 0.2f);
          Box lightBox(start, end);
          this->gameMap->drawBox(lightBox);
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
                renderAvatar(player.position);
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
          glVertex3f(x + 0.3f, y + 0.7f, z - 0.3f);    // Top Right Of The Quad (Top)
          glVertex3f(x - 0.3f, y + 0.7f, z - 0.3f);    // Top Left Of The Quad (Top)
          glVertex3f(x - 0.3f, y + 0.7f, z + 0.3f);    // Bottom Left Of The Quad (Top)
          glVertex3f(x + 0.3f, y + 0.7f, z + 0.3f);    // Bottom Right Of The Quad (Top)
          
          glVertex3f(x + 0.3f, y - 0.7f, z + 0.3f);    // Top Right Of The Quad (Bottom)
          glVertex3f(x - 0.3f, y - 0.7f, z + 0.3f);    // Top Left Of The Quad (Bottom)
          glVertex3f(x - 0.3f, y - 0.7f, z - 0.3f);    // Bottom Left Of The Quad (Bottom)
          glVertex3f(x + 0.3f, y - 0.7f, z - 0.3f);    // Bottom Right Of The Quad (Bottom)
            
          glVertex3f(x + 0.3f, y + 0.7f, z + 0.3f);    // Top Right Of The Quad (Front)
          glVertex3f(x - 0.3f, y + 0.7f, z + 0.3f);    // Top Left Of The Quad (Front)
          glVertex3f(x - 0.3f, y - 0.7f, z + 0.3f);    // Bottom Left Of The Quad (Front)
          glVertex3f(x + 0.3f, y - 0.7f, z + 0.3f);    // Bottom Right Of The Quad (Front)
          
          glVertex3f(x + 0.3f, y - 0.7f, z - 0.3f);    // Top Right Of The Quad (Back)
          glVertex3f(x - 0.3f, y - 0.7f, z - 0.3f);    // Top Left Of The Quad (Back)
          glVertex3f(x - 0.3f, y + 0.7f, z - 0.3f);    // Bottom Left Of The Quad (Back)
          glVertex3f(x + 0.3f, y + 0.7f, z - 0.3f);    // Bottom Right Of The Quad (Back)
          
          glVertex3f(x - 0.3f, y + 0.7f, z + 0.3f);    // Top Right Of The Quad (Left)
          glVertex3f(x - 0.3f, y + 0.7f, z - 0.3f);    // Top Left Of The Quad (Left)
          glVertex3f(x - 0.3f, y - 0.7f, z - 0.3f);    // Bottom Left Of The Quad (Left)
          glVertex3f(x - 0.3f, y - 0.7f, z + 0.3f);    // Bottom Right Of The Quad (Left)
          
          glVertex3f(x + 0.3f, y + 0.7f, z - 0.3f);    // Top Right Of The Quad (Right)
          glVertex3f(x + 0.3f, y + 0.7f, z + 0.3f);    // Top Left Of The Quad (Right)
          glVertex3f(x + 0.3f, y - 0.7f, z + 0.3f);    // Bottom Left Of The Quad (Right)
          glVertex3f(x + 0.3f, y - 0.7f, z - 0.3f);    // Bottom Right Of The Quad (Right)
        glEnd();            // End Drawing The Cube
        glFlush();
      }
    }
  }
}
