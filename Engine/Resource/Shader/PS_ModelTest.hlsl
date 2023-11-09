Texture2D shaderTexture;
SamplerState SampleType;

// 빛 정보를 저장하는 버퍼
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
    // 베이스 컬러 세팅
    float4 textureColor = shaderTexture.Sample(SampleType, input.vTexCoord);
    
    // 기본 값을 환경광으로 설정
    float4 color = ambientColor;
    
    // 정반사 값을 초기화한다.
    float4 specular = float4(0.f, 0.f, 0.f, 0.f);
    
    // 계산을 위해 빛 벡터 방향전환
    float3 lightDir = -lightDirection;
    
    // 이 픽셀의 빛의 양을 계산합니다.
    float lightIntensity = saturate(dot(input.vNormal, lightDir));
    
    // 빛이 노멀에 대해 비추고 있음을 이야기 하는 조건문임
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