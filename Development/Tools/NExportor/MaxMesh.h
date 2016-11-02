#pragma once

//----------------------------------------------------------------------------
// MaxSkin
//----------------------------------------------------------------------------
struct VertexExportOption
{	
	bool mSkin;
	bool mTangent;
	VertexExportOption( )
		: mSkin( false )
		, mTangent( true )
	{

	}
};

class MaxMesh : public MaxObject
{
public:

	MaxMesh( MaxSkeleton* skeleton, INode* node, int mtlid );

	virtual void Prepare( );

	void Serialize( NekoEngine::MeshVertexFactory* Factory, VertexExportOption& Option );

	char*	GetName( );

private:
	enum { PLUGIN_MAX_TEXCOORD_NUMBER  = 4 };
	enum { PLUGIN_MAX_INFLUENCE_NUMBER = 10 };

	struct Influence
	{
		int		mBoneID;
		float	mWeight;
	};

	struct Vertex
	{
		int			mVertexID;
		int			mFaceID;
		Point3		mPosition;
		Point3		mNormal;
		Point3		mTangent;
		Point3		mBiNormal;
		Point3		mTexCoordList[ PLUGIN_MAX_TEXCOORD_NUMBER ];
		int			mTexCoordNumber;
		Influence	mInfluenceList[ PLUGIN_MAX_INFLUENCE_NUMBER ];
		int			mInfluenceNumber;
	};

	struct Face
	{
		unsigned short	mVertexID[3];
	};

	typedef vector< Vertex >	VertexArray;
	typedef vector< Face >		FaceArray;
	

	INode*			mNode;
	int				mMtlID;
	Mesh*			mMesh;
	MaxSkeleton*	mSkeleton;
	VertexArray		mVertexArray;
	FaceArray		mFaceArray;
	


	int				mTextureNumber;
	DWORD			mMaterialID;
	

	bool		IsSkin( );
	int			AddVertex( Vertex& vertex );
	int			AddTexCoord( Vertex& vertex, const Point3& texcoord );
	int			AddInfluence( Vertex& vertex, const Influence& influence );
	Modifier*	FindModifier( );
	Point3		GetNormal( Mesh* mesh, int vertexid, int faceid, const Matrix3& transform );

	

	virtual char*	GetGroupName( );
	void ClearInfluence( Vertex& vertex );
	void BuildTangentSpaceVector( );
};

typedef vector< MaxMesh* > SkinArray;