
/* 컨스턴트 테이블*/
float4x4			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture				g_Texture;

float				g_fColorR = 0.f;
float				g_fColorG = 0.f;
float				g_fColorB = 0.f;
float				g_fAlpha = 0.f;

sampler DefaultSampler = sampler_state 
{
	texture = g_Texture;
	minfilter = linear;
	mipfilter = linear;
	magfilter = linear;
};


struct VS_IN
{
	float3	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{		
	VS_OUT			Out = (VS_OUT)0;	

	float4x4		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);	
	Out.vTexUV = In.vTexUV;

	return Out;
	
}

struct PS_IN
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector	vColor : COLOR;
};


vector PS_MAIN(PS_IN In) : COLOR
{
	vector		vColor = (vector)0;

	vColor = tex2D(DefaultSampler, In.vTexUV);	

	return vColor;
}


vector PS_MAIN_ATTACKED(PS_IN In) : COLOR
{
	vector		vColor = (vector)0;

	vColor = tex2D(DefaultSampler, In.vTexUV);

	vColor.r = 255;
	vColor.gb = 0;

	return vColor;
}

vector PS_MAIN_Glow(PS_IN In) : COLOR
{
	vector		vColor = (vector)0;

	vColor = tex2D(DefaultSampler, In.vTexUV);

	vColor.a = vColor.a * g_fAlpha;

	if (g_fAlpha >= 0.7)
	{
		vColor.r = g_fColorR;
		vColor.g = g_fColorG;
		vColor.b = g_fColorB;
	}

return vColor;
}

vector PS_MAIN_Freeze(PS_IN In) : COLOR
{
	vector		vColor = (vector)0;

	vColor = tex2D(DefaultSampler, In.vTexUV);

	vColor.rg = 0;
	vColor.b = 1 * In.vTexUV.y * 0.5f;

	return vColor;
}

technique DefaultTechnique
{
	/* 셰이더 기법의 캡슐화. */
	/* 그림자 + 법선맵핑 + 림라이트 */
	pass NormalState
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass AttackedState
	{
		AlphaBlendEnable = true;
		SrcBlend = Srcalpha;
		DestBlend = InvSrcAlpha;
		//AlphaTestEnable = true;
		//AlphaRef = 100;
		//AlphaRef = 5;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_ATTACKED();
	}

	pass GlowState
	{
		AlphaBlendEnable = true;
		SrcBlend = Srcalpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_Glow();
	}

	pass FreezeState
	{
		AlphaBlendEnable = true;
		SrcBlend = Srcalpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_Freeze();
	}
}


