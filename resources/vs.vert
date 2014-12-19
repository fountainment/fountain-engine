void main()
{
	vec4 v = gl_Vertex;
	gl_Position = gl_ModelViewProjectionMatrix * v;
}
