#pragma once

//----------------------------------------------------------------------------
// MaxMaterial
//----------------------------------------------------------------------------

class MaxMaterial 
{
public:
	StdMat*		mMaterial;

	MaxMaterial( );

	MaxMaterial( Mtl* material, int mtlid );
	
	bool Save( const wchar_t* Filename );

	DWORD CollectTexture( Mesh* MMesh, MaxMaterial* GlobalMaxMaterial );


	int		GetTextureNumber( Mesh* mesh );
	Texmap*	GetTextureMap( int index, Mesh* mesh );

	virtual char*	GetName( );

	DWORD AddTextureWithDefaultMaterial( const wchar_t* name );
	

	NekoEngine::MaterialInstance	mMaterialInstance;
};

typedef vector< MaxMaterial* > MaterialArray;