#version 120

varying vec3 light;

void main() {
	vec3 vertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	// Calculate normal, trangent and bitangent
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	vec3 t = normalize(gl_NormalMatrix * gl_MultiTexCoord1.xyz);
	vec3 b = cross(n, t) * gl_MultiTexCoord1.w;

	// Create matrix for change of basis
	mat3 tbnMatrix = mat3(t.x, b.x, n.x,
						  t.y, b.y, n.y,
						  t.z, b.z, n.z);

	// Calculate vector from vertex to light position
	light = normalize(gl_LightSource[0].position.xyz - vertex);

	// Transform light vector to tangent space
	light = tbnMatrix * light;

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
