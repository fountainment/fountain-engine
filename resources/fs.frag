uniform float time;
vec2 resolution = vec2(1366, 768);
void main( void )
{
	float g = (gl_FragCoord.x * gl_FragCoord.y) / (resolution.x * resolution.y);
	gl_FragColor = vec4(0.0,0.8,1.3,1.0)*sin((gl_FragCoord.y/ resolution.y ) * sin(g * 100000.0 + time) * 9.0 + (gl_FragCoord.x / resolution.y + 20.0) );
}
