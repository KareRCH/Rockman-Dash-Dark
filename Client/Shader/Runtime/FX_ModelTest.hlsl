
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

// 텍스처
Texture2D g_texDiffuse;
sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
};

#ifdef TOOL
int g_iObjectID = -1;
#endif

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
    float3 vViewDirection : TEXCOORD1;
};

struct PS_OUTPUT
{
    float4 vColor : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
};



//-------------------------------------------------

VPS_INOUT VS_MAIN(VS_INPUT input)
{
    VPS_INOUT output = (VPS_INOUT) 0;
    
    float4x4 matBoneTransform = { 1.f, 0.f, 0.f, 0.f,
                                  0.f, 1.f, 0.f, 0.f,
                                  0.f, 0.f, 1.f, 0.f,
                                  0.f, 0.f, 0.f, 1.f };
    
    // 하나라도 값이 들어가 있음
    //if (!all(input.vBoneID))
    {
        matBoneTransform = g_matBones[input.vBoneID[0]] * input.vWeight[0];
        matBoneTransform += g_matBones[input.vBoneID[1]] * input.vWeight[1];
        matBoneTransform += g_matBones[input.vBoneID[2]] * input.vWeight[2];
        matBoneTransform += g_matBones[input.vBoneID[3]] * input.vWeight[3];
    }
    
    output.vPosition = mul(float4(input.vPosition.xyz, 1.f), mul(matBoneTransform, g_matWorld));
    output.vPosition = mul(output.vPosition, g_matView);
    output.vPosition = mul(output.vPosition, g_matProj);
    
    output.vNormal = mul(input.vNormal, (float3x3) mul(matBoneTransform, g_matWorld));
    output.vNormal = normalize(output.vNormal);
    
    output.vTexCoord = input.vTexCoord;
    
    // 월드 정점 위치 계산
    float4 worldPosition = mul(float4(input.vPosition.xyz, 1.f), g_matWorld);
    
    // Look 벡터 설정
    output.vViewDirection = g_vCamPosition.xyz - worldPosition.xyz;
    
    // 뷰 방향 벡터 표준화
    output.vViewDirection = normalize(output.vViewDirection);
    
    return output;
}

//-------------------------------------------------

PS_OUTPUT PS_MAIN(VPS_INOUT input)
{
    PS_OUTPUT output;
    
    vector vMtrlDiffuse = g_texDiffuse.Sample(DefaultSampler, input.vTexCoord);

    if (vMtrlDiffuse.a < 0.3f)
        discard;
    
    float fStair = 10.f;
    float fAlpha = vMtrlDiffuse.a;
    vMtrlDiffuse = float4(floor(vMtrlDiffuse.x * fStair), floor(vMtrlDiffuse.y * fStair), floor(vMtrlDiffuse.z * fStair), 1.f) / fStair;
    vMtrlDiffuse.a = fAlpha;

    float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(vector(input.vNormal, 1.f))), 0.f);

	/* 스펙큘러가 보여져야하는 영역에서는 1로, 아닌 영역에서는 0으로 정의되는 스펙큘러의 세기가 필요하다. */
    vector vReflect = reflect(normalize(g_vLightDir), normalize(vector(input.vNormal, 1.f)));

    float fSpecular = pow(max(dot(normalize(vector(input.vViewDirection, 1.f)) * -1.f, normalize(vReflect)), 0.f), 30.f);

    output.vColor = g_vLightDiffuse * vMtrlDiffuse * min((fShade + (g_vLightAmbient * g_vMtrlAmbient)), 1.f)
		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;

    
    output.vNormal = float4(input.vNormal, 1.f);
    
    return output;
}

// ----------------------- 툴 전용 --------------------------------



#ifdef TOOL
struct PS_OUTPUT_TOOL
{
    float4 vColor : SV_TARGET0;
    float4 vPosAndID : SV_TARGET1;
};

PS_OUTPUT_TOOL PS_MAIN_TOOL(VPS_INOUT input)
{
    PS_OUTPUT_TOOL output = (PS_OUTPUT_TOOL)0;
    
    vector vMtrlDiffuse = g_texDiffuse.Sample(DefaultSampler, input.vTexCoord);

    if (vMtrlDiffuse.a < 0.3f)
        discard;

    float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(vector(input.vNormal, 1.f))), 0.f);

	/* 스펙큘러가 보여져야하는 영역에서는 1로, 아닌 영역에서는 0으로 정의되는 스펙큘러의 세기가 필요하다. */
    vector vReflect = reflect(normalize(g_vLightDir), normalize(vector(input.vNormal, 1.f)));

    float fSpecular = pow(max(dot(normalize(vector(input.vViewDirection, 1.f)) * -1.f, normalize(vReflect)), 0.f), 30.f);

    output.vColor = g_vLightDiffuse * vMtrlDiffuse * min((fShade + (g_vLightAmbient * g_vMtrlAmbient)), 1.f)
		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    output.vNormal = float4(input.vNormal, 1.f);
    // 야매긴한데 툴 피킹은 오브젝트 ID를 렌더타겟에 저장한다. 위치값도 같이 전달한다.
    output.vPosAndID = float4(input.vPosition.xyz, float(g_iObjectID));
    
    return output;
}
#endif


technique11 DefaultTechnique
{
#ifdef TOOL
    // 툴에서 피킹용으로 쓰이는 패스
    pass Tool
    {
        SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_TOOL();
    }
#endif

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