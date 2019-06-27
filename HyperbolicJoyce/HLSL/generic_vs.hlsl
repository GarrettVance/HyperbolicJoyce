// A constant buffer that stores the three basic column-major matrices for composing geometry.
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};






struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 color : COLOR0;
};







struct PixelShaderInput
{
    float4 pos              : SV_POSITION;
    float3 color            : COLOR0;
    float2 v_texCoord       : TEXCOORD;
};






PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;

    float4 pos_model_space = float4(input.pos, 1.0f);

    // Transform the vertex position into projected (aka "clip") space; 

    float4 pos_world_space = mul(pos_model_space, model);

    float4 pos_view_space = mul(pos_world_space, view);

    float4 pos_clip_space = mul(pos_view_space, projection);

    output.pos = pos_clip_space;

	output.color = input.color;

    output.v_texCoord = float2(
        (1.0 + input.pos.x) / 2.0, 
        (1.0 - input.pos.y) / 2.0
    );

	return output;
}




