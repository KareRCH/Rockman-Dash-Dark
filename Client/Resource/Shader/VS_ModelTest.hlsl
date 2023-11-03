cbuffer MatrixBuffer
{
    float4x4 matWorld;
    float4x4 matView;
    float4x4 matProj;
};

struct VS_INPUT
{
    float3 vPosition : POSITION0;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
};

PS_OUTPUT main(VS_INPUT input)
{
    PS_OUTPUT output;
    
    output.vPosition = mul(float4(input.vPosition.xyz, 1.f), matWorld);
    output.vPosition = mul(output.vPosition, matView);
    output.vPosition = mul(output.vPosition, matProj);
    
    output.vNormal = mul(input.vNormal, (float3x3) matWorld);
    output.vNormal = normalize(output.vNormal);
    
    output.vTexCoord = input.vTexCoord;
    
	return output;
}