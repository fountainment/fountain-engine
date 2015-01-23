#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
vec2 surfaceSize = resolution;
vec2 surfacePosition = (gl_FragCoord.xy - resolution * 0.5) / resolution;

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
float Rand(vec2 co)
{
	float a = 1552.9898;
	float b = 78.233;
	float c = 43758.5453;
	float dt= dot(co.xy ,vec2(a,b));
	float sn= mod(dt,3.14);
	return fract(sin(sn) * c);
}
float iAspectRatio = 2.0;
float Noise(vec2 UV, float Seed, vec2 Frequency) {
	vec2 PerlinR = vec2(UV.x, UV.y) * vec2(Frequency);
	vec2 Perlin1Pos = vec2(floor(PerlinR.x), floor(PerlinR.y));

	float RandX0 = (Perlin1Pos.x+(Perlin1Pos.y)*Seed);
	float RandX1 = ((Perlin1Pos.x+1.0)+(Perlin1Pos.y)*Seed);
	float RandX2 = (Perlin1Pos.x+(Perlin1Pos.y+1.0)*Seed);
	float RandX3 = ((Perlin1Pos.x+1.0)+(Perlin1Pos.y+1.0)*Seed);

	float Perlin0Val = Rand(vec2(RandX0,RandX0*0.1224));
	float Perlin1Val = Rand(vec2(RandX1,RandX1*0.1224));
	float Perlin2Val = Rand(vec2(RandX2,RandX2*0.1224));
	float Perlin3Val = Rand(vec2(RandX3,RandX3*0.1224));

	vec2 Perc = (sin(((PerlinR - Perlin1Pos) * vec2(3.1415926)) - vec2(1.570796)) * vec2(0.5)) + vec2(0.5);

	float Val0to2 = (Perlin0Val*(1.0-Perc.y)) + (Perlin2Val*Perc.y);
	float Val1to3 = (Perlin1Val*(1.0-Perc.y)) + (Perlin3Val*Perc.y);

	return (Val0to2 * (1.0-Perc.x)) + (Val1to3 * Perc.x);
}
float PerlinNoise1(vec2 UV, float Seed) {
	float RetVal = 0.0;
	RetVal += Noise(UV, Seed * 1.2, vec2(2.0)) * 0.5;
	RetVal += Noise(UV, Seed * 1.4, vec2(5.0)) * 0.25;
	RetVal += Noise(UV, Seed * 1.1, vec2(10.0)) * 0.125;
	RetVal += Noise(UV, Seed * 1.5, vec2(24.0)) * 0.0625;
	RetVal += Noise(UV, Seed * 1.2, vec2(54.0)) * 0.03125;
	RetVal += Noise(UV, Seed * 1.3, vec2(128.0)) * 0.025625;
	return RetVal;
}
float PerlinNoise2(vec2 UV, float Seed) {
	float RetVal = 0.0;
	RetVal += Noise(UV, Seed * 1.2, vec2(6.0)) * 0.5;
	RetVal += Noise(UV, Seed * 1.4, vec2(12.0)) * 0.25;
	RetVal += Noise(UV, Seed * 1.1, vec2(24.0)) * 0.125;
	RetVal += Noise(UV, Seed * 1.5, vec2(40.0)) * 0.0625;
	RetVal += Noise(UV, Seed * 1.2, vec2(80.0)) * 0.03125;
	RetVal += Noise(UV, Seed * 1.3, vec2(158.0)) * 0.025625;
	return RetVal;
}
void main() {
	vec2 UV = surfacePosition;
	//UV *= vec2(2.1);

	vec2 TimeOffset = vec2(sin(time * 0.00962379), cos(time * 0.00962379)) * vec2(sin(time * 0.0041839) + 0.3);
	vec2 TempVec2A = TimeOffset;
	vec2 TempVec2B = vec2(0.0);
	vec2 TempVec2C = vec2(0.0);
	vec3 TempVec3A = vec3(0.0);
	TempVec2C.x = pow(1.0 - PerlinNoise1(UV + TempVec2A, 21.32143), 3.5);
	TempVec3A = vec3(TempVec2C.x) * vec3(0.25, 0.97, 0.5);
	TempVec2C.x = pow(((1.0 - PerlinNoise2(UV + (TempVec2A * vec2(1.15)), 12.523)) * TempVec2C.x), 1.1);
	TempVec3A += vec3(TempVec2C.x) * vec3(1.0, 0.0, 0.0);

	vec4 RetVal = vec4(0,0,0,1);
	TempVec2C.x = Rand(vec2(UV.x, UV.y));
	TempVec2C.y = Rand(vec2(UV.y, UV.x));
	float PowIn = ((sin(((time+10.0)*TempVec2C.x*1.7))*0.5)+0.5);
	RetVal.xyz = max(vec3(TempVec2C.x * pow(TempVec2C.y, 10.0) * pow(PowIn, 2.0) * 1.0), vec3(0.0));
	RetVal.xyz += TempVec3A;

	//TempVec2A = vec2(/**/0.142 * iAspectRatio, /**/0.324); TempVec3A = vec3(1.0, 0.8, TempVec2A.y);TempVec2C = fract(UV + vec2(time * 0.012 * 0.15));TempVec2B = vec2(1.0 - abs((TempVec2C.x*iAspectRatio) - TempVec2A.x), 1.0 - abs(TempVec2C.y - TempVec2A.y)); RetVal.xyz += (vec3(max(1.0-pow(dot(vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A, vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A) * ((TempVec2A.y * 10000.0) + 700.0), 0.5), 0.0)) * TempVec3A) + (vec3(min(max(floor((TempVec2B.x * 1.005)) + floor((TempVec2B.y * 1.005)), 0.0), 1.0))) * vec3(pow(TempVec2B.x*TempVec2B.y, 40.0)*0.6) * TempVec3A;
	//TempVec2A = vec2(/**/0.881 * iAspectRatio, /**/0.229); TempVec3A = vec3(1.0, 0.8, TempVec2A.y);TempVec2C = fract(UV + vec2(time * 0.012 * 1.00));TempVec2B = vec2(1.0 - abs((TempVec2C.x*iAspectRatio) - TempVec2A.x), 1.0 - abs(TempVec2C.y - TempVec2A.y)); RetVal.xyz += (vec3(max(1.0-pow(dot(vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A, vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A) * ((TempVec2A.y * 10000.0) + 700.0), 0.5), 0.0)) * TempVec3A) + (vec3(min(max(floor((TempVec2B.x * 1.005)) + floor((TempVec2B.y * 1.005)), 0.0), 1.0))) * vec3(pow(TempVec2B.x*TempVec2B.y, 60.0)*0.6) * TempVec3A;
	//TempVec2A = vec2(/**/0.381 * iAspectRatio, /**/0.973); TempVec3A = vec3(1.0, 0.8, TempVec2A.y);TempVec2C = fract(UV + vec2(time * 0.012 * 0.45));TempVec2B = vec2(1.0 - abs((TempVec2C.x*iAspectRatio) - TempVec2A.x), 1.0 - abs(TempVec2C.y - TempVec2A.y)); RetVal.xyz += (vec3(max(1.0-pow(dot(vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A, vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A) * ((TempVec2A.y * 10000.0) + 700.0), 0.5), 0.0)) * TempVec3A) + (vec3(min(max(floor((TempVec2B.x * 1.005)) + floor((TempVec2B.y * 1.005)), 0.0), 1.0))) * vec3(pow(TempVec2B.x*TempVec2B.y, 80.0)*0.6) * TempVec3A;
	//TempVec2A = vec2(/**/0.792 * iAspectRatio, /**/0.587); TempVec3A = vec3(1.0, 0.8, TempVec2A.y);TempVec2C = fract(UV + vec2(time * 0.012 * 0.60));TempVec2B = vec2(1.0 - abs((TempVec2C.x*iAspectRatio) - TempVec2A.x), 1.0 - abs(TempVec2C.y - TempVec2A.y)); RetVal.xyz += (vec3(max(1.0-pow(dot(vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A, vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A) * ((TempVec2A.y * 10000.0) + 700.0), 0.5), 0.0)) * TempVec3A) + (vec3(min(max(floor((TempVec2B.x * 1.005)) + floor((TempVec2B.y * 1.005)), 0.0), 1.0))) * vec3(pow(TempVec2B.x*TempVec2B.y, 50.0)*0.6) * TempVec3A;
	//TempVec2A = vec2(/**/0.642 * iAspectRatio, /**/0.745); TempVec3A = vec3(1.0, 0.8, TempVec2A.y);TempVec2C = fract(UV + vec2(time * 0.012 * 0.15));TempVec2B = vec2(1.0 - abs((TempVec2C.x*iAspectRatio) - TempVec2A.x), 1.0 - abs(TempVec2C.y - TempVec2A.y)); RetVal.xyz += (vec3(max(1.0-pow(dot(vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A, vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A) * ((TempVec2A.y * 10000.0) + 700.0), 0.5), 0.0)) * TempVec3A) + (vec3(min(max(floor((TempVec2B.x * 1.005)) + floor((TempVec2B.y * 1.005)), 0.0), 1.0))) * vec3(pow(TempVec2B.x*TempVec2B.y, 40.0)*0.6) * TempVec3A;
	//TempVec2A = vec2(/**/0.231 * iAspectRatio, /**/0.624);TempVec2C = fract(UV + vec2(time * 0.012 * 0.30));TempVec2B = vec2(1.0 - abs((TempVec2C.x*iAspectRatio) - TempVec2A.x), 1.0 - abs(TempVec2C.y - TempVec2A.y)); RetVal.xyz += (vec3(max(1.0-pow(dot(vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A, vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A) * ((TempVec2A.y * 10000.0) + 700.0), 0.5), 0.0)) * vec3(1.0, 1.0, 0.8)) + (vec3(min(max(floor((TempVec2B.x * 1.005)) + floor((TempVec2B.y * 1.005)), 0.0), 1.0))) * vec3(pow(TempVec2B.x*TempVec2B.y, 40.0)*0.6) * vec3(1.0, 1.0, 0.8);
	//TempVec2A = vec2(/**/0.342 * iAspectRatio, /**/0.798);TempVec2C = fract(UV + vec2(time * 0.012 * 0.60));TempVec2B = vec2(1.0 - abs((TempVec2C.x*iAspectRatio) - TempVec2A.x), 1.0 - abs(TempVec2C.y - TempVec2A.y)); RetVal.xyz += (vec3(max(1.0-pow(dot(vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A, vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A) * ((TempVec2A.y * 10000.0) + 700.0), 0.5), 0.0)) * vec3(1.0, 1.0, 0.8)) + (vec3(min(max(floor((TempVec2B.x * 1.005)) + floor((TempVec2B.y * 1.005)), 0.0), 1.0))) * vec3(pow(TempVec2B.x*TempVec2B.y, 50.0)*0.6) * vec3(1.0, 1.0, 0.8);
	//TempVec2A = vec2(/**/0.332 * iAspectRatio, /**/0.194);TempVec2C = fract(UV + vec2(time * 0.012 * 0.20));TempVec2B = vec2(1.0 - abs((TempVec2C.x*iAspectRatio) - TempVec2A.x), 1.0 - abs(TempVec2C.y - TempVec2A.y)); RetVal.xyz += (vec3(max(1.0-pow(dot(vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A, vec2(TempVec2C.x*iAspectRatio, TempVec2C.y) - TempVec2A) * ((TempVec2A.y * 10000.0) + 700.0), 0.5), 0.0)) * vec3(1.0, 1.0, 0.8)) + (vec3(min(max(floor((TempVec2B.x * 1.005)) + floor((TempVec2B.y * 1.005)), 0.0), 1.0))) * vec3(pow(TempVec2B.x*TempVec2B.y, 17.0)*0.6) * vec3(1.0, 1.0, 0.8);
	gl_FragColor = RetVal;

}
