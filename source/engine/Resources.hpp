#ifndef __RESOURCES_HPP
#define __RESOURCES_HPP

#include <GL/freeglut.h>

#define NUM_SHADERS 2
#define NUM_TEXTURES 14
#define NUM_MODELS 3

enum PROGRAM_ID { PID_PPL, PID_NMAP };

enum TEXTURE_ID {TID_FONT, TID_WALL, TID_WALL_NMAP, TID_TILES, TID_TILES_NMAP, TID_ACID, TID_ACID_NMAP,
		  TID_CAKE, TID_BLUEPORTAL, TID_ORANGEPORTAL,
		 TID_CROSSHAIR, TID_BALLS, TID_STRINGS, TID_BARREL, TID_NONE};

enum MODEL_ID { MID_PORTAL_STENCIL, MID_PORTAL_OUTLINE, MID_CAKE };

/**
 * Singleton class for loading and binding textures.
 */
class Resources {
public:
  /**
   * Returns reference to the singleton instance of Resources
   *
   * @return Instance of Resources object
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

  void compileModels();
  void drawModel(MODEL_ID mid);

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
  GLuint models[NUM_MODELS];		/**< Handle for display lists */

  void compilePortalModel();
  void compileCakeModel();
};

#endif
