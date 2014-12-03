#version 330 core

struct Light {
	vec3 position;
	vec3 color;
	vec3 attenuation;
};

uniform mat4 modelMatrix;

uniform sampler2D diffuse;
uniform Light lights[100];
uniform int numLights;

in vec3 pass_position;
in vec2 pass_texCoord;
in vec3 pass_normal;

out vec4 out_Color;

void main(void) {
	vec3 refl = vec3(0, 0, 0);
	
	// Normals
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    vec3 normal = normalize(normalMatrix * pass_normal);
    
	//Calculate the location of this fragment (pixel) in world coordinates
    vec3 position = (modelMatrix * vec4(pass_position, 1)).xyz;
    
    for(int i = 0; i < numLights; i++) {
    	Light light = lights[i];
    	
	    //Calculate the vector from this pixels surface to the light source
	    vec3 lightDir = light.position - position;
	    vec3 lightColor = light.color;
	    
	    float length = length(lightDir);
	    
	    //Calculate the cosine of the angle of incidence (brightness)
	    float fDiffuse = dot(normal, normalize(lightDir));
	    
	    float constantAtt  = light.attenuation.x;
	    float linearAtt    = light.attenuation.y;
	    float quadraticAtt = light.attenuation.z;
	    float fAttTotal = 1 / (constantAtt + linearAtt * length + quadraticAtt * length * length);
	    
	    fDiffuse = clamp(fDiffuse, 0, 1);
	    
	    refl.r += lightColor.r * fAttTotal * fDiffuse;
	    refl.g += lightColor.g * fAttTotal * fDiffuse;
	    refl.b += lightColor.b * fAttTotal * fDiffuse;
	}
	//refl.r = clamp(refl.r, 0, 1);
	//refl.g = clamp(refl.g, 0, 1);
	//refl.b = clamp(refl.b, 0, 1);

    out_Color = vec4(refl, 1) * texture(diffuse, pass_texCoord);
}
