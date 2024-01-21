#include "Shader_Defines.hlsli"

// 행렬 변환
cbuffer MatrixBuffer : register(b0)
{
    matrix g_matWorld;
    matrix g_matView;
    matrix g_matProj;
};

// 빛 정보를 저장하는 버퍼
cbuffer LightBuffer : register(b1)
{
    vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
    vector g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
    vector g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
    vector g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);
    
    vector g_vMtrlAmbient = vector(0.3f, 0.3f, 0.3f, 1.f);
    vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
};

vector g_vCamPosition = vector(0.f, 0.f, 0.f, 0.f);
float g_fFar = 1000.f;

// 텍스처
Texture2D g_texDiffuse;

int g_iObjectID = -1;

//-------------------------------------------------

struct VS_INPUT
{
    float3 vPosition : POSITION0;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float3 vTangent : TANGENT0;
};

struct VPS_INOUT
{
    float4 vPosition : SV_POSITION0;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUTPUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

//-------------------------------------------------

VPS_INOUT VS_MAIN(VS_INPUT input)
{
    VPS_INOUT output = (VPS_INOUT) 0;
    
    output.vPosition = mul(float4(input.vPosition.xyz, 1.f), g_matWorld);
    output.vPosition = mul(output.vPosition, g_matView);
    output.vPosition = mul(output.vPosition, g_matProj);
    
    output.vNormal = mul(input.vNormal, (float3x3) g_matWorld);
    output.vNormal = normalize(output.vNormal);
    
    output.vTexCoord = input.vTexCoord;
    
    // 월드 정점 위치 계산
    output.vWorldPos = mul(float4(input.vPosition.xyz, 1.f), g_matWorld);
    output.vProjPos = output.vPosition;
    
    return output;
}

//-------------------------------------------------

PS_OUTPUT PS_MAIN(VPS_INOUT input)
{
    PS_OUTPUT output;
    
    // 베이스 컬러 세팅
    float4 vMtrlDiffuse = g_texDiffuse.Sample(LinearSampler, input.vTexCoord);
    
    if (vMtrlDiffuse.a < 0.3f)
        discard;
    
    output.vDiffuse = vMtrlDiffuse;
    output.vNormal = vector(input.vNormal * 0.5f + 0.5f, 0.f);
    output.vDepth = vector(input.vProjPos.z / input.vProjPos.w, input.vProjPos.w / g_fFar, (float) g_iObjectID, 1.f);
    
    return output;
}


technique11 DefaultTechnique
{
    pass Model
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}