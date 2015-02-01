uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
uniform sampler2D tex;
uniform float useTex;

void main()
{
	int tNum = int(sin(time) * 15. + 16.);
	int ti = tNum / 2;
	vec2 texSize = vec2(512., 512.);
	vec4 color = gl_Color;
	vec4 sr;
	vec4 srAvg = vec4(0.);
	vec2 tmp;
	float pxNum = 0.;
	if (useTex == 1.) {
		for (int i = -ti; i <= ti; i++) {
			for (int j = -ti; j <= ti; j++) {
				tmp = vec2(i, j);
				sr = vec4(.0);
				if (length(tmp) <= float(ti)) {
					tmp = gl_TexCoord[0].st + tmp / texSize;
					if (tmp.x >= 0.0 && tmp.x <= 1.0 && tmp.y >= 0.0 && tmp.y <= 1.0) {
						sr = texture2D(tex, tmp);
						srAvg += sr;
						pxNum += 1.;
					}
				}
			}
		}
		srAvg /= pxNum;
		color *= srAvg;
	}
	gl_FragColor = color;
}
