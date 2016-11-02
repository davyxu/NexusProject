#include "ExportPCH.h"
#include "PluginInclude.h"
//----------------------------------------------------------------------------
// MaxSkin
//----------------------------------------------------------------------------

MaxMesh::MaxMesh( MaxSkeleton* skeleton, INode* node, int mtlid )
	: mNode( node )
	, mMesh( null )
	, mMtlID( mtlid )
	, mSkeleton( skeleton )
	, mTextureNumber( 0 )
	, mMaterialID( -1 )
{
	TriObject* triangle = (TriObject*) ( node->EvalWorldState( 0 ).obj )->ConvertToType( 0, triObjectClassID );

	mMesh = &( triangle->mesh );
	mMesh->checkNormals( 1 );
}

bool MaxMesh::IsSkin( )
{
	// Skin Modifier
	Modifier* modifier = FindModifier( );
	ISkin* skin = modifier != null ? (ISkin*) modifier->GetInterface( I_SKIN ) : null;
	ISkinContextData* skincontext = skin != null ? (ISkinContextData*) skin->GetContextInterface( mNode ) : null;

	return skincontext != null;
}

int MaxMesh::AddVertex( Vertex& vertex )
{
	for ( int i = 0; i < mVertexArray.size( ); i ++ )
	{
		Vertex& oldvertex = mVertexArray[i];

		if ( oldvertex.mPosition.Equals( vertex.mPosition ) == 0 )
			continue;

		if ( oldvertex.mNormal.Equals( vertex.mNormal ) == 0 )
			continue;

		if ( oldvertex.mTexCoordNumber != vertex.mTexCoordNumber )
			continue;

		if ( oldvertex.mInfluenceNumber != vertex.mInfluenceNumber )
			continue;

		int j = 0;

		for ( j = 0; j < oldvertex.mTexCoordNumber; j ++ )
		{
			if ( oldvertex.mTexCoordList[j].Equals( vertex.mTexCoordList[j] ) == 0 )
				break;
		}

		if ( j < oldvertex.mTexCoordNumber )
			continue;

		for ( j = 0; j < oldvertex.mInfluenceNumber; j ++ )
		{
			if ( oldvertex.mInfluenceList[j].mBoneID != vertex.mInfluenceList[j].mBoneID )
				break;
		}

		if ( j < oldvertex.mInfluenceNumber )
			continue;

		return i;
	}

	mVertexArray.push_back( vertex );

	return (int) mVertexArray.size( ) - 1;
}

int MaxMesh::AddTexCoord( Vertex& vertex, const Point3& texcoord )
{
	if ( vertex.mTexCoordNumber == PLUGIN_MAX_TEXCOORD_NUMBER )
		return vertex.mTexCoordNumber;

	vertex.mTexCoordList[ vertex.mTexCoordNumber ++ ] = texcoord;

	return vertex.mTexCoordNumber - 1;
}

void MaxMesh::ClearInfluence( Vertex& vertex )
{
	for ( int i= 0;i< PLUGIN_MAX_INFLUENCE_NUMBER;i++)
	{
		vertex.mInfluenceList[i].mBoneID = -1;
		vertex.mInfluenceList[i].mWeight = 0.0f;
	}
}

int MaxMesh::AddInfluence( Vertex& vertex, const Influence& influence )
{
	if ( vertex.mInfluenceNumber >= PLUGIN_MAX_INFLUENCE_NUMBER )
		return vertex.mInfluenceNumber;

	vertex.mInfluenceList[ vertex.mInfluenceNumber ++ ] = influence;

	return vertex.mInfluenceNumber - 1;
}

Modifier* MaxMesh::FindModifier( )
{
	Object* object = mNode->GetObjectRef( );

	while ( object != null && object->SuperClassID( ) == GEN_DERIVOB_CLASS_ID )
	{
		IDerivedObject* derivedobject = (IDerivedObject*) object;

		for ( int i = 0; i < derivedobject->NumModifiers( ); i ++ )
		{
			Modifier* modifier = derivedobject->GetModifier( i );

			if ( modifier->ClassID( ) == SKIN_CLASSID )
				return modifier;
		}

		object = derivedobject->GetObjRef( );
	}

	return null;
}

