#pragma once

class MaxModel
{
public:
	MaxModel(  );
	virtual ~MaxModel( );

	DWORD GetMeshCount( );

	bool Save( const wchar_t* FileName, VertexExportOption& veo );

	void AddMesh( INode* Node, int Index );

private:
	friend class MaxInterface;

	SkinArray						mMeshArray;

	MaterialArray					mMaterialArray;	
};