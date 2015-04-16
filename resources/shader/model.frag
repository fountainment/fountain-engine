uniform sampler2D tex;
uniform float useTex;
varying float LightIntensity;

void main( void ) {
	vec4 color = gl_Color;
	if (useTex == 1.0) {
		color *= texture2D(tex, gl_TexCoord[0].st);
	}
	color.rgb *= LightIntensity;
	gl_FragColor = color;
}
