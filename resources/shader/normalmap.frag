uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D tex;
uniform float useTex;
uniform sampler2D nmTex;
//uniform vec3 lightPos;
vec3 lightPos = vec3((mouse / resolution - vec2(0.5)), 1.0);
varying vec3 pixelPos;
vec3 eyePos = vec3(0.0, 0.0, 1.0);

void main( void ) {

	vec4 colorx = gl_Color;
	if (useTex == 1.0) {
		colorx *= texture2D(tex, gl_TexCoord[0].st);
	}

	vec3 normalVec = vec3(texture2D(nmTex, gl_TexCoord[0].st));
	normalVec *= 2.0;
	normalVec -= vec3(1.0);
	normalVec *= vec3(-1.0, 1.0, 1.0);
	float l1 = max(0.0, dot(normalVec, normalize(lightPos - pixelPos)));
	//colorx.rgb *= l1;
	//colorx.rgb += l1 - vec3(0.5);
	colorx.rgb = vec3(l1);
	gl_FragColor = colorx;

}