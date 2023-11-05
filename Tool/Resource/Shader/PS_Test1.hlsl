struct PS_INPUT
{
    float4 vPosition : SV_Position0;
    float4 vColor : COLOR0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return input.vColor;
}