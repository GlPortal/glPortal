varying vec3 normal, vertex, light;
uniform sampler2D texture, nmap;

void main() {
	vec3 lightN = normalize(light);

	vec4 nmap = texture2D(nmap, gl_TexCoord[0].st)*2.0 - 1.0;
	vec3 nmap_normal = gl_NormalMatrix * nmap.xyz;
	vec3 normal2 = normalize(normal + 0.0*nmap_normal);

	vec4 diffuse = max(dot(normal2, lightN), 0.0) * (gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse);
	vec4 ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	gl_FragColor = (ambient + diffuse) * texture2D(texture, gl_TexCoord[0].st);
}
