#pragma once

namespace NekoEngine
{
	class D3D9VertexFormat : public RHIVertexFormat
	{
	public:
		D3D9VertexFormat( );

		D3D9VertexFormat(const VertexFormat& vfp);
		virtual ~D3D9VertexFormat( );

		

		

		bool operator == (const D3D9VertexFormat& Other);

		D3DVERTEXELEMENT9* GetVertexElements();

		const D3DVERTEXELEMENT9* GetVertexElements() const;

		virtual void Commit();

		friend DWORD GetTypeHash(const D3D9VertexFormat& Key)
		{
			return Key.Hash;
		}


	private:
		
		/** array of D3D vertex elements */
		D3DVERTEXELEMENT9 VertexElements[ VertexFormat::MaxVertexElementCount ];
		DWORD mElementCount;
		/** hash value based on vertex elements */
		DWORD Hash;

		IDirect3DVertexDeclaration9* mDeclaration;
	};

}