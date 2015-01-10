#version 130
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_explicit_attrib_location : enable

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform vec2 tiling;

//layout(location = 0) in vec3 position;
//layout(location = 1) in vec2 texCoord;
//layout(location = 2) in vec3 normal;
in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec3 pass_position;
out vec2 pass_texCoord;
out vec3 pass_normal;

void main() {
	pass_position = position;
	pass_texCoord = vec2(texCoord.x * tiling.x, texCoord.y * tiling.y);
	pass_normal = normal;
	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1);
}
