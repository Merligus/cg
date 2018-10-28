#version 330 core

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 spot;
	float shine;
};

uniform mat4 transform;
uniform mat3 normalMatrix;
uniform mat4 vpMatrix = mat4(1);
uniform Material material;
uniform vec4 ambientLight = vec4(0.2, 0.2, 0.2, 1);
uniform vec3 lightPosition;
uniform vec4 lightColor = vec4(1);
uniform int flatMode;
uniform vec3 viewPos;

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

out vec4 vertexColor;

void main()
{
  vec4 P = transform * position;
  vec3 L = normalize(lightPosition - vec3(P));
  vec3 N = normalize(normalMatrix * normal);
  vec3 lightDir = normalize(lightPosition - P.xyz);
  vec4 R = vec4(reflect(-lightDir, N), 1.0f);
  vec4 V = vec4(normalize(viewPos - P.xyz), 1.0f);
  vec4 A = ambientLight * float(1 - flatMode) * material.ambient;

  gl_Position = vpMatrix * P;
  vertexColor = A + material.diffuse * lightColor * max(dot(N, L), float(flatMode)) + material.spot * lightColor * pow(max(dot(R, V), 0), material.shine);
}
