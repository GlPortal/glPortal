#version 130

uniform mat4 modelMatrix;

uniform sampler2D diffuse;

in vec4 pass_position;
in vec2 pass_texCoord;
in vec4 pass_normal;

out vec4 out_Color;

void main(void) {
    out_Color = texture(diffuse, pass_texCoord);
}
