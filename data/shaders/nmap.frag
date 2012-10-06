#version 120

varying vec3 light;

uniform sampler2D texture, nmap;

void main() {
	// Get normal from normal map
	vec3 normal = texture2D(nmap, gl_TexCoord[0].st).rgb * 2.0 - 1.0;
	// Normalize light vector after interpolation
	vec3 lightN = normalize(light);

	// Calculate ambient term
	vec4 ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	// Calculate diffuse term from dot product of normal and light vector
	vec4 diffuse = max(dot(normal, lightN), 0.0) * (gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse);

	// Added texture color to pixel
	gl_FragColor = (diffuse + ambient) * texture2D(texture, gl_TexCoord[0].st);
}
