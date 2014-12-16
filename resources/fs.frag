uniform float time;
vec2 resolution = vec2(800, 600);

void main( void ) {
	vec2 position = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - 1.0;
	position.x *= (resolution.x / resolution.y);
	float len = length( position );
	float freq = 800.0;
	vec2 v2 = ( position / len ) * freq * sin( time*4.0 - len*15.0 )*0.05;
	gl_FragColor = vec4( v2.x, v2.y, 0.0, 1.0 );
}
