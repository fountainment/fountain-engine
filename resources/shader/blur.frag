uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D tex;
uniform float useTex;

vec4 ft_GaussianBlur(sampler2D, vec2, vec2, float, float);

void main()
{
	vec2 texSize = vec2(512., 512.);
	vec4 color = gl_Color;
	if (useTex == 1.) {
		color *= ft_GaussianBlur(tex, gl_TexCoord[0].st, texSize, abs(sin(time / 3.14)) * 32.0, 6);
	}
	gl_FragColor = color;
}

vec4 ft_GaussianBlur(sampler2D text, vec2 p, vec2 texSize, float blurRadius, float sampleNum)
{
	if (blurRadius > 0.0 && sampleNum > 1.0) {
		float r = blurRadius;
		float sampleStep = r / sampleNum; 
		vec2 unit = 1.0 / texSize; 
		vec4 res = vec4(0.0);
		float pow = 0.0;
		for (float x = -r; x < r; x += sampleStep) {
			for (float y = -r; y < r; y += sampleStep) {
				float weight = (r - abs(x)) * (r - abs(y));
				res += texture2D(text, p + vec2(x * unit.x, y * unit.y)) * weight; 
				pow += weight;
			}
		}
		return res / pow;
	}
	return texture2D(text, p);
}
