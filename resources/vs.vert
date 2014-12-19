void main()
{
	vec4 v = gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * v;
}
