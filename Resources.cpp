#include "Resources.hpp"
#include "stb_image.c"

void Resources::loadTextures() {
	textures[0]  = createTexture("data/wall.png");
	textures[1] = createTexture("data/tiles.png");
	textures[2]  = createTexture("data/toxic.png");

	textures[3] = createTexture("data/blueportal.png");
	textures[4] = createTexture("data/orangeportal.png");
	textures[5] = createTexture("data/crosshair.png");
	textures[6] = createTexture("data/balls.png");
}

void Resources::bindTexture(int id) {
	glBindTexture(GL_TEXTURE_2D, textures[id]);
}

GLuint Resources::createTexture(const char *filename) {
	GLuint handle;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);

	// Read bitmap data
	int w, h, n;
	unsigned char *data = stbi_load(filename, &w, &h, &n, 0);

	// Create texture with mip maps
	if(n == 3)
		gluBuild2DMipmaps(GL_TEXTURE_2D, n, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
	else if(n == 4)
		gluBuild2DMipmaps(GL_TEXTURE_2D, n, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	// Repeat texture in s- and t-axis
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Print any errors
	GLenum error = glGetError();
	while(error != GL_NO_ERROR) {
		printf("Error: %d\n", error);
		error = glGetError();
	}

	return handle;
}
