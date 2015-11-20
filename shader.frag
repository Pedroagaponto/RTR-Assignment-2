varying vec3 normal;
varying vec4 aColor, dColor;

void main()
{
	vec4 color = aColor;
	vec3 normal2 = normalize(normal);
	vec3 lightDir = normalize(vec3(gl_LightSource[0].position));

	float dotProduct = max(dot(normal2, lightDir), 0.0);


	if (dotProduct > 0.0)
	{
		vec4 sColor = gl_FrontMaterial.specular * gl_LightSource[0].specular;
		float specular = pow(
				max(
					dot(
						normal2,
						normalize(gl_LightSource[0].halfVector.xyz)),
					0.0),
				gl_FrontMaterial.shininess);

		color += (dColor * dotProduct) + (sColor * specular);
	}
	gl_FragColor = color;
}
