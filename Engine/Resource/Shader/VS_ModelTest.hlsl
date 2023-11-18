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
    float3  vPosition : POSITION0;
    float3  vNormal : NORMAL;
    float2  vTexCoord : TEXCOORD0;
    float3  vTangent : TANGENT0;
    int     vBoneID[2] : BONEID0;
    float4  vWeight[2] : WEIGHT0;
};

struct VS_OUTPUT
{
    float4 vPosition : SV_POSITION0;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float3 vViewDirection : TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
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