#version 430

in vec2 uvCoords;

const uint SAMPLES = 1024;
const float divisor = 1.0f / SAMPLES;

out vec3 color;

const float pi = 3.14159265359f;

float radicalInverse(uint bits) 
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10;
}

vec2 hammersleySeq(uint i)
{
	return vec2(i * divisor, radicalInverse(i));
}

vec3 importanceSampleGGX(vec2 Xi, vec3 normal, float roughness)
{
	float a = roughness * roughness;
	
	float phi = 2.0 * pi * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);

	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;

	vec3 up = abs(normal.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, normal));
	vec3 bitangent = cross(normal, tangent);
	
	vec3 sampleVec = tangent * H.x + bitangent * H.y + normal * H.z;
	return normalize(sampleVec);
}

float GeometrySchlickGGX(float NV, float roughness)
{
    float k = (roughness * roughness) / 2.0f;

    return NV / (NV * (1.0f - k) + k);
}

float GeometrySmith(vec3 normal, vec3 V, vec3 L, float roughness)
{
    float NV = max(dot(normal, V), 0.0);
    float NdotL = max(dot(normal, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec2 integrate(float NV, float roughness)
{
    vec3 V;
    V.x = sqrt(1.0 - NV * NV);
    V.y = 0.0;
    V.z = NV;

    float A = 0.0;
    float B = 0.0; 

    vec3 normal = vec3(0.0, 0.0, 1.0);

    for(uint i = 0; i < SAMPLES; ++i)
    {
        vec2 Xi = hammersleySeq(i);
        vec3 H = importanceSampleGGX(Xi, normal, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if(NdotL > 0.0)
        {
            float G = GeometrySmith(normal, V, L, roughness);
            float G_Vis = (G * VdotH) / (NdotH * NV);
            float Fc = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    A *= divisor;
    B *= divisor;
    return vec2(A, B);
}

void main() 
{
    color = vec3(integrate(uvCoords.x, uvCoords.y), 0.0f);
}