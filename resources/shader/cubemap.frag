uniform samplerCube cubeTex;
varying vec3 reflectEyeVec;

void main()
{
	vec4 color = gl_Color;
	color *= textureCube(cubeTex, reflectEyeVec);
	gl_FragColor = color;
}