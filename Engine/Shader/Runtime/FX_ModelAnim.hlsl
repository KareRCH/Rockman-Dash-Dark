#include "Shader_Defines.hlsli"

// 행렬 변환
matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;

// 뼈 행렬들
matrix g_BoneMatrices[200];

float g_fLightFar = 300.f;

vector g_vCamPosition = vector(0.f, 0.f, 0.f, 0.f);
float g_fFar = 1000.f;

// 색 블렌드 전용 색
vector g_vColorAdd = vector(1.f, 1.f, 1.f, 1.f);
float g_fColorAdd_Strength = 0.f;

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
    int4 vBoneID : BONEID;
    float4 vWeight : WEIGHT;
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
    
    float4x4 BoneMatrix = { 1.f, 0.f, 0.f, 0.f,
                            0.f, 1.f, 0.f, 0.f,
                            0.f, 0.f, 1.f, 0.f,
                            0.f, 0.f, 0.f, 1.f };
    
    // 하나라도 값이 들어가 있음
    //if (!all(input.vBoneID))
    {
        BoneMatrix = g_BoneMatrices[In.vBoneID[0]] * In.vWeight[0];
        BoneMatrix += g_BoneMatrices[In.vBoneID[1]] * In.vWeight[1];
        BoneMatrix += g_BoneMatrices[In.vBoneID[2]] * In.vWeight[2];
        BoneMatrix += g_BoneMatrices[In.vBoneID[3]] * In.vWeight[3];
    }
    
    matrix BoneWorldMatrix = mul(BoneMatrix, g_WorldMatrix);
    matrix WVMatrix = mul(BoneWorldMatrix, g_ViewMatrix);
    matrix WVPMatrix = mul(WVMatrix, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), WVPMatrix);
    Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), BoneWorldMatrix)).xyz;
    Out.vTexCoord = In.vTexCoord;
    
    // 월드 정점 위치 계산
    Out.vWorldPos = mul(vector(In.vPosition.xyz, 1.f), BoneWorldMatrix);
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

//-------------------------------------------------

PS_OUTPUT PS_MAIN(VPS_INOUT In)
{
    PS_OUTPUT Out = (PS_OUTPUT)0;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

    if (vMtrlDiffuse.a < 0.3f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, (float) g_iObjectID, 0.f);
    
    return Out;
}

PS_OUTPUT PS_MAIN_BLEND_ADD(VPS_INOUT In)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

    if (vMtrlDiffuse.a < 0.3f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse + g_vColorAdd * g_fColorAdd_Strength;
    Out.vNormal = vector(In.vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, (float) g_iObjectID, 0.f);
    
    return Out;
}

PS_OUTPUT PS_MAIN_SINGLE_COLOR(VPS_INOUT In)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;
    
    Out.vDiffuse = g_vColorAdd;
    Out.vNormal = vector(In.vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, (float) g_iObjectID, 0.f);
    
    return Out;
}


struct PS_OUT_SHADOW
{
    vector vLightDepth : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(VPS_INOUT In)
{
    PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0;

    Out.vLightDepth = In.vProjPos.w / g_fLightFar;
	
    return Out;
}


struct PS_OUT_NONLIGHT
{
    vector vColor : SV_TARGET0;
};

PS_OUT_NONLIGHT PS_MAIN_NONLIGHT(VPS_INOUT In)
{
    PS_OUT_NONLIGHT Out = (PS_OUT_NONLIGHT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexCoord);

    if (vMtrlDiffuse.a < 0.3f)
        discard;
    
    Out.vColor = vMtrlDiffuse;
	
    return Out;
}

technique11 DefaultTechnique
{
    // 기본 패스
    pass Default
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
    
    // 텍스처에 색 더하는 패스
    pass BlendAdd
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_ADD();
    }
    
    // LightDepth 패스
    pass Shadow
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }

    // 이펙트용 패스
    pass NonLight
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_NONLIGHT();
    }

    // 단일 색 덮어쓰기용
    pass SingleColor
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SINGLE_COLOR();
    }
}