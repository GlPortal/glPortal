#version 120

varying vec3 normal, light;

void main() {
	// Calculate vertex position
	vec3 vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	// Calculate vector from vertex to light position
	light = normalize(gl_LightSource[0].position.xyz - vertex);

	// Transform normal
	normal = gl_NormalMatrix * gl_Normal;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
