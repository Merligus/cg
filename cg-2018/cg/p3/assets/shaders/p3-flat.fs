#version 330 core

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 spot;
	float shine;
};

struct LuzDirecional {
	vec3 direction;

	vec4 color;
};

struct LuzPontual {
	vec3 position;

	vec4 color;
	
	int falloff;
};

struct LuzSpot {
	vec3 direction;
	vec3 position;

	vec4 color;

	int falloff;
	float innerCutOff;
	float outerCutOff;
};

uniform int flatMode;
uniform vec3 viewPos;
uniform Material material;
uniform LuzPontual luzesPontuais[8];
uniform LuzDirecional luzesDirecionais[8];
uniform LuzSpot luzesSpots[8];
uniform int nLP;
uniform int nLD;
uniform int nLS;

in vec4 P;

out vec4 fragmentColor;

void main()
{
	vec3 xTangent = dFdx(P.xyz);
	vec3 yTangent = dFdy(P.xyz);

	vec3 N = normalize(cross(xTangent, yTangent));
	vec3 V = normalize(viewPos - P.xyz);

	vec4 vertexColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	for(int i = 0; i < nLP; i++)
	{
		vec3 L = normalize(luzesPontuais[i].position - P.xyz);
		vec3 R = reflect(-L, N);

		vec4 A = luzesPontuais[i].color * float(1 - flatMode) * material.ambient;
		vec4 D = material.diffuse * luzesPontuais[i].color * max(dot(N, L), float(flatMode));
		vec4 S = material.spot * luzesPontuais[i].color * pow(max(dot(R, V), 0), material.shine);
		
		float dl = length(luzesPontuais[i].position - P.xyz);
		vertexColor = vertexColor + (A + D + S)/pow(dl, luzesPontuais[i].falloff);
	}

	for(int i = 0; i < nLD; i++)
	{
		vec3 L = normalize(-luzesDirecionais[i].direction);
		vec3 R = reflect(-L, N);

		vec4 A = luzesDirecionais[i].color * float(1 - flatMode) * material.ambient;
		vec4 D = material.diffuse * luzesDirecionais[i].color * max(dot(N, L), float(flatMode));
		vec4 S = material.spot * luzesDirecionais[i].color * pow(max(dot(R, V), 0), material.shine);

		vertexColor = vertexColor + (A + D + S);
	}

	for(int i = 0; i < nLS; i++)
	{
		vec3 L = normalize(luzesSpots[i].position - P.xyz);
		float theta = dot(normalize(-luzesSpots[i].direction), L);
		float epsilon = luzesSpots[i].innerCutOff - luzesSpots[i].outerCutOff;
		float intensity = clamp((theta - luzesSpots[i].outerCutOff) / epsilon, 0.0, 1.0);
		vec3 R = reflect(-L, N);

		vec4 A = luzesSpots[i].color * float(1 - flatMode) * material.ambient;
		vec4 D = material.diffuse * luzesSpots[i].color * max(dot(N, L), float(flatMode));
		vec4 S = material.spot * luzesSpots[i].color * pow(max(dot(R, V), 0), material.shine);

		float dl = length(luzesSpots[i].position - P.xyz);

		vertexColor = vertexColor + intensity * (A + D + S) / pow(dl, luzesSpots[i].falloff);
	}
	
	fragmentColor = vertexColor;
}