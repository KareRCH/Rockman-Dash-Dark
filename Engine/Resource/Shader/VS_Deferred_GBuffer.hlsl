// 행렬 변환 정보
cbuffer Transform
{
    matrix World;                   // 월드행렬
    matrix View;                    // 뷰행렬
    matrix Projection;              // 투영행렬
    matrix WorldViewProjection;     // 투영까지 이미 계산된 행렬(아직 안씀)
};

// 받는 정보
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 Tex : TEXCOORD;
};

// 나가는 결과물
struct VS_OUTPUT
{
    float4 LocalPos : LOCAL_POS;
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float3 Trangent : TANGENT;
    float2 Tex : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.LocalPos = float4(input.Position, 1.f);
    output.Position = mul(float4(input.Position, 1.f), World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    output.Normal = mul(float4(input.Normal, 1.f), World);
    
    output.Tex = input.Tex;
    
	return output;
}

VS_OUTPUT LightingVS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.LocalPos = float4(input.position, 1.0f);
    output.Position = output.LocalPos;
    output.Tex = input.Tex;

    return output;
}