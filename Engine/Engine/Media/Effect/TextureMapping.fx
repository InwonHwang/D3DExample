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
string TextureMapping_Pass_0_Model : ModelData = ".\\Sphere.x";

float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;
float4x3 gMatrixPalette[64] : MatrixArray;

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

struct VS_SKINNEDINPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;	
	float4 mWeights : TEXCOORD1;
	float4 mBoneIndices : TEXCOORD2;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
};

VS_OUTPUT vs_main(VS_SKINNEDINPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;
	//int4 IndexVector = D3DCOLORtoUBYTE4(Input.mBoneIndices);

	float3 p = { 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < 4; ++i)
	{
		p += Input.mWeights[i] * mul(Input.mPosition, gMatrixPalette[Input.mBoneIndices[i]]);
	}

	Output.mPosition = mul(float4(p, 1), gWorldMatrix);
	Output.mPosition = mul(Output.mPosition, gViewMatrix);
	Output.mPosition = mul(Output.mPosition, gProjectionMatrix);
	Output.mTexCoord = Input.mTexCoord;

	return Output;
}
texture DiffuseMap_Tex;

sampler2D DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap_Tex);
};

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
};

float4 ps_main(PS_INPUT Input) : COLOR
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
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_main();
	}
}

