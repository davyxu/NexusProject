#pragma once

namespace NekoEngine
{
	enum eMaterialValueType
	{
		MVT_None,
		MVT_Float,
		MVT_Float2,
		MVT_Float3,
		MVT_Float4,
	};

	enum eMaterialValueMask
	{
		MVM_RGBA,
		MVM_RGB,
		MVM_R,
		MVM_G,
		MVM_B,
		MVM_A,
	};

	enum eMaterialMathOperator
	{
		MMO_Add,
		MMO_Sub,
		MMO_Mul,
		MMO_Div,
		MMO_Dot,
	};


}
