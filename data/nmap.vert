varying vec3 normal, vertex, light;

void main() {
	vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);

	light = normalize(gl_LightSource[0].position.xyz - vertex);

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
