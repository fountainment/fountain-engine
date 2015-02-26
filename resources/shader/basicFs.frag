uniform sampler2D tex;
uniform float useTex;

void main( void ) {
	vec4 colorx = gl_Color;
	if (useTex == 1.0) {
		colorx *= texture2D(tex, gl_TexCoord[0].st);
	}
	gl_FragColor = colorx;
}