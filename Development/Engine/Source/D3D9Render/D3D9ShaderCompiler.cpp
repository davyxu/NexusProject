#include "D3D9PCH.h"
#include "Engine/RenderStateWrapper.h"

namespace NekoEngine
{
	bool LoadFileToString( const wchar* FileName, AString& Content, bool TerminateString )
	{
		FileStream TFile;
		if ( !TFile.Open(FileName, FAM_Read ) )
		{
			return false;
		}

		dword FileSize = TFile.GetSize();

		Content.resize( FileSize + (TerminateString ? 1: 0) );
		TFile.ReadBuffer( (void*)Content.data(), FileSize * sizeof( char ) );

		if ( TerminateString )
			Content[FileSize] = 0;

		return true;
	}

	class D3D9IncludeEnvironment : public ID3DXInclude
	{
	public:

		STDMETHOD(Open)(D3DXINCLUDE_TYPE Type,LPCSTR Name,LPCVOID ParentData,LPCVOID* Data,UINT* Bytes)
		{
			AString* Result = mEnvironment.mIncludeFileMap.find( Name );
			if ( Result == null )
			{
				Result = &mEnvironment.mIncludeFileMap.set( Name, "" );

				wchar ConvBuffer[MAX_PATH];
				StringConverter::AnsiToUnicode( ConvBuffer, MAX_PATH, Name );

				wchar PathBuffer[MAX_PATH];
				StringHelper::FormatBuffer( PathBuffer, MAX_PATH, L"..\\Content\\EngineShader\\%s", ConvBuffer );
				if ( !LoadFileToString( PathBuffer, *Result, false ) )
					return D3DERR_NOTFOUND;
			}

			*Bytes = Result->length();
			*Data = Result->c_str();
			
			return D3D_OK;
		}

		STDMETHOD(Close)(LPCVOID Data)
		{
			return D3D_OK;
		}

		D3D9IncludeEnvironment( ShaderCompilerEnvironment& InEnvironment )
			: mEnvironment( InEnvironment )
		{
		
		}

	private:
		ShaderCompilerEnvironment& mEnvironment;
	};






	typedef dynamic_hash<D3D9ShaderSlot, RHIShaderRef > ShaderCodeMap;
	ShaderCodeMap GShaderCodeMap;
	
	bool D3D9RHI::CompileShader(	const wchar* SourceFile,
							const wchar* EntryName,
							eShaderType ShaderType,
							ShaderCompilerEnvironment& Environment,
							ShaderCompilerOutput& CompileOutput)
	{
		HRESULT HR;
		DWORD ShaderFlag = 0;
		ID3DXBuffer* CompileError = null;
		ID3DXBuffer* CompiledCode = null;
		ID3DXConstantTable* ConstantBuffer;
		const char* Profile = null;

		StringConverter Conv;


		if ( Profile == null )
		{
			// TODO 未来版本直接在启动时取出, 避免在游戏线程中取设备数据
			if ( ShaderType == ST_VertexShader)
			{
				//Profile = D3DXGetVertexShaderProfile( GDirect3DDevice );
				Profile = "vs_2_0";
			}
			else if ( ShaderType == ST_PixelShader )
			{
				//Profile = D3DXGetPixelShaderProfile( GDirect3DDevice );
				Profile = "ps_2_0";
			}
		}

		D3D9IncludeEnvironment IncludeEnvironment( Environment );

		AString ShaderContent;
		if ( !LoadFileToString( SourceFile, ShaderContent, true ) )
		{
			CompileOutput.mErrors += StringHelper::FormatString(L"Can not open %s", SourceFile );
			return false;
		}

		typedef std::vector<D3DXMACRO> MacroArray;
		MacroArray tMacroArray;

		for ( ShaderCompilerEnvironment::StringPair::iterator it( Environment.mMacroDefineMap );
			it;
			++it)
		{

			D3DXMACRO Macro = { it.get_key().c_str(), it.get_value().c_str() };

			tMacroArray.push_back( Macro );
		}

		{
			D3DXMACRO Macro = { null,null };
			tMacroArray.push_back( Macro );
		}
		
		char ConvBuffer[MAX_PATH];
		StringConverter::UnicodeToAnsi( ConvBuffer, MAX_PATH, EntryName );

		// Assemble the vertex shader from the file
		HR = D3DXCompileShader(ShaderContent.c_str(),
			ShaderContent.size(),
			&tMacroArray[0], 
			&IncludeEnvironment, 
			ConvBuffer,
			Profile,
			ShaderFlag,
			&CompiledCode,
			&CompileError,
			&ConstantBuffer);

		if( FAILED(HR) )
		{
			// 多种Shader编译错误合并
			CompileOutput.mErrors = Conv.AnsiToUnicode((const char*)CompileError->GetBufferPointer());
			CompileOutput.mErrors += L"\r\n";
		
			return false;
		}



		CompileOutput.mShaderRHI = CreateShader( ShaderType, CompiledCode->GetBufferPointer(), CompiledCode->GetBufferSize( ) );

		D3DXCONSTANTTABLE_DESC	Desc;
		VD3DHR(ConstantBuffer->GetDesc(&Desc));
		// Map each constant in the table.
		for(UINT ConstantIndex = 0;ConstantIndex < Desc.Constants;ConstantIndex++)
		{
			// Read the constant description.
			D3DXCONSTANT_DESC ConstantDesc;
			UINT NumConstants = 1;
			VD3DHR(ConstantBuffer->GetConstantDesc(ConstantBuffer->GetConstant(null,ConstantIndex),&ConstantDesc,&NumConstants));

			CompileOutput.mParameterMap.AddParameterAllocation(
				ConstantDesc.Name,
				ConstantDesc.RegisterIndex,
				ConstantDesc.RegisterCount
				);
		}

		SafeRelease( CompiledCode );
		SafeRelease( ConstantBuffer );
		SafeRelease( CompileError );

		return true;
	}

	AString D3D9RHI::DisassembleCode( void* Code )
	{
		ID3DXBuffer* D3DAsmCode = null;
		D3DXDisassembleShader( (DWORD*)Code,FALSE, null, &D3DAsmCode );
		AString FinalCode = (char*)D3DAsmCode->GetBufferPointer( );
		D3DAsmCode->Release();

		return FinalCode;
	}

	RHIShaderRef D3D9RHI::CreateShader( eShaderType ShaderType, void* Code, dword CodeSize )
	{

		D3D9ShaderFinder Finder( ShaderType,(_byte*)Code, CodeSize );

		RHIShaderRef* Result = GShaderCodeMap.find( Finder );

		RHIShader* Shader = null;

		if ( Result == null )
		{
			

			if ( ShaderType == ST_VertexShader)
			{
				Shader = new D3D9VertexShader( ShaderType, (_byte*)Code, CodeSize );
			}
			else if ( ShaderType == ST_PixelShader )
			{
				Shader = new D3D9PixelShader( ShaderType, (_byte*)Code, CodeSize );		
			}
			else
			{
				Assert( false && "Invalid Shader type" )
				return null;
			}

			GShaderCodeMap.set( D3D9ShaderSlot(Shader), Shader );
		}
		else
		{
			Shader = *Result;
		}

		return Shader;
	}

	void RHIReleaseShader( )
	{
		for (ShaderCodeMap::iterator it( GShaderCodeMap );it;++it)
		{
			RHIShaderRef& Shader = it.get_value();
			Shader.Release( );
		}

		GShaderCodeMap.clear();
	}
}