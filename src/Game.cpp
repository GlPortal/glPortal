#include "Game.hpp"
#include "Box.hpp"
#include "Resources.hpp"



/**
 * Respawns the player after dying.
 */
void Game::respawn() {
  fade = 0.f;
  player.create(map.getStartX(), map.getStartY(), map.getStartZ());
}

/**
 * Loads the next level and respawns the player
 */
void Game::nextLevel() {
  current_level++;
  char filename[] = "maps/X.map";
  filename[5] = '0'+current_level; // Hackish but avoids using strings
  map.load(filename);
  respawn();
}

// This method is here for refactoring purposes 
// and can be removed once main is decluttered
void Game::setPlayerMap(Player player, Map map){
  this->map = map;
  this->player = player;
}

// This method is here for refactoring purposes 
// and can be removed once main is decluttered
void Game::setHeightWidth(int height, int width){
  this->width = width;
  this->height = height;
}


void Game::setFade(float fade){
  this->fade = fade;
}

void Game::draw() {
  // Clear depth buffer but not color buffer.
  // Every pixel is redraw every frame anyway.
  glClear(GL_DEPTH_BUFFER_BIT);
  // Load identity matrix
  glLoadIdentity();

  // Draw scene
  player.setView();
  drawPortals();
  map.draw(nmap_enabled);

  player.drawPortalOutlines();
  player.drawShots();
}

/**
 * Draws the inside of both portals as well as their oulines and stencils.
 */
void Game::drawPortals() {
  if(player.portalsActive()) {
    Portal *portals = player.getPortals();
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
      glTranslatef(portals[src].x, portals[src].y, portals[src].z);
      glRotatef(portals[src].getFromRotation(), 0,1,0);
      glRotatef(portals[dst].getToRotation(),   0,1,0);
      glTranslatef(-portals[dst].x, -portals[dst].y, -portals[dst].z);

      // Draw scene from portal view
      map.drawFromPortal(portals[dst], nmap_enabled);
      player.drawPortalOutlines();

      glPopMatrix();
    }
    glDisable(GL_STENCIL_TEST);

    // Draw portal stencils so portals wont be drawn over
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glClear(GL_DEPTH_BUFFER_BIT);
    player.drawPortalStencils();
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  }
}

/**
 * Draws all 2D overlay related.
 * Should be called after drawing all 3D.
 */
void Game::drawOverlay(bool paused) {
  // Switch to orthographic 2D projection
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0,width,height,0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);

  // If game is paused
  if(paused) {
    // Add dark tint to screen
    glColor4f(0.f, 0.f, 0.f, 0.5f);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glVertex2f(  0.f,    0.f);
    glVertex2f(  0.f, height);
    glVertex2f(width, height);
    glVertex2f(width,    0.f);
    glEnd();
    // Draw "Paused" message
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.f, 1.f, 1.f, 1.f);
    Resources::inst().bindTexture(TID_STRINGS);
    glBegin(GL_QUADS);
    glTexCoord2f(0.00f, 0.125f); glVertex2f(width/2-64, height/2-32);
    glTexCoord2f(0.00f, 0.250f); glVertex2f(width/2-64, height/2+32);
    glTexCoord2f(0.25f, 0.250f); glVertex2f(width/2+64, height/2+32);
    glTexCoord2f(0.25f, 0.125f); glVertex2f(width/2+64, height/2-32);
    glEnd();
    // If game is not paused
  } else {
    // Draw crosshair if player is alive
    if(player.getState() == PS_ALIVE) {
      Resources::inst().bindTexture(TID_CROSSHAIR);
      glColor4f(1.f, 1.f, 1.f, 1.f);
      glBegin(GL_QUADS);
      glTexCoord2f(0,0); glVertex2f(width/2-16, height/2-12);
      glTexCoord2f(0,1); glVertex2f(width/2-16, height/2+20);
      glTexCoord2f(1,1); glVertex2f(width/2+16, height/2+20);
      glTexCoord2f(1,0); glVertex2f(width/2+16, height/2-12);
      glEnd();
    }
    // Fade screen if player has died or won
    else if(player.getState() != PS_ALIVE) {
      glColor4f(0.f, 0.f, 0.f, fade);
      glDisable(GL_TEXTURE_2D);
      glBegin(GL_QUADS);
      glVertex2f(  0.f,    0.f);
      glVertex2f(  0.f, height);
      glVertex2f(width, height);
      glVertex2f(width,    0.f);
      glEnd();
      glEnable(GL_TEXTURE_2D);
      glColor3f(1.f, 1.f, 1.f);

      // Draw "Press return to respawn" message if dead
      if(player.getState() == PS_DYING) {
	Resources::inst().bindTexture(TID_STRINGS);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0.000f); glVertex2f(width/2-256, height/2-32);
	glTexCoord2f(0, 0.125f); glVertex2f(width/2-256, height/2+32);
	glTexCoord2f(1, 0.125f); glVertex2f(width/2+256, height/2+32);
	glTexCoord2f(1, 0.000f); glVertex2f(width/2+256, height/2-32);
	glEnd();
      }
      // Draw "Press return to continue" message if won
      else if(player.getState() == PS_WON) {
	Resources::inst().bindTexture(TID_STRINGS);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0.25f); glVertex2f(width/2-256, height/2-64);
	glTexCoord2f(0, 0.50f); glVertex2f(width/2-256, height/2+64);
	glTexCoord2f(1, 0.50f); glVertex2f(width/2+256, height/2+64);
	glTexCoord2f(1, 0.25f); glVertex2f(width/2+256, height/2-64);
	glEnd();
      }
    }
  }

  glEnable(GL_DEPTH_TEST);
  // Restore perspective projection matrix
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

