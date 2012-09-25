varying vec3 normal, vertex;
uniform sampler2D texture;

void main() {
	vec3 light = normalize(gl_LightSource[0].position.xyz - vertex);

	vec4 diffuse = max(dot(normal,light), 0.0) * (gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse);
	vec4 ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	gl_FragColor = (ambient + diffuse) * texture2D(texture, gl_TexCoord[0].st);
}
