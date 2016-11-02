#include "EnginePCH.h"
#include "NekoEngine_Material.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// MaterialDescriptor
	//////////////////////////////////////////////////////////////////////////
	IMPLEMENT_RTTI_CLASS( MaterialDescriptor  )

	MaterialDescriptor::MaterialDescriptor( )
	: mAccIDGen( 0 )
	, mIterator( null )
	{
		mCompiler = new MaterialCompiler;
		
	}


	MaterialDescriptor::~MaterialDescriptor( )
	{
		delete mIterator;

		Clear();
		delete mCompiler;
	}


	void MaterialDescriptor::Serialize( BinarySerializer& Ser )
	{
		Ser  << mNodeMap;

		// 读取后，取所有node中最大的一个id最开始生成的id，避免生成id重复
		if ( Ser.IsLoading() )
		{
			dword MaxNodeID = 0;
			for ( NodeMap::iterator it ( mNodeMap );it;++it )
			{
				MaxNodeID = Math::Max( it.get_key(), MaxNodeID );
				MaterialNode* Node = it.get_value();
				Node->Init( mCompiler, this );
			}

			mAccIDGen = MaxNodeID;			
		}
	}


	MaterialNode* MaterialDescriptor::AddNode( RTTIClass* ClassInfo )
	{
		MaterialNode* Node = RTTI_CAST(MaterialNode, ClassInfo->CreateInstance() );
		if ( Node == null )
			return null;

		Node->Init( mCompiler, this );
		Node->mID = ++mAccIDGen;

		mNodeMap.set( mAccIDGen , Node );

		return Node;
	}

	MaterialNode* MaterialDescriptor::GetNode( dword ID )
	{
		MaterialNode** Result = mNodeMap.find( ID );
		if ( Result == null )
			return null;

		return *Result;
	}

	void MaterialDescriptor::RemoveNode( dword ID )
	{
		MaterialNode* Node = GetNode( ID );
		if ( Node == null )
			return;
		delete Node;

		mNodeMap.remove( ID );
	}

	void MaterialDescriptor::GenerateShaderFile( const wchar* FilenName )
	{
		MaterialNode* TerminateNode = null ;
		for ( NodeMap::iterator it ( mNodeMap );it;++it )
		{
			MaterialNode* Node = it.get_value();
			if ( Node->IsTerminateNode() )
			{
				if ( TerminateNode == null )
					TerminateNode = Node;
				else
				{
					Assert( false )
				}
			}
		}

		if ( TerminateNode == null )
			return;

		TerminateNode->Compile( mCompiler );
		TerminateNode->CombineCode( mCompiler );

		mCompiler->SaveCode( ResourceLocation::Resovle( FilenName ).c_str() );
	}

	void MaterialDescriptor::Clear( )
	{
		for ( NodeMap::iterator it ( mNodeMap );it;++it )
		{
			MaterialNode* Node = it.get_value();
			delete Node;
		}

		mNodeMap.clear();
	}

	MaterialNode* MaterialDescriptor::GetFirstNode()
	{
		delete mIterator;
		mIterator = new NodeMap::iterator( mNodeMap );

		return GetNextNode();
	}


	MaterialNode* MaterialDescriptor::GetNextNode()
	{
		if ( *mIterator == false )
			return null;

		MaterialNode* Node = mIterator->get_value();

		++(*mIterator);

		return Node;
	}

	MaterialNode* MaterialDescriptor::GetTerminateNode()
	{
		for ( NodeMap::iterator it ( mNodeMap );it;++it )
		{
			MaterialNode* Node = it.get_value();
			if ( Node->IsTerminateNode() )
				return Node;
		}

		return null;
	}


}