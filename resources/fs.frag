uniform float time;
vec2 resolution = vec2(1366, 768);
void main( void )
{
	vec2 position = (gl_FragCoord.xy / resolution.xy);
	vec3 color = vec3(0.0, position.x, position.y);
	gl_FragColor = vec4(color, 1.0);
}
