#version 120

varying vec3 normal, light;
uniform sampler2D texture, nmap;

void main() {
	// Normalize vectors after interpolation
	vec3 lightN = normalize(light);
	vec3 normalN = normalize(normal);

	// Calculate ambient term
	vec4 ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	// Calculate diffuse term from dot product of normal and light vector
	vec4 diffuse = max(dot(normalN, lightN), 0.0) * (gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse);

	// Added texture color to pixel
	gl_FragColor = (ambient + diffuse) * texture2D(texture, gl_TexCoord[0].st);
}
