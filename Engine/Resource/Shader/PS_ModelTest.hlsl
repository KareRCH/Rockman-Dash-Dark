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
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float3 vViewDirection : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_TARGET
{    
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
    
    color = saturate(color + specular);
    
    return color;
}