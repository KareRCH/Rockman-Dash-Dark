//#define TOOL // �� ������ ������ �� �ּ� Ǯ���

#include "Shader_Defines.hlsli"

cbuffer MatrixBuffer : register(b0)
{
    matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
}

cbuffer DirectionalLightBuffer : register(b1)
{
    float4 g_vLightDiffuse = float4(1.f, 1.f, 1.f, 1.f);
    float4 g_vLightAmbient = float4(1.f, 1.f, 1.f, 1.f);
    float4 g_vLightSpecular = float4(1.f, 1.f, 1.f, 1.f);
    float g_fLightRange = 30.f;
    float3 g_vLightDir = float3(1.f, -1.f, 1.f);
}

cbuffer PointLightBuffer
{
    float4 g_vLightPos = float4(50.f, 10.f, 50.f, 1.f);
}

cbuffer MaterialBuffer
{
    float4 g_vMtrlAmbient = float4(0.3f, 0.3f, 0.3f, 1.f);
    float4 g_vMtrlSpecular = float4(1.f, 1.f, 1.f, 1.f);
}

cbuffer BrushBuffer
{
    vector g_vBrushPos = vector(50.f, 0.f, 20.f, 1.f);
    float g_fBrushRange = 10.f;
}

vector g_vCamPosition;

Texture2D g_DiffuseTexture[2];
Texture2D g_MaskTexture;
Texture2D g_BrushTexture;

sampler DefaultSampler = sampler_state
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = wrap;
    AddressV = wrap;
};

#ifdef TOOL
int g_iObjectID = -1.f;
#endif

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VPS_INOUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};


VPS_INOUT VS_MAIN(VS_IN In)
{
    VPS_INOUT Out = (VPS_INOUT) 0;

	/* In.vPosition * ���� * �� * ���� */
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    return Out;
}

/* ����� ������ ��� .*/

/* ������ȯ : /w */ /* -> -1, 1 ~ 1, -1 */ 
/* ����Ʈ��ȯ-> 0, 0 ~ WINSX, WINSY */ 
/* �����Ͷ����� : ���������� ����Ͽ� �ȼ��� ������ �����. */


/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT PS_MAIN(VPS_INOUT In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* ù��° ������ ������� �ι�° ������ ��ġ�� �ִ� �ȼ��� ���� ���´�. */
    vector vSourDiffuse = g_DiffuseTexture[0].Sample(DefaultSampler, In.vTexcoord * 100.0f);
    vector vDestDiffuse = g_DiffuseTexture[1].Sample(DefaultSampler, In.vTexcoord * 100.0f);
    vector vBrush = vector(0.f, 0.f, 0.f, 0.f);
	// vector		vBrush = g_BrushTexture.Sample(DefaultSampler, In.vTexcoord);

    if (g_vBrushPos.x - g_fBrushRange < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPos.x + g_fBrushRange &&
		g_vBrushPos.z - g_fBrushRange < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPos.z + g_fBrushRange)
    {
        float2 vUV;

        vUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange)) / (2.f * g_fBrushRange);
        vUV.y = ((g_vBrushPos.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

        vBrush = g_BrushTexture.Sample(DefaultSampler, vUV);
    }

    vector vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexcoord);

    vector vMtrlDiffuse = vMask * vDestDiffuse + (1.f - vMask) * vSourDiffuse + vBrush;

    float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);

	/* ����ŧ���� ���������ϴ� ���������� 1��, �ƴ� ���������� 0���� ���ǵǴ� ����ŧ���� ���Ⱑ �ʿ��ϴ�. */
    float3 vLook = In.vWorldPos.xyz - g_vCamPosition.xyz;
    float3 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));

    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fLightRange);

    Out.vColor = g_vLightDiffuse * vMtrlDiffuse * min((fShade + (g_vLightAmbient * g_vMtrlAmbient)), 1.f)
		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    return Out;
}

