#version 330 core

struct Light {
	vec4 position;
	vec3 color;
	float intensity;
};

uniform mat4 modelMatrix;

uniform Light lights[150];
uniform int numLights;

in vec4 pass_position;
//in vec2 pass_texCoord;
in vec4 pass_normal;

out vec4 out_Color;

void main(void) {
	int i;
	vec3 light = vec3(0, 0, 0);
	
	//Calculate the location of this fragment (pixel) in world coordinates
    vec4 position = modelMatrix * pass_position;
    
    for(i = 0; i < numLights; i++) {
	    //Calculate the vector from this pixels surface to the light source
	    vec4 lightDir = lights[i].position - position;
	    vec3 lightColor = lights[i].color;
	    
	    //Calculate the cosine of the angle of incidence (brightness)
	    float intensity = dot(pass_normal, normalize(lightDir)) * lights[i].intensity / length(lightDir);
	    light.r += lightColor.r * intensity;
	    light.g += lightColor.g * intensity;
	    light.b += lightColor.b * intensity;
	}
    
    light.r = clamp(light.r, 0, 1);
    light.g = clamp(light.g, 0, 1);
    light.b = clamp(light.b, 0, 1);
    
    out_Color = vec4(light, 1);
}
