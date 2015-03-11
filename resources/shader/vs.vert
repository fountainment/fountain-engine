uniform float time;
varying vec3 pixelPos;
varying vec3 normal;

void main()
{
	vec4 v = gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	pixelPos = vec3(v * gl_ModelViewMatrix);
	v = gl_ModelViewProjectionMatrix * v;
	//normal = normalize(vec3(vec4(gl_Normal, 1.0) * gl_ModelViewMatrix));
	normal = gl_Normal;
	gl_Position = v;
}