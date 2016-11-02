#include "EnginePCH.h"
#include "Engine/FileVersion.h"
#include "NekoEngine_Render.h"
#include "NekoEngine_Material.h"

namespace NekoEngine
{
	ShaderMaster* GShaderMaster = null;

	#define SHADER_CACHE_FILE L"ShaderCache.cache"

	//////////////////////////////////////////////////////////////////////////
	// ShaderBinder
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( RenderShader )

	RenderShader::RenderShader()
	{
		::memset( &mModifyTime, 0, sizeof mModifyTime );
	}

	RenderShader::~RenderShader()
	{

	}


	void RenderShader::Commit()
	{
		if ( mShaderRHI.Valid() )
			mShaderRHI->Commit();
	}

	void RenderShader::Serialize( BinarySerializer& Ser )
	{
		if ( Ser.IsLoading() )
		{
			dword ShaderType = 0;
			HeapStream Stream;
			Ser << ShaderType
				<< mModifyTime
				<< Stream;

			// 从编译好的shader直接创建对象
			mShaderRHI = RHI->CreateShader( (eShaderType)ShaderType, Stream.GetBuffer(), Stream.GetSize() );
		}
		else
		{
			RHIShader* Shader = mShaderRHI;
			Assert( Shader != null );

			
			RefStream Stream( Shader->GetCodePointer(), Shader->GetCodeSize() );

			Ser << (dword)Shader->GetType()
				<< mModifyTime
				<< Stream;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// ShaderFactory
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( ShaderFactory )

	ShaderFactory::ShaderFactory()
	{
		mLogger = new SimpleLog;
		
	}

	void ShaderFactory::InitLog(  )
	{
		mLogger->SetPrefix( RTTI_INSTANCE_STRING(this) );
	}

	ShaderFactory::~ShaderFactory( )
	{
		delete mLogger;
		mLogger = null;
	}

	void ShaderFactory::DeleteShader_RenderThread( RenderShader* Binder )
	{
		ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(DeleteShader,RenderShader*,mBinder, Binder, { 

			delete mBinder;
		} );

	}


	//////////////////////////////////////////////////////////////////////////
	// ShaderMaster
	//////////////////////////////////////////////////////////////////////////
	ShaderMaster::ShaderMaster()
	{

	}


	ShaderMaster::~ShaderMaster( )
	{
		Clear( );
	}


	void ShaderMaster::Clear()
	{
		for ( ShaderFactoryMap::iterator it(mShaderFactoryMap);it;++it)
		{
			ShaderFactory* Factory = it.get_value();
			delete Factory;
		}

		mShaderFactoryMap.clear();

		// 线程删除Shader
		GRenderThread.Flush();
	}


	
	void ShaderMaster::InitializeShaderLibrary( bool UseShaderCache )
	{
		mUseShaderCache = UseShaderCache;
	

		RTTIGroup* Group = RTTIObject::GetGroup( L"ShaderFactory" );

		// 创建所有的Shader Factory
		for ( RTTIClass* ClassInfo = Group->GetFirstClass();
			ClassInfo != null;
			ClassInfo = Group->GetNextClass() )
		{
			ShaderFactory* Factory = RTTI_CAST(ShaderFactory, ClassInfo->CreateInstance( ) );

			Factory->InitLog();

			Factory->LoadCache();

			mShaderFactoryMap.set( ClassInfo, Factory );
		}

		
		// 对未编译，或者过期的进行编译
		Rebuild();
	}

	void ShaderMaster::Rebuild(  )
	{
		dword TotalCompiledFiles = 0;
		for ( ShaderFactoryMap::iterator it(mShaderFactoryMap);it;++it)
		{
			ShaderFactory* Factory = it.get_value();

			TotalCompiledFiles += Factory->Compile();

			if ( TotalCompiledFiles > 0 && mUseShaderCache )
			{
				Factory->SaveCache();
			}
		}		
	}

	ShaderFactory* ShaderMaster::GetFactory( RTTIClass* ClassInfo )
	{
		ShaderFactory** Factory = mShaderFactoryMap.find( ClassInfo );

		if ( Factory == null )
			return null;

		return *Factory;
	}



	void ShaderMaster::Tick( float elapse )
	{
		for ( ShaderFactoryMap::iterator it(mShaderFactoryMap);it;++it)
		{
			ShaderFactory* Factory = it.get_value();

			Factory->Tick( elapse );
		}
	}




}