#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec3 geometryNormal[];
in float geometryColor[];

out float fragmentColor;
out vec2 fragmentTextureCoord;


struct TrianglesVerticesDeviation
{
	vec3 Right;
	vec3 Up;
};



bool sameDirection(vec3 v1, vec3 v2)
{
	float innerProduct = dot(normalize(v1), normalize(v2));
	return (innerProduct > 0.99f);
}


TrianglesVerticesDeviation getTrianglesVerticesDeviationFromNormal(vec3 normal)
{
	TrianglesVerticesDeviation deviation;

	//Declare helper
	vec3 Up		= vec3(0, 1, 0);
	vec3 Down	= vec3(0, -1, 0);
	vec3 Right	= vec3(1, 0, 0);
	vec3 Left	= vec3(1, 0, 0);
	vec3 Front	= vec3(0, 0, 1);
	vec3 Back	= vec3(0, 0, -1);

	//Up case
	if (sameDirection(normal, vec3(0, 1, 0)))
	{
		deviation.Right = Right;
		deviation.Up = Back;
	}

	//Down case
	if (sameDirection(normal, vec3(0, -1, 0)))
	{
		deviation.Right = Right;
		deviation.Up = Front;
	}

	//Front case
	if (sameDirection(normal, vec3(0, 0, 1)))
	{
		deviation.Right = Right;
		deviation.Up = Up;
	}

	//Back case
	if (sameDirection(normal, vec3(0, 0, -1)))
	{
		deviation.Right = Right;
		deviation.Up = Down;
	}

	//Right case
	if (sameDirection(normal, vec3(1, 0, 0)))
	{
		deviation.Right = Back;
		deviation.Up = Up;
	}

	//Left case
	if (sameDirection(normal, vec3(-1, 0, 0)))
	{
		deviation.Right = Front;
		deviation.Up = Up;
	}
	
	float scale = 0.5f;
	deviation.Right = scale * deviation.Right;
	deviation.Up = scale * deviation.Up;

	return deviation;
}








void main() {
	fragmentColor = geometryColor[0];
	
	mat4 fullMatrix = projectionMatrix * viewMatrix * modelMatrix;

	TrianglesVerticesDeviation deviation = getTrianglesVerticesDeviationFromNormal(geometryNormal[0]);

    gl_Position = fullMatrix * (gl_in[0].gl_Position + vec4(-deviation.Right + deviation.Up, 0.0) + 0.5f * vec4(geometryNormal[0], 0.0));
	fragmentTextureCoord = vec2(0.0f, 0.0f);
    EmitVertex();

    gl_Position = fullMatrix * (gl_in[0].gl_Position + vec4(+deviation.Right + deviation.Up, 0.0) + 0.5f * vec4(geometryNormal[0], 0.0));
	fragmentTextureCoord = vec2(1.0f, 0.0f);
    EmitVertex();

	gl_Position = fullMatrix * (gl_in[0].gl_Position + vec4(-deviation.Right - deviation.Up, 0.0) + 0.5f * vec4(geometryNormal[0], 0.0));
	fragmentTextureCoord = vec2(0.0f, 1.0f);
    EmitVertex();

    gl_Position = fullMatrix * (gl_in[0].gl_Position + vec4(+deviation.Right - deviation.Up, 0.0) + 0.5f * vec4(geometryNormal[0], 0.0));
	fragmentTextureCoord = vec2(1.0f, 1.0f);
    EmitVertex();
    
    EndPrimitive();
}  