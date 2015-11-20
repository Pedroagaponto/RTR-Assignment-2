varying vec3 normal;
varying vec4 aColor, dColor;

void main()
{
	aColor = gl_FrontMaterial.ambient * gl_LightSource[0].ambient +
			 gl_LightModel.ambient * gl_FrontMaterial.ambient;
	dColor = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;


	normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_Position = gl_ProjectionMatrix * (gl_ModelViewMatrix * gl_Vertex);
}
