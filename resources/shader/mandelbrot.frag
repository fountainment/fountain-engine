uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D tex;
uniform float useTex;
varying vec3 pixelPos;

uniform vec2 observePos;
uniform float scale;

void main( void ) {

	vec4 colorx = gl_Color;
	if (useTex == 1.0) {
		colorx *= texture2D(tex, gl_TexCoord[0].st);
	}

	float r, i, cr, ci;
	float tr, r2;
	r = pixelPos.x / scale - observePos.x;
	i = pixelPos.y / scale - observePos.y;
	cr = r;
	ci = i;
	for (float j = 0.0; j < 1000.0 && r2 < 4.0; ++j) {
		tr = r;
		r = (tr * tr) - (i * i) + cr;
		i = 2.0 * tr * i + ci;
		r2 = (r * r) + (i * i);
		if (r2 >= 4.0) {
			colorx.rgb = vec3(j / 100.0);
		}
	}
	if (r2 < 4.0) colorx.rgb *= 0.0;
	gl_FragColor = colorx;

}