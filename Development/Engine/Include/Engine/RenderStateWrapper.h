#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Render
	*  @{
	*/


	/** 
		RHI状态基类
	*/
	template<typename InitializerType,typename RHIRefType>
	class TStaticStateRHI
	{
	public:

		static RHIRefType GetRHI()
		{
			static StaticStateResource Resource;
			return Resource.mStateRHI;
		};

		static void Commit( )
		{
			GetRHI( )->Commit( );
		}

	private:

		/** 
			RHI静态渲染状态
		*/
		class StaticStateResource : public RenderResource
		{
		public:

			RHIRefType mStateRHI;

			StaticStateResource()
			{
				Init();
			}

			// FRenderResource interface.
			virtual void InitRHI()
			{
				mStateRHI = InitializerType::CreateRHI();
			}
			virtual void ReleaseRHI()
			{
				mStateRHI.Release();
			}
		};
	};

	/**
	 * RHI采样器状态模板封装
	 * TStaticSamplerStateRHI<...>::GetStaticState() 返回RHISamplerStateRef
	 * 只能在渲染线程使用
	 */
	template<eSamplerFilter Filter=SF_Nearest,eSamplerAddressMode AddressU=AM_Wrap,eSamplerAddressMode AddressV=AM_Wrap,eSamplerAddressMode AddressW=AM_Wrap>
	class TStaticSamplerState : public TStaticStateRHI<TStaticSamplerState<Filter,AddressU,AddressV,AddressW>,RHISamplerStateRef>
	{
	public:
		static RHISamplerStateRef CreateRHI()
		{
			SamplerStateInitializer Initializer = { Filter, AddressU, AddressV, AddressW };
			return RHI->CreateSamplerState(Initializer);
		}
	};

	/**
	 * RHI光栅化状态模板封装
	 * TStaticRasterizerState<...>::GetStaticState() 返回RHIRasterizerStateRef
	 * 只能在渲染线程使用
	 */
	template<eRasterizerFillMode FillMode=FM_Solid,eRasterizerCullMode CullMode=CM_None>
	class TStaticRasterizerState : public TStaticStateRHI<TStaticRasterizerState<FillMode,CullMode>,RHIRasterizerStateRef>
	{
	public:
		static RHIRasterizerStateRef CreateRHI()
		{
			RasterizerStateInitializer Initializer = { FillMode, CullMode, 0, 0 };
			return RHI->CreateRasterizerState(Initializer);
		}
	};

	/**
	 * RHI深度缓冲状态模板缓冲
	 * TStaticDepthState<...>::GetStaticState() 返回RHIDepthStateRef
	 * 只能在渲染线程使用
	 */
	template<
		bool bEnableDepthWrite=true,
		eCompareFunction DepthTest=CF_LessEqual
		>
	class TStaticDepthState : public TStaticStateRHI<
		TStaticDepthState<
			bEnableDepthWrite,
			DepthTest
			>,
		RHIDepthStateRef
		>
	{
	public:
		static RHIDepthStateRef CreateRHI()
		{
			

			DepthStateInitializer Initializer =
			{
				bEnableDepthWrite,
				DepthTest
			};
			return RHI->CreateDepthState(Initializer);
		}
	};

	/**
	 * RHI模板封装
	 * TStaticStencilState<...>::GetStaticState() 将返回RHIStencilStateRef
	 * 只能在渲染线程使用
	 */
	template<
		bool bEnableFrontFaceStencil = false,
		eCompareFunction FrontFaceStencilTest = CF_Always,
		eStencilOp FrontFaceStencilFailStencilOp = SO_Keep,
		eStencilOp FrontFaceDepthFailStencilOp = SO_Keep,
		eStencilOp FrontFacePassStencilOp = SO_Keep,
		bool bEnableBackFaceStencil = false,
		eCompareFunction BackFaceStencilTest = CF_Always,
		eStencilOp BackFaceStencilFailStencilOp = SO_Keep,
		eStencilOp BackFaceDepthFailStencilOp = SO_Keep,
		eStencilOp BackFacePassStencilOp = SO_Keep,
		dword StencilReadMask = 0xFFFFFFFF,
		dword StencilWriteMask = 0xFFFFFFFF,
		dword StencilRef = 0
		>
	class TStaticStencilState : public TStaticStateRHI<
		TStaticStencilState<
			bEnableFrontFaceStencil,
			FrontFaceStencilTest,
			FrontFaceStencilFailStencilOp,
			FrontFaceDepthFailStencilOp,
			FrontFacePassStencilOp,
			bEnableBackFaceStencil,
			BackFaceStencilTest,
			BackFaceStencilFailStencilOp,
			BackFaceDepthFailStencilOp,
			BackFacePassStencilOp,
			StencilReadMask,
			StencilWriteMask,
			StencilRef
			>,
		RHIStencilStateRef
		>
	{
	public:
		static RHIStencilStateRef CreateRHI()
		{
			StencilStateInitializer Initializer =
			{
				bEnableFrontFaceStencil,
				FrontFaceStencilTest,
				FrontFaceStencilFailStencilOp,
				FrontFaceDepthFailStencilOp,
				FrontFacePassStencilOp,
				bEnableBackFaceStencil,
				BackFaceStencilTest,
				BackFaceStencilFailStencilOp,
				BackFaceDepthFailStencilOp,
				BackFacePassStencilOp,
				StencilReadMask,
				StencilWriteMask,
				StencilRef
			};
			return RHI->CreateStencilState(Initializer);
		}
	};

	/**
	 * RHI混合状态
	 * TStaticBlendState<...>::GetStaticState() 返回RHIBlendStateRef
	 * 只能在渲染线程使用
	 */
	template<
		eBlendOperation ColorBlendOp = BO_Add,
		eBlendFactor ColorSrcBlend = BF_One,
		eBlendFactor ColorDestBlend = BF_Zero,
		eBlendOperation AlphaBlendOp = BO_Add,
		eBlendFactor AlphaSrcBlend = BF_One,
		eBlendFactor AlphaDestBlend = BF_Zero,
		eCompareFunction AlphaTest = CF_Always,
		_byte AlphaRef = 255
		>
	class TStaticBlendState : public TStaticStateRHI<
		TStaticBlendState<ColorBlendOp,ColorSrcBlend,ColorDestBlend,AlphaBlendOp,AlphaSrcBlend,AlphaDestBlend,AlphaTest,AlphaRef>,
		RHIBlendStateRef
		>
	{
	public:
		static RHIBlendStateRef CreateRHI()
		{
			BlendStateInitializer Initializer =
			{
				ColorBlendOp,
				ColorSrcBlend,
				ColorDestBlend,
				AlphaBlendOp,
				AlphaSrcBlend,
				AlphaDestBlend,
				AlphaTest,
				AlphaRef
			};
			return RHI->CreateBlendState(Initializer);
		}
	};

	/** @} */
	/** @} */
}
