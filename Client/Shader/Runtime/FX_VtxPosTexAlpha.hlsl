#include "Shader_Defines.hlsli"


/* float2, float3, float4 == vector */
/* vector.x == vector.r */
/* vector.xy = 1.f;*/
/* float4x4 == matrix */
/* matrix._11_12 = 1.f; */

/* 셰이더의 전역변수 == 상수테이블(Constant Table) */
cbuffer Matrices : register(b0)
{
    matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
}

float g_fAlpha = 1.f;
float2 g_vMinUV = float2(0.f, 0.f);
float2 g_vMaxUV = float2(1.f, 1.f);

Texture2D g_Texture;

/* 정점의 변환(월드변환, 뷰변환, 투영변환.)을 수행한다. */
/* 정점의 구성정보를 추가, 삭제등의 변경을 수행한다.*/

/* 정점 네개. */
/* 인덱스 여섯개 .*/

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};


struct VPS_INOUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};



VPS_INOUT VS_MAIN(VS_IN In)
{
    VPS_INOUT Out = (VPS_INOUT) 0;

	/* In.vPosition * 월드 * 뷰 * 투영 */
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

/* 통과된 정점을 대기 .*/

/* 투영변환 : /w */ /* -> -1, 1 ~ 1, -1 */ 
/* 뷰포트변환-> 0, 0 ~ WINSX, WINSY */ 
/* 래스터라이즈 : 정점정보에 기반하여 픽셀의 정보를 만든다. */

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
};

/* 픽셀셰이더 : 픽셀의 색!!!! 을 결정한다. */
PS_OUT PS_MAIN(VPS_INOUT In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    bool bIsBetween = all(In.vTexcoord >= g_vMinUV) && all(In.vTexcoord <= g_vMaxUV);
    if (!bIsBetween)
        discard;
        
	/* 첫번째 인자의 방식으로 두번째 인자의 위치에 있는 픽셀의 색을 얻어온다. */
    vector vSourColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    if (vSourColor.a < 0.3f)
        discard;

    Out.vDiffuse = vSourColor;
    Out.vDiffuse.a = g_fAlpha;

    return Out;
}


technique11 DefaultTechnique
{
	/* 내가 원하는 특정 셰이더들을 그리는 모델에 적용한다. */
    pass Default
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		/* 렌더스테이츠 */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass UI
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		/* 렌더스테이츠 */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
