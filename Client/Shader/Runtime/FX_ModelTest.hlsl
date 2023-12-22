
#include "Shader_Defines.hlsli"

// 행렬 변환
cbuffer MatrixBuffer : register(b0)
{
    float4x4 g_matWorld;
    float4x4 g_matView;
    float4x4 g_matProj;
};

// 빛 정보를 저장하는 버퍼
cbuffer LightBuffer : register(b1)
{
    float4  g_colDiffuse;
    float4  g_colAmbient;
    float4  g_colSpecular;
    float   g_fSpecularPower;
    float3  g_vLightDir;
};


// 뼈 행렬들
cbuffer BoneBuffer : register(b2)
{
    float4x4 g_matBones[128];
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
    output.vViewDirection = g_matView[3].xyz - worldPosition.xyz;
    
    // 뷰 방향 벡터 표준화
    output.vViewDirection = normalize(output.vViewDirection);
    
    return output;
}

//-------------------------------------------------

PS_OUTPUT PS_MAIN(VPS_INOUT input)
{
    PS_OUTPUT output;
    
    // 베이스 컬러 세팅
    float4 textureColor = g_texDiffuse.Sample(DefaultSampler, input.vTexCoord);
    
    // 기본 값을 환경광으로 설정
    float4 color = g_colAmbient;
    
    // 정반사 값을 초기화한다.
    float4 specular = float4(0.f, 0.f, 0.f, 0.f);
    
    // 계산을 위해 빛 벡터 방향전환
    float3 lightDir = -normalize(g_vLightDir);
    
    // 이 픽셀의 빛의 양을 계산합니다.
    float lightIntensity = saturate(dot(input.vNormal, lightDir));
    
    // 빛이 노멀에 대해 비추고 있음을 이야기 하는 조건문임
    if (lightIntensity > 0.f)
    {
        color += (g_colDiffuse * lightIntensity);
        
        color = saturate(color);
        
        float3 reflection = normalize(2 * lightIntensity * input.vNormal - lightDir);
        
        specular = pow(saturate(dot(reflection, input.vViewDirection)), g_fSpecularPower);
    }
    
    color = color * textureColor;
    
    output.vColor = saturate(color + specular);
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
    
    // 베이스 컬러 세팅
    float4 textureColor = g_texDiffuse.Sample(DefaultSampler, input.vTexCoord);
    
    // 기본 값을 환경광으로 설정
    float4 color = g_colAmbient;
    
    // 정반사 값을 초기화한다.
    float4 specular = float4(0.f, 0.f, 0.f, 0.f);
    
    // 계산을 위해 빛 벡터 방향전환
    float3 lightDir = -normalize(g_vLightDir);
    
    // 이 픽셀의 빛의 양을 계산합니다.
    float lightIntensity = saturate(dot(input.vNormal, lightDir));
    
    // 빛이 노멀에 대해 비추고 있음을 이야기 하는 조건문임
    if (lightIntensity > 0.f)
    {
        color += (g_colDiffuse * lightIntensity);
        
        color = saturate(color);
        
        float3 reflection = normalize(2 * lightIntensity * input.vNormal - lightDir);
        
        specular = pow(saturate(dot(reflection, input.vViewDirection)), g_fSpecularPower);
    }
    
    color = color * textureColor;
    
    output.vColor = saturate(color + specular);
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