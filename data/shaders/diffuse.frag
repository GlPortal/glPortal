#version 330 core

struct Light {
	vec4 position;
	vec3 color;
	float constantAtt;
	float linearAtt;
	float quadraticAtt;
};

uniform mat4 modelMatrix;

uniform sampler2D diffuse;
uniform Light lights[100];
uniform int numLights;

in vec4 pass_position;
in vec2 pass_texCoord;
in vec4 pass_normal;

out vec4 out_Color;

void main(void) {
	vec3 light = vec3(0, 0, 0);
	
	//Calculate the location of this fragment (pixel) in world coordinates
    vec4 position = modelMatrix * pass_position;
    
    for(int i = 0; i < numLights; i++) {
	    //Calculate the vector from this pixels surface to the light source
	    vec4 lightDir = lights[i].position - position;
	    float length = length(lightDir);
	    vec3 lightColor = lights[i].color;
	    
	    //Calculate the cosine of the angle of incidence (brightness)
	    float fDiffuse = dot(pass_normal, normalize(lightDir));
	    float fAttTotal = 1 / (lights[i].constantAtt + lights[i].linearAtt * length + lights[i].quadraticAtt * length * length);
	    light.r += lightColor.r * fAttTotal;
	    light.g += lightColor.g * fAttTotal;
	    light.b += lightColor.b * fAttTotal;
	}

    out_Color = vec4(light, 1) * texture(diffuse, pass_texCoord);
}