Point3 MaxMesh::GetNormal( Mesh* mesh, int vertexid, int faceid, const Matrix3& transform )
{
	Point3 normal;

	float tempmatrix[4][3];
	for ( int r = 0; r < 3; r ++ )
	{
		for ( int c = 0; c < 3; c ++ )
			tempmatrix[c][r] = transform.GetAddr( )[r][c];
	}

	for ( int c = 0; c < 3; c ++ )
		tempmatrix[3][c] = 0;

	Matrix3 transpose( tempmatrix );
	transpose.NoTrans( );

	RVertex* rvertex = mesh->getRVertPtr( vertexid );

	if ( rvertex != null && rvertex->rFlags & SPECIFIED_NORMAL )
	{
		normal = rvertex->rn.getNormal( );
	}
	else if ( rvertex != null && ( rvertex->rFlags & NORCT_MASK ) > 0 && mesh->faces[ faceid ].smGroup != 0 )
	{
		if ( ( rvertex->rFlags & NORCT_MASK ) == 1 )
		{
			normal = rvertex->rn.getNormal( );
		}
		else
		{
			for ( int l = 0; l < ( rvertex->rFlags & NORCT_MASK ); l ++ )
			{
				if ( rvertex->ern[l].getSmGroup( ) & mesh->faces[ faceid ].smGroup )
				{
					normal = rvertex->ern[l].getNormal( );

					break;
				}
			}
		}
	}
	else
	{
		normal = mesh->getFaceNormal( faceid );
	}

	normal = normal * Inverse( transpose );

	return normal.Normalize( );
}

char* MaxMesh::GetName( )
{
	return mNode->GetName( );
}

char* MaxMesh::GetGroupName( )
{
	if ( mNode->IsGroupMember( ) )
		return mNode->GetParentNode( )->GetName( );

	return null;
}

::ISkin* SearchSkinHelper( INode* node )
{
	::Object* object = node->GetObjectRef( );

	while ( object != null && object->SuperClassID( ) == GEN_DERIVOB_CLASS_ID )
	{
		::IDerivedObject* derivedobject = (::IDerivedObject*) object;

		for ( int i = 0; i < derivedobject->NumModifiers( ); i ++ )
		{
			::Modifier* modifier = derivedobject->GetModifier( i );

			if ( modifier == null )
				continue;

			// Found skin.
			if ( modifier->ClassID( ) == SKIN_CLASSID )
				return (::ISkin*) modifier->GetInterface( I_SKIN );
		}

		object = derivedobject->GetObjRef( );
	}

	return null;
}



