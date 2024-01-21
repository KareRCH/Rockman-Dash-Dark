#include "Shader_Defines.hlsli"



cbuffer WVP : register(b0)
{
    matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
};

TextureCube		g_Texture;

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vTexcoord : TEXCOORD0;
};


struct VPS_INOUT
{
	float4		vPosition : SV_POSITION;
	float3		vTexcoord : TEXCOORD0;	
};



VPS_INOUT VS_MAIN(VS_IN In)
{
    VPS_INOUT Out = (VPS_INOUT) 0;

	/* In.vPosition * ���� * �� * ���� */
	matrix		matWV, matWVP;

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
	float4		vDiffuse : SV_TARGET0;
};

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN(VPS_INOUT In)
{
	PS_OUT		Out = (PS_OUT)0;

    Out.vDiffuse = g_Texture.Sample(LinearSampler, In.vTexcoord);
    //Out.vDiffuse = float4(1.f, 0.f, 0.f, 1.f);

	return Out;
}


technique11 DefaultTechnique
{
	pass Sky
	{
		SetRasterizerState(RS_Cull_None);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

}
