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

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};

struct LuzPontual {
	vec3 position;

	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	
	int falloff;
};

uniform int flatMode;
uniform vec3 viewPos;
uniform Material material;
uniform LuzPontual luzesPontuais[8];
uniform LuzDirecional luzesDirecionais[8];
uniform int nLP;
uniform int nLD;

in vec3 N;
in vec4 P;

out vec4 fragmentColor;

void main()
{
	vec3 V = normalize(viewPos - P.xyz);

	vec4 vertexColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	for(int i = 0; i < nLP; i++)
	{
		vec3 L = normalize(luzesPontuais[i].position - vec3(P));
		vec3 R = reflect(-L, N);

		vec4 A = luzesPontuais[i].ambient * float(1 - flatMode) * material.ambient;
		vec4 D = material.diffuse * luzesPontuais[i].diffuse * max(dot(N, L), float(flatMode));
		vec4 S = material.spot * luzesPontuais[i].specular * pow(max(dot(R, V), 0), material.shine);
		
		float dl = length(luzesPontuais[i].position - vec3(P));
		vertexColor = vertexColor + (A + D + S)/pow(dl, luzesPontuais[i].falloff);
	}

	for(int i = 0; i < nLD; i++)
	{
		vec3 L = normalize(-luzesDirecionais[i].direction);
		vec3 R = reflect(-L, N);

		vec4 A = luzesDirecionais[i].ambient * float(1 - flatMode) * material.ambient;
		vec4 D = material.diffuse * luzesDirecionais[i].diffuse * max(dot(N, L), float(flatMode));
		vec4 S = material.spot * luzesDirecionais[i].specular * pow(max(dot(R, V), 0), material.shine);

		vertexColor = vertexColor + (A + D + S);
	}
	
	fragmentColor = vertexColor;
}