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
	float gray = color.x * .299 + color.y * .587 + color.z * .114;
	float cp = colorPercent;
	if (cp < .0) cp = .0;
	if (cp > 1.) cp = 1.;
	float r = (color.r - gray) * cp + gray;
	float g = (color.g - gray) * cp + gray;
	float b = (color.b - gray) * cp + gray;
	color = vec4(r, g, b, color.a);
	gl_FragColor = color;
}
