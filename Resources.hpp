#ifndef __RESOURCES_HPP
#define __RESOURCES_HPP

#include <GL/glut.h>

#define NUM_SHADERS 2
#define NUM_TEXTURES 11

enum PROGRAM_ID { PID_PPL, PID_NMAP };

static const char *vertex_shaders[2]   = {"data/shaders/ppl.vert", "data/shaders/nmap.vert"};
static const char *fragment_shaders[2] = {"data/shaders/ppl.frag", "data/shaders/nmap.frag"};

enum TEXTURE_ID { TID_WALL, TID_WALL_NMAP, TID_TILES, TID_TILES_NMAP, TID_ACID, TID_ACID_NMAP,
				  TID_BLUEPORTAL, TID_ORANGEPORTAL, TID_CROSSHAIR, TID_BALLS, TID_STRINGS, TID_NONE };

static const char *texture_files[NUM_TEXTURES] = {
	"data/wall.png", "data/wall_normalmap.png",
	"data/tiles.png", "data/tiles_normalmap.png",
	"data/acid.png", "data/acid_normalmap.png",
	"data/blueportal.png", "data/orangeportal.png",
	"data/crosshair.png",
	"data/balls.png",
	"data/strings.png"
};

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
	void bindTexture(TEXTURE_ID id);

	void compileShaders();
	void enableProgram(PROGRAM_ID pid);
	void disableProgram();

private:
	Resources() { }
	Resources(Resources const&);
	void operator=(Resources const&);

	GLuint createTexture(const char *filename);
	char *readShader(const char *filename);
	void printShaderLog(GLuint shader);
	void printProgramLog(GLuint program);

	GLuint textures[NUM_TEXTURES];	/**< Handles for textures */
	GLuint programs[NUM_SHADERS];	/**< Handles for the shader programs */
};

#endif
