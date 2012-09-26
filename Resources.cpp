#include <GL/glew.h> // Needs to be imported before Resource.hpp as it includes gl.h
#include "Resources.hpp"
#include <fstream>
#include "stb_image.c"

/**
 * Loads all textures from files.
 */
void Resources::loadTextures() {
	textures[TID_WALL]  = createTexture("data/wall.png");
	textures[TID_WALL_NMAP] = createTexture("data/wall_normalmap.png");
	textures[TID_TILES] = createTexture("data/tiles.png");
	textures[TID_TILES_NMAP] = createTexture("data/tiles_normalmap.png");
	textures[TID_ACID]  = createTexture("data/toxic.png");

	textures[TID_BLUEPORTAL]   = createTexture("data/blueportal.png");
	textures[TID_ORANGEPORTAL] = createTexture("data/orangeportal.png");
	textures[TID_CROSSHAIR]    = createTexture("data/crosshair.png");
	textures[TID_BALLS]        = createTexture("data/balls.png");
}

/**
 * Binds a texture.
 *
 * @param id TEXTURE_ID of the texture to bind
 * @param unit Texture unit to bind to (e.g. GL_TEXTUREunit)
 */
void Resources::bindTexture(TEXTURE_ID id, int unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textures[id]);

	if(id == TID_WALL) {
		bindTexture(TID_WALL_NMAP, 1);
	} else if(id == TID_TILES) {
		bindTexture(TID_TILES_NMAP, 1);
	}
}

/**
 * Compiles all GLSL shaders
 */
void Resources::compileShaders() {
	GLuint vert, frag; // Vertex and fragment shader handles

	// Create handles for program and shaders
	program = glCreateProgram();
	vert = glCreateShader(GL_VERTEX_SHADER);
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	// Read shaders from files
	char *vs_source = readShader("data/nmap.vert");
	char *fs_source = readShader("data/nmap.frag");
	// Loader shader sources
	glShaderSource(vert, 1, (const char**)&vs_source, NULL);
	glShaderSource(frag, 1, (const char**)&fs_source, NULL);
	// Compile shaders
	glCompileShader(vert);
	glCompileShader(frag);
	// Attach shaders to program
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	// Link program
	glLinkProgram(program);
	
	// Bind first texture unit to "texture" in fragment shader
	GLuint param = glGetUniformLocation(program, "tex");
	glUniform1i(param, 0);
	// Bind second texture unit to "nmap" in fragment shader
	param = glGetUniformLocation(program, "nmap");
	glUniform1i(param, 1);

	// Free memory from shader files
	delete[] vs_source;
	delete[] fs_source;
}

/**
 * Enables a shader program from the enumerator PROGRAM_ID.
 *
 * @param pid PROGRAM_ID for the program to enable
 */
void Resources::enableProgram(PROGRAM_ID pid) {
	glUseProgram(program);
}

/**
 * Disables the current program and falls back to fixed-function pipeline
 */
void Resources::disableProgram() {
	glUseProgram(0);
}

/**
 * Reads the entire contents of a shader into a char buffer.
 * Data must be freed after use.
 *
 * @param filename Path to the shader to read
 * @return Pointer to array of chars containing shader
 */
char* Resources::readShader(const char *filename) {
	std::ifstream file;
	char *buffer;
	int length;

	file.open(filename);
	file.seekg(0, std::ios::end);
	length = file.tellg();
	file.seekg(0, std::ios::beg);
	buffer = new char[length+1];
	file.read(buffer, length);
	buffer[length] = 0; // Null terminate to be safe
	file.close();

	return buffer;
}

/**
 * Loads a texture from disk and return a handle to it.
 *
 * @param filename Path to a texture image
 *
 * @return Handle of the created OpenGL texture
 */
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
