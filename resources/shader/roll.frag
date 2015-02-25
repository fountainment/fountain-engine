#ifdef GL_ES
precision mediump float;
#endif


uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
vec2 surfacePosition = (gl_FragCoord.xy - resolution * 0.5) / resolution;

#define PI 3.14159265358979
#define N 33
void main( void ) {
	float size = 0.0555;
	float dist = 0.0;
	float ang = 0.0;
	vec2 pos = vec2(0.0,0.0);
	vec3 color = vec3(0.5);;
	
	for(int i=0; i<N; i++){
		float r = 0.42;
		ang += PI / (float(N)*1.8);
		pos = vec2(cos(ang + time*10.0)*r,sin(ang + time*10.0)*r);
		dist += size / distance(pos, surfacePosition);
		vec3 c = vec3(0.096,0.05,0.045);
		color = c*dist;
	}
	gl_FragColor = vec4(color, 1.0);
}
