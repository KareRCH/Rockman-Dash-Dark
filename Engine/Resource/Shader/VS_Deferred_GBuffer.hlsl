// ��� ��ȯ ����
cbuffer Transform
{
    matrix World;                   // �������
    matrix View;                    // �����
    matrix Projection;              // �������
    matrix WorldViewProjection;     // �������� �̹� ���� ���(���� �Ⱦ�)
};

// �޴� ����
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 Tex : TEXCOORD;
};

// ������ �����
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