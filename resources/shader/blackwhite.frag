uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D tex;
uniform float useTex;

void main( void ) {
	vec4 color = gl_Color;
	if (useTex == 1.0) {
		color *= texture2D(tex, gl_TexCoord[0].st);
	}
	float gray = color.x * .299 + color.y * .587 + color.z * .114;
	color = vec4(vec3(gray), color.w);
	gl_FragColor = color;
}
