#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform vec2 tiling;

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 normal;

out vec4 pass_position;
out vec2 pass_texCoord;
out vec4 pass_normal;

void main() {
	pass_position = position;
	pass_texCoord = vec2(texCoord.x * tiling.x, texCoord.y * tiling.y);
	pass_normal = normal;
	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
