#include <cmath>
#include <GL/glut.h>
#include "Portal.hpp"
#include "engine/Resources.hpp"

/**
 * Places and activates the portal.
 * Coordinates will be rounded for better alignment with walls and floor.
 *
 * @param x New x-coordinate
 * @param y New y-coordinate
 * @param z New z-coordinate
 * @param dir New direction
 * @param map The current map
 */
#include <cstdio>
void Portal::place(float x, float y, float z, PORTAL_DIR dir, Map& map) {
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
  if(map.collidesWithWall(bbox) == false && map.collidesWithAcid(bbox) == false) {
    this->x = nx;
    this->y = ny;
    this->z = nz;
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
 * @param map The current map
 */
void Portal::placeOnBox(Box &box, float hitx, float hity, float hitz, Map& map) {
  float dist[4];
  int min;

  // Calculate distance from shot to planes
  dist[0] = hitx - box.x1; // Distance from left face to x
  dist[1] = box.x2 - hitx; // Distance from right face to x
  dist[2] = hitz - box.z1; // Distance from back face to z
  dist[3] = box.z2 - hitz; // Distance from front face to z

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
    if(box.z2 - box.z1 < 2) return;

    // Left face
    if(min == 0) {
      place(box.x1, hity, hitz, PD_LEFT, map);
      // Right face
    } else {
      place(box.x2, hity, hitz, PD_RIGHT, map);
    }

    // Portal on the XY-plane
  } else {
    // Make sure box is wide enough
    if(box.x2 - box.x1 < 2.f) return;

    // Back face
    if(min == 2) {
      place(hitx, hity, box.z1, PD_BACK, map);
      // Front Face
    } else {
      place(hitx, hity, box.z2, PD_FRONT, map);
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
    if(box.z1 > z-0.75f && box.z2 < z+0.75f
       && box.x1 < x && box.x2 > x
       && box.y1 > y-1.25f && box.y2 < y+1.25f) {
      return true;
    }
  } else if(dir == PD_FRONT || dir == PD_BACK) {
    if(box.x1 > x-0.75f && box.x2 < x+0.75f
       && box.z1 < z && box.z2 > z
       && box.y1 > y-1.25f && box.y2 < y+1.25f) {
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
bool Portal::throughPortal(float r, float s, float t) {
  switch(dir) {
  case PD_LEFT:
    if(r > x && r < x+0.2f && s > y-1.25f && s < y+1.25f && t < z+0.75f && t > z-0.75f)
      return true;
    break;
  case PD_RIGHT:
    if(r < x && x > x-0.2f && s > y-1.25f && s < y+1.25f && t < z+0.75f && t > z-0.75f)
      return true;
    break;
  case PD_FRONT:
    if(t < z && t > z-0.2f && s > y-1.25f && s < y+1.25f && r < x+0.75f && r > x-0.75f)
      return true;
    break;
  case PD_BACK:
    if(t > z && t < z+0.2f && s > y-1.25f && s < y+1.25f && r < x+0.75f && r > x-0.75f)
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

  glTranslatef(x,y,z);
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

  glTranslatef(x,y,z);
  glRotatef(getFromRotation(), 0,1,0);

  // Bind blue of orange portal texture
  if(color == PC_BLUE)
    Resources::inst().bindTexture(TID_BLUEPORTAL);
  else
    Resources::inst().bindTexture(TID_ORANGEPORTAL);

  Resources::inst().drawModel(MID_PORTAL_OUTLINE);

  glPopMatrix();
}
