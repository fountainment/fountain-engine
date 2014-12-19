uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

void main( void ) {
	vec4 color = gl_Color;
	float gray = color.x * .299 + color.y * .587 + color.z * .114;
	color = vec4(vec3(gray), 1.0);
	gl_FragColor = color;
}
