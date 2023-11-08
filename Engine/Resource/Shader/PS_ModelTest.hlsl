Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

struct PS_INPUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    
    textureColor = shaderTexture.Sample(SampleType, input.vTexCoord);
    lightDir = -lightDirection;
    lightIntensity = saturate(dot(input.vNormal, lightDir));
    color = saturate(diffuseColor * lightIntensity);
    
    color = color * textureColor;
    
    //textureColor = shaderTexture.Sample(SampleType, input.vTexCoord);
    textureColor = float4(1.f, 0.5f, 0.5f, 1.f);
    
    return color;
}