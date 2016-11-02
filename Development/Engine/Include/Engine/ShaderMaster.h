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
		Shader类绑定信息
	*/
	struct ShaderMetaInfo 
	{
		/// 类型
		eShaderType mType;

		/// Shader源码位置
		const wchar* mSourceFile;

		/// 源码入口
		const wchar* mEntryName;
		

		ShaderMetaInfo(eShaderType Type, const wchar* SourceFile, const wchar* EntryName )
			: mType( Type )
			, mSourceFile( SourceFile )
			, mEntryName( EntryName )
		{
			
		}

	};


	/** 
		RHIShader封装层
		@remark
			在渲染线程使用的Shader类
	*/
	class RenderShader : public RTTIObject
	{
	public:
		DECLARE_RTTI_CLASS( RenderShader, RTTIObject,ClassFlag_Intrinsic| ClassFlag_Serializable )
		RenderShader( );

		virtual ~RenderShader( );

		/** 
			提交Shader
		*/
		virtual void Commit( );		

		/** 
			获取Shader类绑定信息
		*/
		virtual ShaderMetaInfo* GetShaderMetaInfo( ){ return null;}

		/** 
			是否显示反汇编
		*/
		virtual bool ShowDisassembleCode( ){ return false; }

		/// 二进制序列化
		virtual void Serialize( BinarySerializer& Ser );

		/** 
			是否材质Shader
		*/
		virtual bool IsMaterialShader( ){ return false; }

		/** 
			获取RHIShader
		*/
		RHIShader* GetRHIShader( ){ return mShaderRHI; }

	protected:
		/** 
			预处理宏回调
		*/
		virtual void PreprocessMacro( ShaderCompilerEnvironment& CompilerEnvironment ){ }

		/** 
			绑定Shader参数
		*/
		virtual void BindShaderParameter( eShaderType ShaderType, const ShaderParameterMap& ParameterMap ){}	


	protected:
		friend class GlobalShaderFactory;
		RHIShaderRef mShaderRHI;
		FileTime	 mModifyTime;
	};

	/// 绑定Shader信息到类
	#define DECLARE_SHADER_METAINFO( ShaderType, SourceFile, EntryName ) \
	public:\
	virtual ShaderMetaInfo* GetShaderMetaInfo( ) \
	{ static ShaderMetaInfo metainfo( ShaderType, SourceFile, EntryName ); return &metainfo; }
		
	
	/** 
		Shader工厂
	*/
	class ShaderFactory : public RTTIObject
	{
		DECLARE_RTTI_ABSTRACT( ShaderFactory, RTTIObject, ClassFlag_Intrinsic|ClassFlag_Serializable )

	public:
		ShaderFactory( );
		virtual ~ShaderFactory( );

		/** 
			编译Shader
		*/
		virtual dword Compile( ) = 0;

		/// 二进制序列化
		virtual void Serialize( BinarySerializer& Ser ) = 0;

		/// 清除所有Shader
		virtual void Clear( ) = 0;

		/// 更新
		virtual void Tick( float Elapse ){ }

		/// 加载缓冲
		virtual void LoadCache( ) = 0;

		/// 保存换成
		virtual void SaveCache( ) = 0;

		void InitLog( );

		/// 在渲染线程中删除RHIShader
		void DeleteShader_RenderThread( RenderShader* Binder );

	protected:
		Logger* mLogger;
	};

	/** 
		Shader管理器
	*/
	class ShaderMaster
	{
	public:
		ShaderMaster( );

		virtual ~ShaderMaster();

		/** 
			初始化Shader库
			@param
				UseShaderCache 是否编译Shader后缓冲到磁盘提升下次加载速度
		*/
		void InitializeShaderLibrary( bool UseShaderCache );		

		/// 重编译
		void Rebuild( );

		/// 删除所有Shader工厂
		void Clear(  );

		/// 更新
		void Tick( float elapse );

		/** 
			获取一个Shader工厂
			@param
				工厂类信息
			@return
				Shader工厂
		*/
		ShaderFactory* GetFactory( RTTIClass* ClassInfo );

		template<typename TClass>
		TClass* GetFactory( )
		{
			return RTTI_CAST(TClass, GetFactory( RTTI_CLASS_INFO( TClass) ) );
		}

		
	private:
		bool mUseShaderCache;
		typedef dynamic_hash<RTTIClass*, ShaderFactory*> ShaderFactoryMap;
		ShaderFactoryMap mShaderFactoryMap;
	};


	extern ShaderMaster* GShaderMaster;

	/** @} */
	/** @} */
}