void MaxMesh::Prepare()
{
	const char* name = GetName();

	if ( mMesh->numFaces == 0 )
		return;

	Matrix3 transform = MaxNode( mNode ).GetTransform( );

	// Material
	MaxMaterial material( mNode->GetMtl( ), mMtlID );

	mTextureNumber = material.GetTextureNumber( mMesh );

	// Skin Modifier
	ISkin* skin = SearchSkinHelper( mNode );
	ISkinContextData* skincontext = skin != null ? (ISkinContextData*) skin->GetContextInterface( mNode ) : null;

	for ( int i = 0; i < mMesh->numFaces; i ++ )
	{
		if ( mMtlID != -1 && mMtlID != mMesh->faces[i].getMatID( ) )
			continue;

		Face face;

		for ( int j = 0; j < 3; j ++ )
		{
			Vertex vertex;

			vertex.mVertexID		= mMesh->faces[i].v[j];
			vertex.mFaceID			= i;
			vertex.mTexCoordNumber	= 0;
			vertex.mInfluenceNumber	= 0;
			vertex.mTangent = Point3::Origin;
			vertex.mBiNormal = Point3::Origin;

			// Vertex Position
			vertex.mPosition = mMesh->verts[ vertex.mVertexID ]* transform;

			// 关闭原来的X调整, 在MAX中茶壶嘴位置是X位置,也是物体正方形 [10/20/2011 Davyxu]
			//vertex.mPosition.x = - vertex.mPosition.x;

			ClearInfluence( vertex );

			// Vertex Normal
			vertex.mNormal = GetNormal( mMesh, vertex.mVertexID, vertex.mFaceID,  transform );
			//vertex.mNormal.x = - vertex.mNormal.x;

			

			// Vertex Texcoord * N
			for ( int t = 0; t < mTextureNumber; t ++ )
			{
				Texmap* texmap = material.GetTextureMap( t, mMesh );

				if ( texmap == null )
				{
					AddTexCoord( vertex, Point3( 0.0f, 0.0f, 0.0f ) );
					continue;
				}

				StdUVGen* stduvgen = (StdUVGen*) texmap->GetTheUVGen( );

				if ( stduvgen == null )
				{
					AddTexCoord( vertex, Point3( 0.0f, 0.0f, 0.0f ) );
					continue;
				}

				Matrix3 uvtransform;
				stduvgen->GetUVTransform( uvtransform );

				TVFace* tvface = mMesh->mapFaces( texmap->GetMapChannel( ) );
				UVVert* uvvert = mMesh->mapVerts( texmap->GetMapChannel( ) );

				Point3 texcoord;
				if ( tvface != null && uvvert != null )
					texcoord = uvvert[ tvface[i].t[j] ] * uvtransform;

				AddTexCoord( vertex, Point3( texcoord.x, 1.0f - texcoord.y, texcoord.z ) );
			}

			// Skin Influence * N
			if ( skincontext != null )
			{
				for ( int s = 0; s < skincontext->GetNumAssignedBones( vertex.mVertexID ); s ++ )
				{
					int boneid = skincontext->GetAssignedBone( vertex.mVertexID, s );

					if ( boneid < 0 )
						continue;

					Influence influence;
					influence.mBoneID = mSkeleton->GetBoneID( skin->GetBone( boneid )->GetName( ) );
					influence.mWeight = skincontext->GetBoneWeight( vertex.mVertexID, s );

					AddInfluence( vertex, influence );
				}
			}

			// Vertex Index
			face.mVertexID[j] = AddVertex( vertex );
		}

		if ( transform.Parity( ) == 0 )
		{
			short t = face.mVertexID[1];
			face.mVertexID[1] = face.mVertexID[2];
			face.mVertexID[2] = t;
		}

		mFaceArray.push_back( face ); 
	}

	BuildTangentSpaceVector( );


	mMaterialID = material.CollectTexture( mMesh, GMaxCollector->mMaxMaterial );

}

void MaxMesh::BuildTangentSpaceVector( )
{
	for ( int f = 0; f < mFaceArray.size( ); f ++ )
	{
		Face& face = mFaceArray[f];

		Point3& P1 = mVertexArray[ face.mVertexID[0] ].mPosition;
		Point3& P2 = mVertexArray[ face.mVertexID[1] ].mPosition;
		Point3& P3 = mVertexArray[ face.mVertexID[2] ].mPosition;

		Point3& UV1 = mVertexArray[ face.mVertexID[0] ].mTexCoordList[0];
		Point3& UV2 = mVertexArray[ face.mVertexID[1] ].mTexCoordList[0];
		Point3& UV3 = mVertexArray[ face.mVertexID[2] ].mTexCoordList[0];

		Point3  e1 = P2 - P1;
		Point3  e2 = P3 - P1;
		Point2 u1 ( UV2.x - UV1.x , UV2.y - UV1.y );
		Point2 u2 ( UV3.x - UV1.x , UV3.y - UV1.y );

		float det =  ( u1.x * u2.y - u2.x * u1.y);

		Point3 Tangent;

		if(det == 0.0f)
		{
			Tangent = e1;
		}
		else
		{
			Tangent = u2.y * e1 - u1.y * e2;
		}

		//从Normal 和 Tangent里重新计算出Tangent,因为面的Tangent和顶点的Normal可能不垂直
		Point3 final_tangent;
		for(int i = 0 ;i < 3 ; ++i)
		{
			Point3& Normal = mVertexArray[ face.mVertexID[i] ].mNormal;
			Point3& VertexTangent = mVertexArray[ face.mVertexID[i] ].mTangent;
			Point3& VertexBinormal = mVertexArray[ face.mVertexID[i] ].mBiNormal;

			VertexBinormal  = CrossProd(Normal,Tangent);
			final_tangent = CrossProd(VertexBinormal,Normal);
			final_tangent.Normalize();

			
			VertexTangent.x += final_tangent.x;
			VertexTangent.y += final_tangent.y;
			VertexTangent.z += final_tangent.z;
		}

	}
}


