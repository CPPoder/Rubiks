#version 330 core

in float fragmentColor;

out vec4 fragColor;


bool floatEqual(float f1, float f2)
{
    float diff = abs(f1 - f2);
    return (diff < 0.01f);
}

vec3 mapFloatToColor(float f)
{
    vec3 color;

    vec3 WHITE = vec3(1.0f, 1.0f, 1.0f);
    vec3 YELLOW = vec3(1.0f, 1.0f, 0.2f);
    vec3 BLUE = vec3(0.2f, 0.4f, 1.0f);
    vec3 GREEN = vec3(0.1f, 0.9f, 0.0f);
    vec3 RED = vec3(1.0f, 0.1f, 0.1f);
    vec3 ORANGE = vec3(1.0f, 0.5f, 0.1f);

    if (floatEqual(f, 1.0))
    {
        color = WHITE;
    }
    if (floatEqual(f, 2.0))
    {
        color = YELLOW;
    }
    if (floatEqual(f, 3.0))
    {
        color = BLUE;
    }
    if (floatEqual(f, 4.0))
    {
        color = GREEN;
    }
    if (floatEqual(f, 5.0))
    {
        color = RED;
    }
    if (floatEqual(f, 6.0))
    {
        color = ORANGE;
    }

    return color;
}



void main()
{
    fragColor = vec4(mapFloatToColor(fragmentColor), 1.0);
}
