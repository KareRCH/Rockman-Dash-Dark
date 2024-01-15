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

// 텍스처
Texture2D g_texDiffuse;
sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
};


//-------------------------------------------------

struct VS_INPUT
{
    float3 vPosition : POSITION0;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float3 vTangent : TANGENT0;
};

struct PS_INPUT
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

PS_INPUT VS_MAIN(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    output.vPosition = mul(float4(input.vPosition.xyz, 1.f), g_matWorld);
    output.vPosition = mul(output.vPosition, g_matView);
    output.vPosition = mul(output.vPosition, g_matProj);
    
    output.vNormal = mul(input.vNormal, (float3x3) g_matWorld);
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

PS_OUTPUT PS_MAIN(PS_INPUT input)
{
    PS_OUTPUT output;
    
    // 베이스 컬러 세팅
    float4 vMtrlDiffuse = g_texDiffuse.Sample(DefaultSampler, input.vTexCoord);
    
    if (vMtrlDiffuse.a < 0.3f)
        discard;
    
    float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(vector(input.vNormal, 1.f))), 0.f);
    
    // 정반사 값을 초기화한다.
    vector vReflect = reflect(normalize(g_vLightDir), normalize(vector(input.vNormal, 1.f)));
    float fSpecular = pow(max(dot(normalize(vector(input.vViewDirection, 1.f)) * -1.f, normalize(vReflect)), 0.f), 30.f);
    
    float fStair = 5.f;
    vector vLight = g_vLightDiffuse * min((fShade + (g_vLightAmbient * g_vMtrlAmbient)), 1.f)
		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    vector vColor = vMtrlDiffuse * vector(ceil(vLight.x * fStair),
                    ceil(vLight.y * fStair), ceil(vLight.z * fStair), fStair) / fStair;
    
    output.vColor = vColor;
    output.vNormal = float4(input.vNormal, 1.f);
    
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

    pass Particle
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}