void MaxMesh::Serialize( NekoEngine::MeshVertexFactory* Factory, VertexExportOption& Option )
{ 

	DWORD PreBuffer = sizeof(Point3) * 2 + sizeof(Point2);
	if ( Option.mSkin )
	{
		PreBuffer += sizeof(float) *4 *2;
	}
	
	NekoEngine::HeapStream* VertexBuffer = new NekoEngine::HeapStream;
	
	VertexBuffer->AllocBuffer( PreBuffer * mVertexArray.size() );


	// 按照顶点缓冲的格式来填充
	for ( int v = 0; v < mVertexArray.size( ); v ++ )
	{
		Vertex& vertex = mVertexArray[v];

		
		// Vertex Position
		VertexBuffer->WriteBuffer(&vertex.mPosition,sizeof(Point3));

		// Vertex Normal
		VertexBuffer->WriteBuffer(&vertex.mNormal,sizeof(Point3));

		// 单层纹理

		// 没有纹理时自动填充
		if ( mTextureNumber == 0 )
		{
			VertexBuffer->WriteBuffer(Point2( 0.0f, 0.0f ),sizeof(Point2));
		}
		// 使用第一层
		else
		{
			VertexBuffer->WriteBuffer(&Point2(vertex.mTexCoordList[0].x,vertex.mTexCoordList[0].y),sizeof(Point2));
		}

		// 骨骼权重
		if ( Option.mSkin )
		{
			const int MAX_VERTEX_INFLUENCENUMBER = 4;
			
			// 写入权重
			for ( int s = 0; s < MAX_VERTEX_INFLUENCENUMBER; s ++ )
			{
				if (s < vertex.mInfluenceNumber)
					VertexBuffer->WriteTypes<float>( vertex.mInfluenceList[s].mWeight );
				else
					VertexBuffer->WriteTypes<float>( 0.0f );
			}

			// 写入骨头
			for ( int s = 0; s < MAX_VERTEX_INFLUENCENUMBER; s ++ )
			{
				if (s < vertex.mInfluenceNumber)
					VertexBuffer->WriteTypes<float>( (float)vertex.mInfluenceList[s].mBoneID );
				else
					VertexBuffer->WriteTypes<float>( -1.0f );// 无效骨头
			}
		}

		if ( Option.mTangent )
		{
			VertexBuffer->WriteBuffer( &vertex.mTangent, sizeof( Point3 ) );
			VertexBuffer->WriteBuffer( &vertex.mBiNormal, sizeof( Point3 ) );
		}
	}


	NekoEngine::HeapStream* IndexBuffer = new NekoEngine::HeapStream;

	DWORD IndexBufferSize = sizeof(Face) * mFaceArray.size();
	IndexBuffer->AllocBuffer( IndexBufferSize );

	IndexBuffer->WriteBuffer(&mFaceArray[0],IndexBufferSize);


	Factory->InitData( NekoEngine::StringConverter().AnsiToUnicode( GetName() ), 
					   mVertexArray.size(),
					   VertexBuffer,
					   mFaceArray.size() * 3, 
					   IndexBuffer,
					   mMaterialID );

}

