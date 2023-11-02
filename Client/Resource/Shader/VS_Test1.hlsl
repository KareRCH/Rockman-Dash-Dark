cbuffer MatrixBuffer
{
    float4x4 matWorld;
    float4x4 matView;
    float4x4 matProjection;
};

struct VS_INPUT
{
    float3 vPosition : POSITION0;
    float4 vColor : COLOR0;
};

struct PS_INPUT
{
    float4 vPosition : SV_Position0;
    float4 vColor : COLOR0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    
    output.vPosition = mul(float4(input.vPosition.xyz, 1.f), matWorld);
    output.vPosition = mul(output.vPosition, matView);
    output.vPosition = mul(output.vPosition, matProjection);
    output.vColor = input.vColor;
    
    return output;
}