SamplerState gSampler;

Texture2D gTexColor;
Texture2D gTexNormal;
Texture2D gTexPosition;

// 행렬 변환 정보
cbuffer Transform
{
    matrix gWorld; // 월드행렬
    matrix gView; // 뷰행렬
    matrix gProjection; // 투영행렬
    matrix gWorldViewProjection; // 투영까지 이미 계산된 행렬(아직 안씀)
};


struct PS_INPUT
{
    float4 LocalPos : LOCAL_POS;
    float4 Position : SV_Position;
    float3 Normal : NORMAL;
    float3 Trangent : TANGENT;
    float2 Tex : TEXCOORD;
};


struct PS_OUTPUT
{
    float4 Position : SV_TARGET0;
	float4 Normal : SV_TARGET1;
	float4 Color : SV_TARGET2;
};

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
    output.Position = mul(input.LocalPos, gWorld);
    output.Normal = float4(input.Normal, 1.f);
    output.Color = float4(1.f, 1.f, 1.f, 1.f);
	
	return output;
}

PS_OUTPUT LightingPS(PS_INPUT input)
{
    PS_OUTPUT output;
    float3 normal = gTexNormal.Sample(gSampler, input.Tex).rgb;
    float3 diffuse = gTexColor.Sample(gSampler, input.Tex).rgb;

    if (length(normal) > 0.0f)
    {
        float3 lightDir = normalize(float3(1.0f, 1.0f, 1.0f));
        float3 position = float3(gView._41, gView._42, gView._43);

        float lambertian = max(dot(lightDir, normal), 0.0f);
        float specular = 0.0f;

		[flatten]
        if (lambertian > 0.0f)
        {
            float3 viewDir = normalize(-position);
            float3 halfDir = normalize(lightDir + viewDir);
            float specAngle = max(dot(halfDir, normal), 0.0f);
            specular = pow(specAngle, 100.0f);
        }

        float3 colorLinear = lambertian * diffuse + specular * float3(1.0f, 1.0f, 1.0f);
        output.Color = float4(pow(colorLinear, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f)), 1.0f);
        return output;
    }

    output.Color = float4(diffuse, 1.0f);
    
    return output;
}