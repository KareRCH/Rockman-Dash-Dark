// 행렬 변환
cbuffer MatrixBuffer : register(b0)
{
    float4x4 matWorld;
    float4x4 matView;
    float4x4 matProj;
};

// 카메라용 버퍼
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
    
    // 월드 정점 위치 계산
    float4 worldPosition = mul(float4(input.vPosition.xyz, 1.f), matWorld);
    
    // Look 벡터 설정
    output.vViewDirection = cameraPosition.xyz - worldPosition.xyz;
    
    // 뷰 방향 벡터 표준화
    output.vViewDirection = normalize(output.vViewDirection);
    
    return output;
}

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
    
    output.Color = saturate(color + specular);
    output.Normal = float4(input.vNormal, 1.f);
    
    return output;
}