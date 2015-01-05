#version 130

uniform mat4 modelMatrix;

uniform sampler2D diffuse;

in vec4 pass_position;
in vec2 pass_texCoord;
in vec4 pass_normal;

out vec4 out_Color;

void main(void) {
	float smoothing = 0.1;// / (4 * 0.01);
	vec4 color = vec4(1, 1, 1, 1);
	float mask = texture2D(diffuse, pass_texCoord).a;
	
	if(mask < 0.5) {
		out_Color = vec4(0, 0, 0, 0);
	} else {
		out_Color = vec4(1, 1, 1, 1);
	}

    color.a *= smoothstep(0.5 - smoothing, 0.5 + smoothing, mask);
    
    out_Color = color;
}
