#include "GameScreen.hpp"

GameScreen::GameScreen(Window &window){
  this->window = window;
}

void GameScreen::drawPauseScreen() {
  int height = window.getHeight();
  int width  = window.getWidth();
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
}
