#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in float vertexColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 geometryNormal;
out float geometryColor;

void main()
{
	geometryNormal = vertexNormal;
	geometryColor = vertexColor;

	//gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPos, 1.0);
	gl_Position = vec4(vertexPos, 1.0);
}
