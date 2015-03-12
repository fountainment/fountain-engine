uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D tex;
uniform float useTex;
varying vec3 pixelPos;

uniform vec2 observePos;
uniform float scale;

void main( void ) {
	vec4 color = vec4(vec3(0.0), 1.0);
	float r, i, cr, ci;
	float tr, r2;

	r = pixelPos.x / scale - observePos.x;
	i = pixelPos.y / scale - observePos.y;
	cr = r;
	ci = i;
	for (float j = 0.0; j < 1000.0; j++) {
		tr = r;
		r = (tr * tr) - (i * i) + cr;
		i = 2.0 * tr * i + ci;
		r2 = (r * r) + (i * i);
		if (r2 >= 4.0) {
			color = vec4(vec3(min(1.0, j / 100.0)), 1.0);
			break;
		}
	}
	gl_FragColor = color;
}
