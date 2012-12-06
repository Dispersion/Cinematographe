Texture2D shaderTexture;
SamplerState SampleType;

cbuffer cbPerObject
{
	float4x4 WVP;
};

struct VOut
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

VOut VShader(float4 position : POSITION, float2 tex : TEXCOORD0)
{
    VOut output;

    output.position = mul(position, WVP);
    output.tex = tex;

    return output;
}

float4 PShader(float4 position : SV_POSITION, float2 tex : TEXCOORD0) : SV_TARGET
{
    return shaderTexture.Sample(SampleType, tex);
}