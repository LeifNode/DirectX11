//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************


cbuffer cbPerObject : register( b0 )
{
	float4x4 gBoxToWorld;
	float4x4 gBoxWorldViewProj;
};

struct VertexIn
{
	float3 PosL  : POSITION;
	float3 Normal: NORMAL;
	float3 Tangent : TANGENT;
    float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
	float3 Normal: NORMAL;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(gBoxWorldViewProj, float4(vin.PosL, 1.0f));
	
	vout.Normal = normalize(mul(gBoxToWorld, float4(vin.Normal, 0.0f)).xyz);

	// Just pass vertex color into the pixel shader.
    vout.Color = float4(1.0f, 1.0f, 1.0f, 1.0f); 
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float4 color = float4(0.05f, 0.05f, 0.05f, 0.0f) + pin.Color * saturate(dot(float3(0.5f, 1.0f, 0.5f), pin.Normal));

	return color;
}