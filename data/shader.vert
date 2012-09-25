varying vec3 normal, vertex;

void main() {
	vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = ftransform();
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
