#include "Character.hpp"
#include <iostream> 

namespace glPortal {
  namespace engine {
    namespace gui {
      Character::Character(char character){
	      this->character = character;
	      std::pair<int,int> pair = characterMap[character];
	      row    = pair.first;
	      column = pair.second;
      }

      void Character::draw(int drawPositionX, int drawPositionY) {
        glEnable(GL_TEXTURE_2D);
	      Resources::inst().bindTexture(TID_FONT);
	      glBegin(GL_QUADS);
	      float characterTextureWidth  = 0.03829f;
	      float characterTextureHeight = 0.163f;
	      //left top corner x, y
	      glTexCoord2f((column - 1)*characterTextureWidth, (row - 1)*characterTextureHeight); 
	      glVertex2f(drawPositionX, drawPositionY);
	      //left bottom corner x, y
	      glTexCoord2f((column - 1) * characterTextureWidth, row * characterTextureHeight); 
	      glVertex2f(drawPositionX, drawPositionY + 27);
	      //right bottom corner x, y
	      glTexCoord2f(column * characterTextureWidth, row * characterTextureHeight); 
	      glVertex2f(drawPositionX + 27, drawPositionY + 27);
	      //right top corner x, y
	      glTexCoord2f(column * characterTextureWidth, (row - 1) * characterTextureHeight); 
	      glVertex2f(drawPositionX + 27, drawPositionY);
	      glEnd();
	      glDisable(GL_TEXTURE_2D);
      }
    }
  }
} 
