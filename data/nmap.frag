#version 120

varying vec3 vertex, light;
uniform sampler2D texture, nmap;

void main() {
	vec3 normal = texture2D(nmap, gl_TexCoord[0].st).rgb * 2.0 - 1.0;
	vec3 lightN = normalize(light);

	vec4 diffuse = max(dot(normal, lightN), 0.0) * (gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse);
	vec4 ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	gl_FragColor = (ambient + diffuse) * texture2D(texture, gl_TexCoord[0].st);
}
