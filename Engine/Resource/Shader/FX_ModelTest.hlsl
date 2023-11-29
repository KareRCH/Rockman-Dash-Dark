// ��� ��ȯ
cbuffer MatrixBuffer : register(b0)
{
    float4x4 matWorld;
    float4x4 matView;
    float4x4 matProj;
};

// ī�޶�� ����
cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

cbuffer BoneBuffer
{
    float4x4 matBone[128];
};

struct VS_INPUT
{
    float3 vPosition : POSITION0;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float3 vTangent : TANGENT0;
    int4 vBoneID : BONEID;
    float4 vWeight : WEIGHT;
};

struct VS_OUTPUT
{
    float4 vPosition : SV_POSITION0;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float3 vViewDirection : TEXCOORD1;
};

VS_OUTPUT VS_MAIN(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    output.vPosition = mul(float4(input.vPosition.xyz, 1.f), matWorld);
    output.vPosition = mul(output.vPosition, matView);
    output.vPosition = mul(output.vPosition, matProj);
    
    output.vNormal = mul(input.vNormal, (float3x3) matWorld);
    output.vNormal = normalize(output.vNormal);
    
    output.vTexCoord = input.vTexCoord;
    
    // ���� ���� ��ġ ���
    float4 worldPosition = mul(float4(input.vPosition.xyz, 1.f), matWorld);
    
    // Look ���� ����
    output.vViewDirection = cameraPosition.xyz - worldPosition.xyz;
    
    // �� ���� ���� ǥ��ȭ
    output.vViewDirection = normalize(output.vViewDirection);
    
    return output;
}

Texture2D shaderTexture;
SamplerState SampleType;

// �� ������ �����ϴ� ����
cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

struct PS_INPUT
{
    float4 vPosition : SV_POSITION0;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float3 vViewDirection : TEXCOORD1;
};

struct PS_OUTPUT
{
    float4 Color : SV_TARGET0;
    float4 Normal : SV_TARGET1;
};

PS_OUTPUT PS_MAIN(PS_INPUT input)
{
    PS_OUTPUT output;
    
    // ���̽� �÷� ����
    float4 textureColor = shaderTexture.Sample(SampleType, input.vTexCoord);
    
    // �⺻ ���� ȯ�汤���� ����
    float4 color = ambientColor;
    
    // ���ݻ� ���� �ʱ�ȭ�Ѵ�.
    float4 specular = float4(0.f, 0.f, 0.f, 0.f);
    
    // ����� ���� �� ���� ������ȯ
    float3 lightDir = -lightDirection;
    
    // �� �ȼ��� ���� ���� ����մϴ�.
    float lightIntensity = saturate(dot(input.vNormal, lightDir));
    
    // ���� ��ֿ� ���� ���߰� ������ �̾߱� �ϴ� ���ǹ���
    if (lightIntensity > 0.f)
    {
        color += (diffuseColor * lightIntensity);
        
        color = saturate(color);
        
        float3 reflection = normalize(2 * lightIntensity * input.vNormal - lightDir);
        
        specular = pow(saturate(dot(reflection, input.vViewDirection)), specularPower);
    }
    
    color = color * textureColor;
    
    output.Color = saturate(color + specular);
    output.Normal = float4(input.vNormal, 1.f);
    
    return output;
}