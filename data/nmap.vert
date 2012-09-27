#version 120

varying vec3 vertex, light;
varying mat3 tbnMatrix;

void main() {
	vertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	vec3 t = normalize(gl_NormalMatrix * gl_MultiTexCoord1.xyz);
	vec3 b = cross(n, t);

	tbnMatrix = mat3(t, b, n);

	light = normalize(gl_LightSource[0].position.xyz - vertex);
	light = light * tbnMatrix;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
