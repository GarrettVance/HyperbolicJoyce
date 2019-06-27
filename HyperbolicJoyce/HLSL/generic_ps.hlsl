



cbuffer conbuf_Klein : register(b0)
{
    float2      obs_z;
    float2      obs_dir;
    float2      e_pixel_size;
    float2      e_alpha_channel;
};







Texture2D ObjTexture;
SamplerState ObjSamplerState;






struct PixelShaderInput
{
    float4 pos              : SV_POSITION;
    float3 color            : COLOR0;
    float2 v_texCoord       : TEXCOORD;
};








float4 main(PixelShaderInput input) : SV_TARGET
{
    //    This Pixel Shader depends on having 
    //    a CB Constant Buffer which is fed with the 
    //    variable value of alpha_channel for opacity / blending: 
    //      

   
    float4 gv_color = ObjTexture.Sample(ObjSamplerState, input.v_texCoord);
    float4 opacity_color = float4(gv_color.xyz, e_alpha_channel.x);

    return opacity_color;
}



