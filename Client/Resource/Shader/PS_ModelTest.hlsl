Texture2D shaderTexture;
SamplerState SampleType;

struct PS_INPUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 textureColor;
    
    //textureColor = shaderTexture.Sample(SampleType, input.vTexCoord);
    textureColor = float4(1.f, 0.5f, 0.5f, 1.f);
    
    return textureColor;
}