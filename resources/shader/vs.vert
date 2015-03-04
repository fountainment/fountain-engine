uniform float time;
varying vec3 pixelPos;

void main()
{
	vec4 v = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	pixelPos = vec3(v * gl_ModelViewMatrix);
	v = gl_ModelViewProjectionMatrix * v;
	gl_Position = v;
}
