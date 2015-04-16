varying vec3 normal;
varying vec3 pixelPos;
varying vec3 normal;
uniform vec2 mouse;
uniform vec2 resolution;
vec3 lightPos = vec3(mouse - 0.5 * resolution, 500.0);
vec3 eyePos = vec3(0.0, 0.0, 1000.0);
uniform samplerCube cubeTex;

void main()
{
	vec4 color = gl_Color;
	vec3 eyeVec = normalize(eyePos - pixelPos);
	vec3 N = normalize(normal);
	vec3 reflectVec = reflect(-eyeVec, N);
	color *= textureCube(cubeTex, reflectVec);
	gl_FragColor = color;
}