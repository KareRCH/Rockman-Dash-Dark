#include "Shader_Defines.hlsli"

/*
모델의 외각선만을 그리도록 하는 셰이더
형태를 표현할 때 사용
*/

// 행렬 변환
cbuffer MatrixBuffer : register(b0)
{
    matrix g_WorldMatrix;
    matrix g_ViewMatrix;
    matrix g_ProjMatrix;
};

vector g_vCamPosition = vector(0.f, 0.f, 0.f, 0.f);
float g_fFar = 1000.f;

vector g_vColor = vector(1.f, 1.f, 1.f, 1.f);
float g_fAlpha = 1.f;
float g_fElipse = 0.1f;

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
    float4 vWorldPos : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 vDiffuse : SV_TARGET0;
};



//-------------------------------------------------

VPS_INOUT VS_MAIN(VS_INPUT In)
{
    VPS_INOUT Out = (VPS_INOUT) 0;
    
    matrix WVMatrix = mul(g_WorldMatrix, g_ViewMatrix);
    matrix WVPMatrix = mul(WVMatrix, g_ProjMatrix);
    
    Out.vPosition = mul(float4(In.vPosition.xyz, 1.f), WVPMatrix);
    Out.vNormal = normalize(mul(In.vNormal, (float3x3) g_WorldMatrix));
    
    // 월드 정점 위치 계산
    Out.vWorldPos = mul(float4(In.vPosition.xyz, 1.f), g_WorldMatrix);
    
    return Out;
}

//-------------------------------------------------

PS_OUTPUT PS_MAIN(VPS_INOUT In)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;
    
    float fDot = dot(normalize((In.vWorldPos - g_vCamPosition).xyz), In.vNormal);
    if (!(fDot < g_fElipse && fDot > -g_fElipse))
        discard;
    
    Out.vDiffuse = vector(g_vColor.xyz, g_fAlpha);
    
    return Out;
}

// ----------------------- 툴 전용 --------------------------------

technique11 DefaultTechnique
{
    // 기본 패스
    pass Default
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    
}