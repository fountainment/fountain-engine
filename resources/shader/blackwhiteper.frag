uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D tex;
uniform float useTex;
uniform float colorPercent;

void main( void ) {
	vec4 color = gl_Color;
	if (useTex == 1.0) {
		color *= texture2D(tex, gl_TexCoord[0].st);
	}
	if (colorPercent < .0) colorPercent = .0;
	if (colorPercent > 1.) colorPercent = 1.;
	float r = color.x * (.299 + .701 * colorPercent);
	float g = color.y * (.587 + .413 * colorPercent);
	float b = color.z * (.114 + .886 * colorPercent);
	color = vec4(r, g, b, color.w);
	gl_FragColor = color;
}
