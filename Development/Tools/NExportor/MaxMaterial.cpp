#include "ExportPCH.h"
#include "PluginInclude.h"
//----------------------------------------------------------------------------
// MaxMaterial
//----------------------------------------------------------------------------

MaxMaterial::MaxMaterial( Mtl* material, int mtlid ) : mMaterial( null )
{
	if ( material != null )
	{
		if ( material->IsMultiMtl( ) )
		{
			if ( mtlid >= 0 && mtlid < material->NumSubMtls( ) )
				mMaterial = (StdMat*) material->GetSubMtl( mtlid );
		}
		else
		{
			mMaterial = (StdMat*) material;
		}
	}
}

MaxMaterial::MaxMaterial( )
{

}

int MaxMaterial::GetTextureNumber( Mesh* mesh )
{
	if ( mMaterial == null )
		return 0;

	int texnumber = 0, mapchannel = 0;

	for ( int i = 0; i < mMaterial->NumSubTexmaps( ); i ++ )
	{
		Texmap* texmap = mMaterial->GetSubTexmap( i );

		if ( texmap == null || mMaterial->MapEnabled( i ) == 0 || texmap->ClassID( ) != Class_ID( BMTEX_CLASS_ID, 0 ) )
			continue;

		texnumber	++;
		mapchannel	++;
	}

	return texnumber;
}

Texmap* MaxMaterial::GetTextureMap( int index, Mesh* mesh )
{
	if ( mMaterial == null )
		return null;

	int texnumber = 0, mapchannel = 0;

	for ( int i = 0; i < mMaterial->NumSubTexmaps( ); i ++ )
	{
		Texmap* texmap = mMaterial->GetSubTexmap( i );

		if ( texmap == null || mMaterial->MapEnabled( i ) == 0 || texmap->ClassID( ) != Class_ID( BMTEX_CLASS_ID, 0 ) )
			continue;

		if ( mesh->mapFaces( texmap->GetMapChannel( ) ) == null )
			continue;

		if ( mesh->mapVerts( texmap->GetMapChannel( ) ) == null )
			continue;

		if ( texmap->GetMapChannel( ) > mapchannel )
		{
			texnumber	++;
			mapchannel	++;
		}

		if ( texnumber == index + 1 )
			return texmap;
	}

	return null;
}

char* MaxMaterial::GetName( )
{
	if ( mMaterial == null )
		return "";

	return mMaterial->GetName( ).data( );
}

DWORD MaxMaterial::AddTextureWithDefaultMaterial( const wchar_t* name )
{
	//NekoEngine::MaterialInstance* Instance = &mMaterialInstance;
	//DWORD MaterialIndex = mMaterialInstance.AddTextureName( NekoEngine::Path::RemoveExtention( name  ).c_str() );

	//// 默认材质
	//NekoEngine::MaterialInstanceData MID;
	//MID.mMaterialShaderIndex = 0; // 使用默认第一个
	//MID.mTextureUseList.push_back( 0 );

	//mMaterialInstance.AddInstance( MID );

	return 0;
}

DWORD MaxMaterial::CollectTexture( Mesh* MMesh, MaxMaterial* GlobalMaxMaterial )
{
	int TextureCount = GetTextureNumber( MMesh );

	NekoEngine::StringConverter conv;
	std::wstring filename;

	Texmap*	texmap = GetTextureMap( 0, MMesh );

	if ( texmap == null )
		return -1;

	std::wstring pathname = conv.AnsiToUnicode( ( (BitmapTex*) texmap )->GetMapName( ) );

	wstring::size_type pos = pathname.find_last_of( L"\\/:" );
	if ( pos == wstring::npos )
		filename = pathname;
	else
		filename = pathname.substr( pos + 1 );

	return GlobalMaxMaterial->AddTextureWithDefaultMaterial( filename.c_str() );
}

bool MaxMaterial::Save( const wchar_t* Filename )
{
	using namespace NekoEngine;


	NekoEngine::FileStream Stream(Filename,FAM_Write );
	BinarySerializer Ser( &Stream, false );
	Ser << mMaterialInstance;

	return true;
}

