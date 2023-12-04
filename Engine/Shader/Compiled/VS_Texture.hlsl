cbuffer MatrixBuffer
{
    matrix matWorld;
    matrix matView;
    matrix matProj;
};

struct VS_INPUT
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PS_INPUT
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
};

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    
    input.position.w = 1.0f;
    
    output.position = mul(input.position, matWorld);
    output.position = mul(output.position, matView);
    output.position = mul(output.position, matProj);
    
    output.tex = input.tex;
    
    return output;

}