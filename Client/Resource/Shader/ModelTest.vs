cbuffer MatrixBuffer
{
    matrix matWorld;
    matrix matView;
    matrix matProj;
};

struct VS_INPUT
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD;
};

struct PS_OUTPUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD;
};

PS_OUTPUT main(VS_INPUT input) : PS_OUTPUT
{
    PS_OUTPUT output;
    
    output.vPosition = mul(float4(input.vPosition.xyz, 1.f), matWorld);
    output.vPosition = mul(output.vPosition, matView);
    output.vPosition = mul(output.vPosition, matProj);
    
    output.vNormal = input.vNormal;
    output.vTexCoord = input.vTexCoord;
    
	return output;
}