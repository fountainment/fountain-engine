uniform float time;

void main()
{
	vec4 v = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	v = gl_ModelViewProjectionMatrix * v;
	//v.x += sin(v.y * 5. + time / 3.) / 4.;
	gl_Position = v;
}
