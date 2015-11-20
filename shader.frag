varying vec3 normal;

void main()
{
	vec4 aColor = gl_FrontMaterial.ambient * gl_LightSource[0].ambient + 
		gl_LightModel.ambient * gl_FrontMaterial.ambient;
	vec4 dColor = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	vec4 sColor = gl_FrontMaterial.specular * gl_LightSource[0].specular;

	vec4 color = aColor;
	vec3 normal2 = normalize(normal);
	vec3 lightDir = normalize(vec3(gl_LightSource[0].position));

	float dotProduct = max(dot(normal2, lightDir), 0.0);
	float specular = pow(max(dot(normal2,
		normalize(gl_LightSource[0].halfVector.xyz)), 0.0), 20.0);

	if (dotProduct > 0.0)
		color = aColor + (dColor * dotProduct) + (sColor * specular);

	gl_FragColor = color;
}
