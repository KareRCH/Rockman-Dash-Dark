// 래스터 라이저
RasterizerState RS_Default
{
	FillMode = Solid;	
	CullMode = Back;
	FrontCounterClockwise = false;
};

RasterizerState RS_Fill_Wireframe
{
	FillMode = Wireframe;
	CullMode = Back;
	FrontCounterClockwise = false;
};

RasterizerState RS_Cull_None
{
	FillMode = Solid;
	CullMode = None;
	FrontCounterClockwise = false;
};


// 깊이 버퍼
DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};


DepthStencilState DSS_None
{
	DepthEnable = false;
	DepthWriteMask = zero;
};


// 블렌더 상태
BlendState BS_Default
{
	BlendEnable[0] = false;	
};

BlendState BS_AlphaBlend_Add
{
	BlendEnable[0] = true;	

	SrcBlend = SRC_ALPHA;
	DestBlend = Inv_Src_Alpha;
	BlendOp = add;
};
