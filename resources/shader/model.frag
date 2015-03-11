uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D tex;
uniform float useTex;
varying vec3 normal;
varying vec3 pixelPos;
vec3 lightPos = vec3(mouse - 0.5 * resolution, 500.0);
vec3 eyePos = vec3(0.0, 0.0, 1000.0);

void main( void ) {
	vec3 N = normal;
	vec3 P = pixelPos;
	vec4 color = gl_Color;
	if (useTex == 1.0) {
		color *= texture2D(tex, gl_TexCoord[0].st);
	}

	vec3 L = normalize(lightPos - P);
	vec3 V = normalize(eyePos - P);
	vec3 H = normalize(L + V);

	float light = pow(max(dot(N, H), 0.0), 9.0);
	color.rgb *= light;
	//if (light > 0.9) color.rgb = vec3(0.0, 0.0, 1.0);
	gl_FragColor = color;
}
