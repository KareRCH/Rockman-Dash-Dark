#include "Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix g_ProjMatrixInv, g_ViewMatrixInv;

vector g_vCamPosition;
float g_fFar = 1000.f;

Texture2D g_DepthTexture;
Texture2D g_FinalTexture;
Texture2D g_BloomTexture;

float g_fStart = 30.f;          // �Ȱ� ���� ����
float g_fRange = 100.f;         // ���� �Լ��� �Ȱ� ����
float g_fDensity = 0.01f;       // ���� �Լ��� �Ȱ� �β�
float g_fFarMinFog = 0.1f;      // Far���� �ش��ϴ� �ȼ��� ���� ���̴� �ּ�ġ
float4 g_vColor = vector(0.f, 0.f, 0.f, 1.f);       // �Ȱ� ��

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};


struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};



VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

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


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_LINEAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vFinal = g_FinalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    vector vBloom = g_BloomTexture.Sample(LinearSampler, In.vTexcoord);
    
    float fViewZ = vDepth.y * g_fFar;
    float fFarFogFactor = floor(vDepth.y) * g_fFarMinFog;   // 0, 1���� ��¦ ������ �ϴ� ����ġ
    //vector vWorldPos;

	/* ���������̽� ���� ��ġ. */
	/* ������ġ * ������� * �����* ������� / View.z */
    //vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    //vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    //vWorldPos.z = vDepth.x;
    //vWorldPos.w = 1.f;
    //
	///* �佺���̽� ���� ��ġ�� ������. */
	///* ������ġ * ������� * ����� */
    //vWorldPos = vWorldPos * fViewZ;
    //vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    //
	///* ���� ���� ��ġ�� ������. */
	///* ������ġ * ������� */
    //vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    //
    //float fLength = length((vWorldPos - g_vCamPosition).xyz);
    float fFogFactor = saturate((g_fRange - fViewZ) / (g_fRange - g_fStart)) - fFarFogFactor;
    
    Out.vColor = max(vector(fFogFactor * vFinal.xyz + (1.f - fFogFactor) * g_vColor.xyz, 1.f), vBloom);

    return Out;
}

PS_OUT PS_MAIN_EXPONENTIAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vFinal = g_FinalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    
    float fViewZ = vDepth.y * g_fFar;
    vector vWorldPos;

	/* ���������̽� ���� ��ġ. */
	/* ������ġ * ������� * �����* ������� / View.z */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepth.x;
    vWorldPos.w = 1.f;

	/* �佺���̽� ���� ��ġ�� ������. */
	/* ������ġ * ������� * ����� */
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* ���� ���� ��ġ�� ������. */
	/* ������ġ * ������� */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    float fLength = length((vWorldPos - g_vCamPosition).xyz);
    float fFogFactor = pow((1.f / 2.71828), fLength * g_fDensity);
    
    Out.vColor = vector(fFogFactor * vFinal.xyz + (1.f - fFogFactor) * g_vColor.xyz, 1.f);

    return Out;
}

PS_OUT PS_MAIN_SQUARE_EXPONENTIAL(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vFinal = g_FinalTexture.Sample(LinearSampler, In.vTexcoord);
    vector vDepth = g_DepthTexture.Sample(LinearSampler, In.vTexcoord);
    
    float fViewZ = vDepth.y * g_fFar;
    vector vWorldPos;

	/* ���������̽� ���� ��ġ. */
	/* ������ġ * ������� * �����* ������� / View.z */
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepth.x;
    vWorldPos.w = 1.f;

	/* �佺���̽� ���� ��ġ�� ������. */
	/* ������ġ * ������� * ����� */
    vWorldPos = vWorldPos * fViewZ;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* ���� ���� ��ġ�� ������. */
	/* ������ġ * ������� */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    float fLength = length((vWorldPos - g_vCamPosition).xyz);
    float fFogFactor = pow((1.f / 2.71828), pow((fLength * g_fDensity), 2.f));
    
    Out.vColor = vector(fFogFactor * vFinal.xyz + (1.f - fFogFactor) * g_vColor.xyz, 1.f);

    return Out;
}


technique11 DefaultTechnique
{
    pass Linear
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LINEAR();
    }

    pass Exponential
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EXPONENTIAL();
    }

    pass SquareExponential
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SQUARE_EXPONENTIAL();
    }
}
