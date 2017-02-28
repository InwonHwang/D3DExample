//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// TextureMapping
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;
float4x4 gFinalTransforms[29] : BoneMatrixArray;

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

struct VS_SKINNEDINPUT
{
	float4 position : POSITION;
	float4 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float4 weights : BLENDWEIGHT;
	unsigned int4 boneIndices : BLENDINDICES;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT VertexBlend(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	/*float4 p = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float lastWeight = 0.0f;

	for (int i = 0; i < 4; ++i)
	{
		lastWeight += input.weights[i];
		p += input.weights[i] * mul(input.position, gFinalTransforms[input.boneIndices[i]]);
	}
	p.w = 1.0f;*/

	//output.mPosition = mul(p, gWorldMatrix);
	output.mPosition = mul(input.mPosition, gWorldMatrix);
	output.mPosition = mul(output.mPosition, gViewMatrix);
	output.mPosition = mul(output.mPosition, gProjectionMatrix);
	output.mTexCoord = input.mTexCoord;
	return output;
}

texture DiffuseMap_Tex : DiffuseMap;

sampler2D DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap_Tex);
};

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
};

float4 TextureMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
	return albedo.rgba;
}

//--------------------------------------------------------------//
// Technique Section for TextureMapping
//--------------------------------------------------------------//
technique TextureMapping
{
	pass Pass_0
	{
		VertexShader = compile vs_2_0 VertexBlend();
		PixelShader = compile ps_2_0 TextureMapping_Pass_0_Pixel_Shader_ps_main();
	}
}

