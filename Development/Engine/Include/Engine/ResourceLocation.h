#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/


	/** 
		资源类型
	*/
	enum eResourceType
	{
		RFT_Unknown = 0,
		RFT_SkeletonAnimation,	///< SA 骨骼动画
		RFT_SkeletonMesh,		///< SK 骨骼模型
		RFT_StaticMesh,			///< SM 静态模型
		RFT_Texture,			///< TX 纹理
		RFT_MaterialInstance,	///< MI 材质实例
		RFT_Material,			///< MT 材质描述
		RFT_ShaderFile,			///< SF Shader文件
		RFT_MaxResourceType,
	};

	/** 
		资源名解析类
		@remark
			资源名是将相对路径中的分隔符转为.的资源名
			因此要求资源名中不能携带点
	*/
	class ResourceLocation
	{
	public:
		/**
			修改资源类型名
			@param
				ResourceName 原资源名
			@param
				OutResourceName 输出资源名
			@param
				Type 资源类型
			@return
				资源名称是否发生解析错误
		*/
		static bool ModifyType( const WString& ResourceName, WString& OutResourceName, eResourceType Type );

		/**
			将资源名解析为平台路径
			@param
				ResourceName 资源名
			@return
				平台陆军
		*/
		static WString Resovle(const WString& ResourceName );

		/**
			将平台路径转换为资源名
			@param
				FileName 绝对路径
			@return
				资源名
		*/
		static WString Build( const WString& FileName );

		/**
			解析资源名里资源类型
			@param
				ResourceName 资源名
			@return
				资源类型
		*/
		static eResourceType ParseType( const WString& ResourceName );

		/**
			获取资源类型的扩展名
			@param
				Type 资源类型
			@return
				扩展名,例如: .SA
		*/
		static WString GetExtension( eResourceType Type );
	};


	/** 
		资源的自动标记
	*/
	template<typename TClass>
	class ResourcePtrMarker
	{
	public:
		ResourcePtrMarker( )
			: mIsDirty( false )
		{

		}

		virtual ~ResourcePtrMarker( ){}


		void operator = ( const TClass& Resource )
		{
			mResource = Resource;
			mIsDirty = true;
		}

		/** 
			资源是否有效
		*/
		bool Valid( )
		{
			return mResource.Valid( );
		}

		/** 
			资源是否为空
		*/
		bool Empty( )
		{
			return mResource.Empty( );
		}

		/** 
			资源是否准备好, 如果资源被重置,将变脏,没有准备好
		*/
		bool IsResourceReady( )
		{
#ifdef _DEBUG			

			if ( mIsDirty )
				return false;

			if ( !mResource.Valid() )
				return false;

			return mResource->IsResourceReady( );
#else
			return !mIsDirty && mResource.Valid() && mResource->IsResourceReady( );
#endif

			
		}

		/** 
			资源是否脏
		*/
		bool IsDirty( )
		{
			return mIsDirty;
		}

		/** 
			资源是否可以更新
		*/
		bool CheckReadyToUpdate( )
		{
			return mIsDirty && mResource.Valid() && mResource->IsResourceReady( );
		}

		/** 
			重置脏标志
		*/
		void ClearFlag( )
		{
			mIsDirty = false;
		}

		TClass& GetResource( )
		{
			return mResource;
		}

		const TClass& GetResource( ) const
		{
			return mResource;
		}

	private:
		TClass	mResource;
		bool	mIsDirty;
	};

	/** @} */
	/** @} */
}
