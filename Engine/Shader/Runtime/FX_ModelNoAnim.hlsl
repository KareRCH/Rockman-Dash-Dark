#include "Shader_Defines.hlsli"

// 행렬 변환
cbuffer MatrixBuffer : register(b0)
{
    matrix g_WorldMatrix;
    matrix g_ViewMatrix;
    matrix g_ProjMatrix;
};

vector g_vCamPosition = vector(0.f, 0.f, 0.f, 0.f);
float g_fFar = 1000.f;

// 텍스처
Texture2D g_DiffuseTexture;

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
    float4 vPosition : SV_POSITION;
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

VPS_INOUT VS_MAIN(VS_INPUT In)
{
    VPS_INOUT Out = (VPS_INOUT) 0;
    
    Out.vPosition = mul(float4(In.vPosition.xyz, 1.f), g_WorldMatrix);
    Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);
    
    Out.vNormal = mul(In.vNormal, (float3x3) g_WorldMatrix);
    Out.vNormal = normalize(Out.vNormal);
    
    Out.vTexCoord = In.vTexCoord;
    
    // 월드 정점 위치 계산
    Out.vWorldPos = mul(float4(In.vPosition.xyz, 1.f), g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

//-------------------------------------------------

PS_OUTPUT PS_MAIN(VPS_INOUT In)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;
    
    // 베이스 컬러 세팅
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
    
    if (vMtrlDiffuse.a < 0.3f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, (float) g_iObjectID, 1.f);
    
    return Out;
}

struct PS_OUTPUT_NONLIGHT
{
    float4 vColor : SV_TARGET0;
};

PS_OUTPUT_NONLIGHT PS_MAIN_NONLIGHT(VPS_INOUT In)
{
    PS_OUTPUT_NONLIGHT Out = (PS_OUTPUT_NONLIGHT) 0;
    
    // 베이스 컬러 세팅
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
    
    if (vMtrlDiffuse.a < 0.3f)
        discard;
    
    Out.vColor = vMtrlDiffuse;
    
    return Out;
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
    pass NonLight
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONLIGHT();
    }
}