#version 130

struct Light {
	vec3 position;
	vec3 color;
	float distance;
	float energy;
};

uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

uniform sampler2D diffuse;
uniform Light lights[100];
uniform int numLights;

in vec3 pass_position;
in vec2 pass_texCoord;
in vec3 pass_normal;

out vec4 out_Color;

/* Calculates point light attribution */
float calcPointAtt(Light light, vec3 lightDir) {
	float lightLength = length(lightDir);
	float x = lightLength / light.distance;
    float fAtt = 1 - sqrt(x);
    if (fAtt < 0) {
    	fAtt = 0;
    }
    return fAtt;
}

void main(void) {
	vec3 refl = vec3(0, 0, 0);
	
	// Normals
	mat3 normalMatrix = transpose(mat3(normalMatrix));
    vec3 normal = normalize(normalMatrix * pass_normal);
    //vec3 normal = normalize(pass_normal);
    
	//Calculate the location of this fragment (pixel) in world coordinates
    vec3 position = (modelMatrix * vec4(pass_position, 1)).xyz;
    float ambient = 0.3;
    
    for(int i = 0; i < numLights; i++) {
    	Light light = lights[i];
    	
	    //Calculate the vector from this pixels surface to the light source
	    vec3 lightDir = light.position - position;
	    float fAtt = calcPointAtt(light, lightDir);
	    float fDiffuse = clamp(dot(normal, normalize(lightDir)), 0, 1);
	    refl += light.color * fDiffuse * light.energy * fAtt;
	}
	refl += ambient;

    out_Color = vec4(refl, 1) * texture(diffuse, pass_texCoord);
}
