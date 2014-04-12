#include <cmath>
#include "Portal.hpp"
#include "engine/Resources.hpp"
#include <iostream>
#include <cstdio>

/**
 * Places and activates the portal.
 * Coordinates will be rounded for better alignment with walls and floor.
 *
 * @param x New x-coordinate
 * @param y New y-coordinate
 * @param z New z-coordinate
 * @param dir New direction
 * @param gameMapThe current gameMap
 */
void Portal::place(float x, float y, float z, PORTAL_DIR dir, GameMap& gameMap) {
  float nx, ny, nz; // New coordinates
  nx = round(x);
  ny = round(y-0.25f)+0.25f;
  nz = round(z);

  // Calculate in front of portal
  Box bbox;
  if(dir == PD_FRONT) {
    bbox.set(nx-0.74f, ny-1.24f, nz+0.01f, nx+0.74f, ny+1.24f, nz+1.49f);
  } else if (dir == PD_BACK) {
    bbox.set(nx-0.74f, ny-1.24f, nz-1.49f, nx+0.74f, ny+1.24f, nz-0.01f);
  } else if (dir == PD_RIGHT) {
    bbox.set(nx+0.01f, ny-1.24f, nz-0.74f, nx+1.49f, ny+1.24f, nz+0.74f);
  } else if (dir == PD_LEFT) {
    bbox.set(nx-1.49f, ny-1.24f, nz-0.74f, nx-0.01f, ny+1.24f, nz+0.74f);
  }
  // Only place portal if nothing is in front of it
  if(gameMap.collidesWithWall(bbox) == false && gameMap.collidesWithAcid(bbox) == false) {
    this->position.x = nx;
    this->position.y = ny;
    this->position.z = nz;
    this->dir = dir;
    active = true;
  }
}

/**
 * Places the portal on a box based on the shot's position
 * when the collision occured.
 *
 * @param box The box to place portal on
 * @param hitx X-coordinate of shot-box collision
 * @param hity Y-coordinate of shot-box collision
 * @param hitz Z-coordinate of shot-box collision
 * @param gameMapThe current gameMap
 */
void Portal::placeOnBox(Box &box, float hitx, float hity, float hitz, GameMap& gameMap) {
  float dist[4];
  int min;

  // Calculate distance from shot to planes
  dist[0] = hitx - box.start.x; // Distance from left face to x
  dist[1] = box.end.x - hitx; // Distance from right face to x
  dist[2] = hitz - box.start.z; // Distance from back face to z
  dist[3] = box.end.z - hitz; // Distance from front face to z

  // Find smallest distance
  min = 0;	
  for(int i = 1; i < 4; i++) {
    if(dist[i] < dist[min]) {
      min = i;
    }
  }

  // Portal on the YZ-plane
  if(min <= 1) {
    // Make sure box is wide enough
    if(box.end.z - box.start.z < 2) return;

    // Left face
    if(min == 0) {
      place(box.start.x, hity, hitz, PD_LEFT, gameMap);
      // Right face
    } else {
      place(box.end.x, hity, hitz, PD_RIGHT, gameMap);
    }

    // Portal on the XY-plane
  } else {
    // Make sure box is wide enough
    if(box.end.x - box.start.x < 2.f) return;

    // Back face
    if(min == 2) {
      place(hitx, hity, box.start.z, PD_BACK, gameMap);
      // Front Face
    } else {
      place(hitx, hity, box.end.z, PD_FRONT, gameMap);
    }
  }
}

/**
 * Checks wheter a box is inside the portal
 *
 * @param box Box to check collision with
 *
 * @return True if part of the box is contained in the portal
 */
bool Portal::inPortal(Box &box) {
  if(!active) return false;

  if(dir == PD_RIGHT || dir == PD_LEFT) {
    if(box.start.z > position.z-0.75f && box.end.z < position.z+0.75f
       && box.start.x < position.x && box.end.x > position.x
       && box.start.y > position.y-1.25f && box.end.y < position.y+1.25f) {
      return true;
    }
  } else if(dir == PD_FRONT || dir == PD_BACK) {
    if(box.start.x > position.x-0.75f && box.end.x < position.x+0.75f
       && box.start.z < position.z && box.end.z > position.z
       && box.start.y > position.y-1.25f && box.end.y < position.y+1.25f) {
      return true;
    }
  }

  return false;
}

/**
 * Checks whether a point has passed through the portal
 *
 * @param r X-coordinate of the point to check
 * @param s Y-coordinate of the point to check
 * @param t Z-coordinate of the point to check
 */
bool Portal::throughPortal(float x, float y, float z) {
  float px = position.x;
  float py = position.y;
  float pz = position.z;
  switch(dir) {
  case PD_LEFT:
    if(x > px && x < px+0.2f && y > py-1.25f && y < py+1.25f && z < pz+0.75f && z > pz-0.75f)
      return true;
    break;
  case PD_RIGHT:
    if(x < px && x > px-0.2f && y > py-1.25f && y < py+1.25f && z < pz+0.75f && z > pz-0.75f)
      return true;
    break;
  case PD_FRONT:
    if(z < pz && z > pz-0.2f && y > py-1.25f && y < py+1.25f && x < px+0.75f && x > px-0.75f)
      return true;
    break;
  case PD_BACK:
    if(z > pz && z < pz+0.2f && y > py-1.25f && y < py+1.25f && x < px+0.75f && x > px-0.75f)
      return true;
    break;
  }
  return false;
}

/**
 * Disables the portal
 */
void Portal::disable() {
  active = false;
}

/**
 * Rotation from Z-axis to portal's view.
 *
 * @return Angle between Z-axis and portal's view in degrees.
 */
float Portal::getFromRotation() {
  switch(dir) {
  case PD_RIGHT: return  90.f;
  case PD_BACK:  return 180.f;
  case PD_LEFT:  return -90.f;
  case PD_FRONT:
  default:	   return 0.f;
  }
}

/**
 * Rotation from portal's view to Z-axis.
 *
 * @return Angle between portal's view and Z-axis in degrees.
 */
float Portal::getToRotation() {
  switch(dir) {
  case PD_RIGHT: return  90.f;
  case PD_FRONT: return 180.f;
  case PD_LEFT:  return -90.f;
  case PD_BACK:
  default:	   return 0.f;
  }
}

/**
 * Draws a round stencil in the portal.
 * Used for stencil buffer effects.
 */
void Portal::drawStencil() {
  glPushMatrix();

  glTranslatef(position.x, position.y, position.z);
  glRotatef(getFromRotation(), 0,1,0);
  Resources::inst().drawModel(MID_PORTAL_STENCIL);

  glPopMatrix();
}

/**
 * Draws the colored outline of a portal.
 *
 * @param color Color of the portal. Either PC_BLUE or PC_ORANGE
 */
void Portal::drawOutline(PORTAL_COLOR color) {
  glPushMatrix();

  glTranslatef(position.x, position.y, position.z);
  glRotatef(getFromRotation(), 0,1,0);

  //Portal material
  float portal_diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  float portal_emission[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, portal_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, portal_emission);

  // Bind blue of orange portal texture
  glEnable(GL_TEXTURE_2D);
    if(color == PC_BLUE) {
      Resources::inst().bindTexture(TID_BLUEPORTAL);
    }
    else {
      Resources::inst().bindTexture(TID_ORANGEPORTAL);
    }
    
    Resources::inst().drawModel(MID_PORTAL_OUTLINE);
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();
}

