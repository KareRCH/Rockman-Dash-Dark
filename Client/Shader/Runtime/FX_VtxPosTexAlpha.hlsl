#include "Shader_Defines.hlsli"


/* float2, float3, float4 == vector */
/* vector.x == vector.r */
/* vector.xy = 1.f;*/
/* float4x4 == matrix */
/* matrix._11_12 = 1.f; */

/* ���̴��� �������� == ������̺�(Constant Table) */
cbuffer Matrices : register(b0)
{
    matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
}

float g_fAlpha = 1.f;
float2 g_vMinUV = float2(0.f, 0.f);
float2 g_vMaxUV = float2(1.f, 1.f);

Texture2D g_Texture;

/* ������ ��ȯ(���庯ȯ, �亯ȯ, ������ȯ.)�� �����Ѵ�. */
/* ������ ���������� �߰�, �������� ������ �����Ѵ�.*/

/* ���� �װ�. */
/* �ε��� ������ .*/

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

	/* In.vPosition * ���� * �� * ���� */
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

/* ����� ������ ��� .*/

/* ������ȯ : /w */ /* -> -1, 1 ~ 1, -1 */ 
/* ����Ʈ��ȯ-> 0, 0 ~ WINSX, WINSY */ 
/* �����Ͷ����� : ���������� ����Ͽ� �ȼ��� ������ �����. */

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
};

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN(VPS_INOUT In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    bool bIsBetween = all(In.vTexcoord >= g_vMinUV) && all(In.vTexcoord <= g_vMaxUV);
    if (!bIsBetween)
        discard;
        
	/* ù��° ������ ������� �ι�° ������ ��ġ�� �ִ� �ȼ��� ���� ���´�. */
    vector vSourColor = g_Texture.Sample(LinearSampler, In.vTexcoord);
    
    if (vSourColor.a < 0.3f)
        discard;

    Out.vDiffuse = vSourColor;
    Out.vDiffuse.a = g_fAlpha;

    return Out;
}


technique11 DefaultTechnique
{
	/* ���� ���ϴ� Ư�� ���̴����� �׸��� �𵨿� �����Ѵ�. */
    pass Default
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend_Add, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
		/* ������������ */
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
		/* ������������ */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
