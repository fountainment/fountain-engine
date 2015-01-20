//---------------------------------------------------------
// Shader:   WaterTurbulence.glsl
// WaterTurbulence by joltz0r 2013-07-07
// http://glslsandbox.com/e#22111.0
// mouse zooming added by I.G.P.  2015-01-12
//---------------------------------------------------------

#ifdef GL_ES
  precision highp float;
#endif

uniform float time;
uniform vec3 mouse;
uniform vec2 resolution;

//---------------------------------------------------------
#define MAX_ITER 12
#define ITENSITY 0.2

vec3 blue = vec3(0.25, 0.55, 1.3);

void main( void ) 
{
  vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
  p *= 6.0 * (0.4 + mouse.y / resolution.y);
  vec2 i = p;
  float c = 0.0;
  int iter = MAX_ITER;
  for (int n = 0; n < MAX_ITER; n++) 
  {
    float t = time * (1.0 - (1.0 / float(n+1)));
    i = p + vec2( cos(t - i.x) + sin(t + i.y), 
                  sin(t - i.y) + cos(t + i.x));
    c += 2.0 / length(vec2( (sin(i.x + t) / ITENSITY),
                            (cos(i.y + t) / ITENSITY)));
  }
  c /= float(MAX_ITER);
  gl_FragColor = vec4(c*blue, 1.0);
}
