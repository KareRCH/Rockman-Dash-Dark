
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


// 뼈 행렬들
cbuffer BoneBuffer : register(b2)
{
    matrix g_matBones[256];
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
    int4 vBoneID : BONEID;
    float4 vWeight : WEIGHT;
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

VPS_INOUT VS_MAIN(VS_INPUT In)
{
    VPS_INOUT Out = (VPS_INOUT) 0;
    
    float4x4 matBoneTransform = { 1.f, 0.f, 0.f, 0.f,
                                  0.f, 1.f, 0.f, 0.f,
                                  0.f, 0.f, 1.f, 0.f,
                                  0.f, 0.f, 0.f, 1.f };
    
    // 하나라도 값이 들어가 있음
    //if (!all(input.vBoneID))
    {
        matBoneTransform = g_matBones[In.vBoneID[0]] * In.vWeight[0];
        matBoneTransform += g_matBones[In.vBoneID[1]] * In.vWeight[1];
        matBoneTransform += g_matBones[In.vBoneID[2]] * In.vWeight[2];
        matBoneTransform += g_matBones[In.vBoneID[3]] * In.vWeight[3];
    }
    
    matrix BoneWorldMatrix = mul(matBoneTransform, g_matWorld);
    matrix WVMatrix = mul(BoneWorldMatrix, g_matView);
    matrix WVPMatrix = mul(WVMatrix, g_matProj);
    
    Out.vPosition = mul(float4(In.vPosition.xyz, 1.f), WVPMatrix);
    Out.vNormal = normalize(mul(In.vNormal, (float3x3) BoneWorldMatrix));
    Out.vTexCoord = In.vTexCoord;
    
    // 월드 정점 위치 계산
    Out.vWorldPos = mul(float4(In.vPosition.xyz, 1.f), BoneWorldMatrix);
    Out.vProjPos = Out.vPosition;
    
    return Out;
}

//-------------------------------------------------

PS_OUTPUT PS_MAIN(VPS_INOUT In)
{
    PS_OUTPUT Out = (PS_OUTPUT)0;
    
    vector vMtrlDiffuse = g_texDiffuse.Sample(LinearSampler, In.vTexCoord);

    if (vMtrlDiffuse.a < 0.3f)
        discard;
    
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = vector(In.vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fFar, (float) g_iObjectID, 0.f);
    
    //float fDot = dot(normalize((In.vWorldPos - g_vCamPosition).xyz), Out.vNormal.xyz);
    //if (fDot < 0.18f && fDot > -0.18f)
    //    Out.vDiffuse.rgb = vector(0.f, 0.f, 0.f, 1.f).rgb;
    
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
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    
}