#include "Character.hpp"

namespace glPortal {
  namespace engine {
    namespace gui {
      Character::Character(char character){
	this->character = character;
	//if(strcmp(character, 'A')){
	if(character == 'A'){
	  row     = 1;
	  column  = 1;	  
	}

	if(character == 'B'){
	  row     = 1;
	  column  = 2;	  
	}

	if(character == 'C'){
	  row     = 1;
	  column  = 3;	  
	}

	/*	if(*character == "A"){
	  row     = 1;
	  column  = 1;	  
	}
	if(*character == "B"){
	  row     = 1;
	  column  = 2;	  
	  }*/

      }

      void Character::draw(int drawPositionX, int drawPositionY){
	Resources::inst().bindTexture(TID_FONT);
	glBegin(GL_QUADS);
	float characterTextureWidth  = 0.035f;
	float characterTextureHeight = 0.17f;
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
      }
    }
  }
} 
