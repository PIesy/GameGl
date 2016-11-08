#version 330


smooth in vec2 texCoord;
smooth in vec4 out_color;
smooth in vec4 lightPos;

uniform bool useTex = false;

uniform sampler2DShadow tex2D;

out vec4 color;

void main(void)
{
    float visibility = 0;
    vec3 nonHomogeneous = lightPos.xyz / lightPos.w;
    nonHomogeneous = nonHomogeneous * 0.5 + 0.5;
    nonHomogeneous.z = nonHomogeneous.z - 0.00005f;

    color = out_color;
//    if (useTex)
//        color = vec4(texture(texture2D, nonHomogeneous.xy).xyz, 1);
    visibility = clamp(texture(tex2D, nonHomogeneous.xyz), 0.2f, 1.0f);

    color = vec4((visibility * out_color).xyz, out_color.w);
//    if (visibility == gl_FragCoord.z)
//        color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
//    else
//        color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
