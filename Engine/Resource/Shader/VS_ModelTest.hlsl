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

struct VS_INPUT
{
    float3 vPosition : POSITION0;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float3 vViewDirection : TEXCOORD1;
};

PS_OUTPUT main(VS_INPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
    
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