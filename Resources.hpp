#ifndef __RESOURCES_HPP
#define __RESOURCES_HPP

#include <GL/glut.h>

enum TEXTURE_ID { TID_WALL, TID_WALL_NMAP, TID_TILES, TID_TILES_NMAP, TID_ACID,
				  TID_BLUEPORTAL,TID_ORANGEPORTAL, TID_CROSSHAIR, TID_BALLS, TID_NONE };

enum PROGRAM_ID { PID_NMAP };

/**
 * Singleton class for loading and binding textures.
 */
class Resources {
public:
	/**
	 * Returns reference to the singleton instance of Resources
	 *
	 * @return Instance of Resources
	 */
	static Resources& inst() {
		static Resources instance;
		return instance;
	}

	void loadTextures();
	void bindTexture(TEXTURE_ID id, int unit = 0);

	void compileShaders();
	void enableProgram(PROGRAM_ID pid);
	void disableProgram();

private:
	Resources() { }
	Resources(Resources const&);
	void operator=(Resources const&);

	GLuint createTexture(const char *filename);
	char *readShader(const char *filename);

	GLuint textures[9]; /**< Handles for textures */
	GLuint program;		/**< Handle for the shader program */
};

#endif
