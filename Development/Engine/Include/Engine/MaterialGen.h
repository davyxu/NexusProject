#pragma once

namespace NekoEngine
{
	/** 
		²ÄÖÊÉú³É
	*/
	class MaterialHelper
	{
	public:
		static void GenerateDiffuseLightingMaterial( const wchar* Location , bool Skinned );

		static void GenerateSpecularLigtingMaterial( const wchar* Location, bool Skinned , const wchar* BaseMapTextureName );

		static void GenerateBumpMapMaterial( const wchar* Location, bool Skinned  );
		static void GenerateConstantColorMaterial( const wchar* Location, bool Skinned );
		static void GeneratePureEmissiveMaterial( const wchar* Location, bool Skinned );
	};
}
