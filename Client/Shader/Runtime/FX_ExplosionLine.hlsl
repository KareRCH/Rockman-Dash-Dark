#include "Shader_Defines.hlsli"

// 행렬 변환
matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;

vector g_vCamPosition = vector(0.f, 0.f, 0.f, 0.f);
float g_fFar = 1000.f;

// 텍스처
Texture2D g_DiffuseTexture;
float g_fOffset = 0.f;
float g_fAlpha = 1.f;

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
};

struct PS_OUTPUT
{
    float4 vDiffuse : SV_TARGET0;
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
    
    return Out;
}


//-------------------------------------------------

PS_OUTPUT PS_MAIN(VPS_INOUT In)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;
    
    // 베이스 컬러 세팅
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(LinearBorderSampler_Alpha, float2(In.vTexCoord.x, In.vTexCoord.y - g_fOffset));
    
    if (vMtrlDiffuse.a < 0.01f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vDiffuse.a = g_fAlpha;
    
    return Out;
}

PS_OUTPUT PS_MAIN_INNER(VPS_INOUT In)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;
    
    // 베이스 컬러 세팅
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);
    
    if (vMtrlDiffuse.a < 0.01f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vDiffuse.a = g_fAlpha;
    
    return Out;
}


technique11 DefaultTechnique
{
    pass Line
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

    pass Inner
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_INNER();
    }
}