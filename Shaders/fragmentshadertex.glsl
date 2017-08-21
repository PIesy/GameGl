#version 430

void main(void)
{
    float dist = distance(lightPosition, position.xyz) + 0.50f;

    gl_FragDepth = clamp(dist / farPlane, 0.0f, 1.0f);
    //depth = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0f);
}
