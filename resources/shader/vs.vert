uniform float time;
varying vec3 pixelPos;
varying vec3 normal;
varying float LightIntensity;
uniform vec2 mouse;
uniform vec2 resolution;
vec3 lightPos = vec3(mouse - 0.5 * resolution, 500.0);
vec3 eyePos = vec3(0.0, 0.0, 1000.0);
varying vec3 reflectEyeVec;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	pixelPos = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = normalize(gl_NormalMatrix * gl_Normal);
	vec3 lightVec = normalize(lightPos - pixelPos);
	vec3 reflectVec = reflect(-lightVec, normal);
	vec3 viewVec = normalize(eyePos - pixelPos);
	float diffuse = max(dot(lightVec, normal), 0.0);
	float spec = 0.0;
	if (diffuse > 0.0) {
		spec = max(dot(reflectVec, viewVec), 0.0);
		spec = pow(spec, 16.0);
	}
	LightIntensity = 0.7 * diffuse + 0.3 * spec;
	reflectEyeVec = reflect(-viewVec, normal);
	gl_Position = ftransform();
}