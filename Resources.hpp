#ifndef __RESOURCES_HPP
#define __RESOURCES_HPP

#include <GL/glut.h>

class Resources {
public:
	static Resources& inst() {
		static Resources instance;
		return instance;
	}

	void loadTextures();
	void bindTexture(int id);

private:
	Resources() {}
	Resources(Resources const&);
	void operator=(Resources const&);

	GLuint createTexture(const char *filename);

	GLuint textures[7];
};

#endif
