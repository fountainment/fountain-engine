uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D tex;
uniform float useTex;
varying vec3 normal;
varying vec3 pixelPos;
vec3 lightPos = vec3(mouse - 0.5 * resolution, 300.0);
vec3 eyePos = vec3(0.0, 0.0, 1000.0);

void main( void ) {
	vec3 normalVec;
	vec4 color = gl_Color;
	if (useTex == 1.0) {
		color *= texture2D(tex, gl_TexCoord[0].st);
	}
	normalVec = normal;
	//normalVec = normal * vec3(-1.0, 1.0, 1.0);
	float light = max(0.0, dot(normalize(normalVec), normalize(lightPos - pixelPos)));
	color.rgb *= light;
	gl_FragColor = color;
}
