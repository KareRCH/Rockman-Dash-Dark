cbuffer MatrixBuffer
{
    matrix matWorld;
    matrix matView;
    matrix matProjection;
};

struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PS_INPUT
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

PS_INPUT main(VS_INPUT input) : PS_INPUT
{
    PS_INPUT output;
    
    input.position.w = 1.f;
    
    output.position = mul(input.position, matWorld);
    output.position = mul(output.position, matView);
    output.position = mul(output.position, matProjection);
    
    output.color = input.color;
    
    return output;
}