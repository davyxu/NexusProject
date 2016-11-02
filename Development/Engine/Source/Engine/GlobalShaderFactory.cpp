#include "EnginePCH.h"
#include "NekoEngine_Render.h"

namespace NekoEngine
{
	IMPLEMENT_RTTI_CLASS( GlobalShaderFactory);


	GlobalShaderFactory::~GlobalShaderFactory()
	{
		Clear();
	}

	RenderShader* GlobalShaderFactory::Find( RTTIClass* ClassInfo )
	{
		RenderShader** binder = mGlobalShaderMap.find( ClassInfo );

		if ( binder == null )
			return null;

		return *binder;
	}

	void GlobalShaderFactory::Serialize( BinarySerializer& Ser )
	{
		typedef std::list<RenderShader*> ShaderBinderList;
		ShaderBinderList SerializeList;

		if ( Ser.IsLoading() )
		{
			Ser << SerializeList;

			for ( ShaderBinderList::iterator it = SerializeList.begin();
				it != SerializeList.end();
				++it )
			{
				RenderShader* Binder = *it;
				mGlobalShaderMap.set( RTTI_INSTANCE_CLASS( Binder ), Binder  );
			}
		}
		else
		{
			for ( GlobalShaderMap::iterator it(mGlobalShaderMap);it;++it)
			{
				RenderShader* Binder = it.get_value();
				SerializeList.push_back( Binder );
			}

			Ser << SerializeList;
		}
	}

	#define GLOBAL_SHADER_CACHEFILE L"GlobalShaderCache.cache"

	void GlobalShaderFactory::LoadCache( )
	{
		FileStream File( GLOBAL_SHADER_CACHEFILE, FAM_Read );

		if ( !File.IsValid() )
			return;

		BinarySerializer Ser( &File, true );
		Ser.SerializerFileHeader( FILE_VER_SHADERCACHE );

		Serialize( Ser );
	}

	void GlobalShaderFactory::SaveCache( )
	{
		FileStream File( GLOBAL_SHADER_CACHEFILE, FAM_Write );

		if ( !File.IsValid() )
			return;

		BinarySerializer Ser( &File, false );
		Ser.SerializerFileHeader( FILE_VER_SHADERCACHE );

		Serialize( Ser );
	}

	void GlobalShaderFactory::Clear(  )
	{
		// D3D底层还握有RHIShader的引用
		for ( GlobalShaderMap::iterator it(mGlobalShaderMap);it;++it)
		{
			RenderShader* Binder = it.get_value();
			DeleteShader_RenderThread( Binder );
		}

		mGlobalShaderMap.clear();

	}

	dword GlobalShaderFactory::Compile( )
	{
		RTTIGroup* Group = RTTIObject::GetGroup( L"GlobalShader" );

		for ( RTTIClass* ClassInfo = Group->GetFirstClass();
			ClassInfo != null;
			ClassInfo = Group->GetNextClass() )
		{
			// 已经缓冲过的类
			if ( Find( ClassInfo ) != null )
				continue;

			RenderShader* Binder = RTTI_CAST(RenderShader, ClassInfo->CreateInstance( ) );

			mGlobalShaderMap.set( ClassInfo, Binder );
		}


		ShaderCompilerEnvironment ShaderEnvironment;

		dword CompiledFile = 0;

		for ( GlobalShaderMap::iterator it(mGlobalShaderMap);
			it;
			++it )
		{
			RenderShader* Binder = it.get_value();

			if ( CompileGlobalShader( Binder, ShaderEnvironment ) )
			{
				CompiledFile++;
			}

			if ( Binder->ShowDisassembleCode() )
			{
				ShaderMetaInfo* MetaInfo = Binder->GetShaderMetaInfo();

				if ( MetaInfo != null )
				{
					mLogger->Info( L"=====%s ASM Code Begin=====", MetaInfo->mSourceFile);

					mLogger->Info( StringConverter().AnsiToUnicode( RHI->DisassembleCode( Binder->mShaderRHI->GetCodePointer() ).c_str()) );

					mLogger->Info( L"===== ASM Code End=======");

				}
			}
		}

		return CompiledFile;
	}


	bool GlobalShaderFactory::CompileGlobalShader( RenderShader* Binder, ShaderCompilerEnvironment& CompilerEnvironment )
	{
		ShaderMetaInfo* MetaInfo = Binder->GetShaderMetaInfo();

		if ( MetaInfo == null )
			return false;

		WString RealPath = ResourceLocation::Resovle( MetaInfo->mSourceFile );

		// 比较文件是否有修改
		FileTime ThisFileTime;
		if ( File::GetFileModifyTime( RealPath.c_str() , ThisFileTime ) )
		{
			if ( ThisFileTime == Binder->mModifyTime )
				return false;
		}
		else
		{
			return false;
		}


		// 保存当前修改时间
		Binder->mModifyTime = ThisFileTime;

		mLogger->Info( L"Recompile Shader file %s ...", RealPath.c_str() );


		// 各种Shader信息都共用这个结构
		ShaderCompilerOutput ShaderOutput;

		Binder->PreprocessMacro( CompilerEnvironment );

		if ( RHI->CompileShader( RealPath.c_str(), MetaInfo->mEntryName, MetaInfo->mType, CompilerEnvironment, ShaderOutput ) )
		{
			Binder->mShaderRHI = ShaderOutput.mShaderRHI;

			Binder->BindShaderParameter( MetaInfo->mType, ShaderOutput.mParameterMap );
		}
		else
		{
			mLogger->Error( ShaderOutput.mErrors.c_str() );
			return false;
		}

		CompilerEnvironment.mMacroDefineMap.clear();			

		return true;
	}


}