PS_OUT PS_MAIN_POINT(VPS_INOUT In)
{
    PS_OUT Out = (PS_OUT) 0;

	/* ù��° ������ ������� �ι�° ������ ��ġ�� �ִ� �ȼ��� ���� ���´�. */
    vector vMtrlDiffuse = g_DiffuseTexture[0].Sample(DefaultSampler, In.vTexcoord * 100.0f);

    float3 vLightDir = float3((In.vWorldPos - g_vLightPos).xyz);


    float fAtt = max(g_fLightRange - length(vLightDir), 0.f) / g_fLightRange;

    float fShade = max(dot(normalize(vLightDir) * -1.f, normalize(In.vNormal)), 0.f);

	/* ����ŧ���� ���������ϴ� ���������� 1��, �ƴ� ���������� 0���� ���ǵǴ� ����ŧ���� ���Ⱑ �ʿ��ϴ�. */
    float3 vLook = In.vWorldPos.xyz - g_ViewMatrix[3].xyz;
    float3 vReflect = reflect(normalize(vLightDir), normalize(In.vNormal));

    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fLightRange);

    Out.vColor = (g_vLightDiffuse * vMtrlDiffuse * min((fShade + (g_vLightAmbient * g_vMtrlAmbient)), 1.f)
		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular) * fAtt;
    

    return Out;
}



//---------------------------------- �� ���� �Լ� --------------------


#ifdef TOOL
struct PS_OUT_TOOL
{
    float4 vColor       : SV_TARGET0;
    float4 vPosAndID    : SV_TARGET1;
};

/* �ȼ����̴� : �ȼ��� ��!!!! �� �����Ѵ�. */
PS_OUT_TOOL PS_MAIN_TOOL(VPS_INOUT In)
{
    PS_OUT_TOOL Out = (PS_OUT_TOOL) 0;

	/* ù��° ������ ������� �ι�° ������ ��ġ�� �ִ� �ȼ��� ���� ���´�. */
    vector vSourDiffuse = g_DiffuseTexture[0].Sample(DefaultSampler, In.vTexcoord * 100.0f);
    vector vDestDiffuse = g_DiffuseTexture[1].Sample(DefaultSampler, In.vTexcoord * 100.0f);
    vector vBrush = vector(0.f, 0.f, 0.f, 0.f);
	// vector		vBrush = g_BrushTexture.Sample(DefaultSampler, In.vTexcoord);

    if (g_vBrushPos.x - g_fBrushRange < In.vWorldPos.x && In.vWorldPos.x <= g_vBrushPos.x + g_fBrushRange &&
		g_vBrushPos.z - g_fBrushRange < In.vWorldPos.z && In.vWorldPos.z <= g_vBrushPos.z + g_fBrushRange)
    {
        float2 vUV;

        vUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushRange)) / (2.f * g_fBrushRange);
        vUV.y = ((g_vBrushPos.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

        vBrush = g_BrushTexture.Sample(DefaultSampler, vUV);
    }

    vector vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexcoord);

    vector vMtrlDiffuse = vMask * vDestDiffuse + (1.f - vMask) * vSourDiffuse + vBrush;

    float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);

	/* ����ŧ���� ���������ϴ� ���������� 1��, �ƴ� ���������� 0���� ���ǵǴ� ����ŧ���� ���Ⱑ �ʿ��ϴ�. */
    float3 vLook = In.vWorldPos.xyz - g_vCamPosition.xyz;
    float3 vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));

    float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fLightRange);

    Out.vColor = g_vLightDiffuse * vMtrlDiffuse * min((fShade + (g_vLightAmbient * g_vMtrlAmbient)), 1.f)
		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    // �� ��ŷ ������ ���� ���. ȭ�鿡 �׷����� �ȼ����� �������� ��ü�� ���� �� �� �ִ�.
    Out.vPosAndID = float4(In.vWorldPos.xyz, g_iObjectID);
    //Out.vColor = Out.vPosAndID;

    return Out;
}
#endif



technique11 DefaultTechnique
{
#ifdef TOOL
    // �� ���� �н�, ������ �� �� �̰ɷ� �н��� ������ ����
    pass Tool
    {
        SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		/* ������������ */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_TOOL();
    }
#endif

	// �⺻ �н�
    pass Default
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

		/* ������������ */
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
