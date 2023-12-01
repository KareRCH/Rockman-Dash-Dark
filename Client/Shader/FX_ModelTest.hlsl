
// ��� ��ȯ
cbuffer MatrixBuffer : register(b0)
{
    float4x4 g_matWorld;
    float4x4 g_matView;
    float4x4 g_matProj;
};

// �� ������ �����ϴ� ����
cbuffer LightBuffer
{
    float4  g_colDiffuse;
    float4  g_colAmbient;
    float4  g_colSpecular;
    float   g_fSpecularPower;
    float3  g_vLightDir;
};


// �� ��ĵ�
cbuffer BoneBuffer
{
    float4x4 g_matBones[128];
};


// �ؽ�ó
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
    int4 vBoneID : BONEID;
    float4 vWeight : WEIGHT;
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
    
    float4x4 matBoneTransfrom = { 1.f, 0.f, 0.f, 0.f,
                                  0.f, 1.f, 0.f, 0.f,
                                  0.f, 0.f, 1.f, 0.f,
                                  0.f, 0.f, 0.f, 1.f };
    
    // �ϳ��� ���� �� ����
    if (!all(input.vWeight))
    {
        matBoneTransfrom = g_matBones[input.vBoneID[0]] * input.vWeight[0];
        matBoneTransfrom += g_matBones[input.vBoneID[1]] * input.vWeight[1];
        matBoneTransfrom += g_matBones[input.vBoneID[2]] * input.vWeight[2];
        matBoneTransfrom += g_matBones[input.vBoneID[3]] * input.vWeight[3];
    }
    
    output.vPosition = mul(float4(input.vPosition.xyz + matBoneTransfrom[3].xyz, 1.f), g_matWorld);
    output.vPosition = mul(output.vPosition, g_matView);
    output.vPosition = mul(output.vPosition, g_matProj);
    
    output.vNormal = mul(input.vNormal, (float3x3) g_matWorld);
    output.vNormal = normalize(output.vNormal);
    
    output.vTexCoord = input.vTexCoord;
    
    // ���� ���� ��ġ ���
    float4 worldPosition = mul(float4(input.vPosition.xyz, 1.f), g_matWorld);
    
    // Look ���� ����
    output.vViewDirection = g_matView[3].xyz - worldPosition.xyz;
    
    // �� ���� ���� ǥ��ȭ
    output.vViewDirection = normalize(output.vViewDirection);
    
    return output;
}

//-------------------------------------------------

PS_OUTPUT PS_MAIN(PS_INPUT input)
{
    PS_OUTPUT output;
    
    // ���̽� �÷� ����
    float4 textureColor = g_texDiffuse.Sample(DefaultSampler, input.vTexCoord);
    
    // �⺻ ���� ȯ�汤���� ����
    float4 color = g_colAmbient;
    
    // ���ݻ� ���� �ʱ�ȭ�Ѵ�.
    float4 specular = float4(0.f, 0.f, 0.f, 0.f);
    
    // ����� ���� �� ���� ������ȯ
    float3 lightDir = -normalize(g_vLightDir);
    
    // �� �ȼ��� ���� ���� ����մϴ�.
    float lightIntensity = saturate(dot(input.vNormal, lightDir));
    
    // ���� ��ֿ� ���� ���߰� ������ �̾߱� �ϴ� ���ǹ���
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


technique11 DefaultTechnique
{
    pass Model
    {
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