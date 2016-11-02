#include "EnginePCH.h"
#include "NekoEngine_Material.h"

namespace NekoEngine
{
	//////////////////////////////////////////////////////////////////////////
	// MaterialSlot
	//////////////////////////////////////////////////////////////////////////

	MaterialSlot::MaterialSlot()
		: mNode( null )
		, mLinkedNodeID( -1 )
		, mInputMask( MVM_RGB )
		, mInputResult( -1 )
		, mMaterial( null )
		, mDefaultValue( 0 )
		, mCompileFinish( false )
	{

	}

	bool MaterialSlot::IsFree( )
	{
		return mLinkedNodeID == -1;
	}

	void MaterialSlot::Compile(MaterialCompiler* Compiler)
	{
		if ( mCompileFinish )
			return;

		MaterialNode* LinkedNode = mMaterial->GetNode( mLinkedNodeID );

		// 没有连接任何输入，使用默认值
		if ( LinkedNode == null )
		{
			mInputResult = Compiler->AddCodeChunk( mValueType, Compiler->GetFreeVarIndex(), "%s;",mDefaultValue );
		}
		// 使用节点输入
		else
		{
			LinkedNode->Compile( Compiler );

			mInputResult = LinkedNode->GetOutputCodeIndex();
		}

		mCompileFinish = true;
	}

	void MaterialSlot::CombineCode( MaterialCompiler* Compiler )
	{
		MaterialCodeBody CodeBody;
		Compiler->CollectRelatedCode( mInputResult, CodeBody );
		Compiler->CombineCodeBody( CodeBody, mCodeBody );

		mCodeResult = Compiler->GetValueMasked( GetResult() );
	}

	MaterialMaskedValue MaterialSlot::GetResult()
	{
		return MaterialMaskedValue(mInputResult, mInputMask);
	}

	void MaterialSlot::SolveInputMaskAsValueType( )
	{
		mValueType = MaterialCompiler::GetMaskMappingValueType( mInputMask );
	}

	void MaterialSlot::Bind( const wchar* Name,  MaterialNode* Node, eMaterialValueType Type, const char* DefaultValue )
	{
		mName			= Name;
		mValueType		= Type;
		mDefaultValue	= DefaultValue;

		Node->AddSlot( this );
	}

	void MaterialSlot::Connect( dword NodeID , eMaterialValueMask Mask )
	{
		mLinkedNodeID = NodeID;
		mInputMask = Mask;

		// 通知输出端自己的地址
		MaterialNode* LinkedNode = mMaterial->GetNode( mLinkedNodeID );
		if ( LinkedNode != null )
		{
			LinkedNode->AddOutputSlot( this );
		}

	}

	void MaterialSlot::Disconnect( MaterialNode* NodeDeleteSource )
	{

		MaterialNode* LinkedNode = mMaterial->GetNode( mLinkedNodeID );

		mLinkedNodeID = -1;
		mInputMask = MVM_RGB;

		// 避免在Node删除Slot时的循环嵌套,这里不用从Node删,Node那边会全部清除
		if ( NodeDeleteSource == LinkedNode )
			return;

		// 从输出端删除我自己
		if ( LinkedNode != null )
		{
			LinkedNode->RemoveOutputSlot( this );
		}
	}

	MaterialNode* MaterialSlot::GetLinkedNode()
	{
		return mMaterial->GetNode( mLinkedNodeID );
	}



	//////////////////////////////////////////////////////////////////////////
	// MaterialNode
	//////////////////////////////////////////////////////////////////////////

	IMPLEMENT_RTTI_CLASS( MaterialNode )

	MaterialNode::MaterialNode()
		: mOutputResult( -1 )
		, mCompileFinish( false )
		, mID( -1 )
		, mMaterial( null )
		, mScenePos( Vector2::cZero )
	{

	}

	MaterialNode::~MaterialNode()
	{

	}

	void MaterialNode::StaticInitProperty( )
	{
		PropertyCollection* PC = RTTI_CLASS_INFO(MaterialNode)->GetPropertyCollection( );

		PC->Add( PROPERTY_MEMBER_VAR( MaterialNode, Vector2, mScenePos ) );
	}


	void MaterialNode::Init( MaterialCompiler* Compiler, MaterialDescriptor* Mat )
	{
		mMaterial = Mat;

		for ( dword i = 0;i< mSlotArray.size();i++)
		{
			MaterialSlot* Slot = mSlotArray[i];
			Slot->mMaterial = Mat;
		}
	}

	void MaterialNode::AddSlot( MaterialSlot* Slot )
	{
		mSlotArray.push_back( Slot );
	}

	MaterialSlot* MaterialNode::GetSlot( dword Index )
	{
		return mSlotArray[Index];
	}

	NekoEngine::dword MaterialNode::GetSlotCount()
	{
		return mSlotArray.size();
	}

	void MaterialNode::Compile( MaterialCompiler* Compiler )
	{
		if ( mCompileFinish )
			return;

		for ( dword i = 0;i< mSlotArray.size();i++)
		{
			MaterialSlot* Slot = mSlotArray[i];
			Slot->Compile( Compiler );
		}

		mCompileFinish = true;
	}

	void MaterialNode::CombineCode( MaterialCompiler* Compiler)
	{
		for ( dword i = 0;i< mSlotArray.size();i++)
		{
			MaterialSlot* Slot = mSlotArray[i];
			Slot->CombineCode( Compiler );
		}
	}

	void MaterialNode::Serialize( BinarySerializer& Ser )
	{
		Ser << mID;

		if ( Ser.IsLoading() )
		{
			dword SlotCount;

			Ser << SlotCount;

			Assert( SlotCount == mSlotArray.size() );
		}
		else
		{
			Ser << (dword)mSlotArray.size();
		}

		if ( Ser.GetVersion() == 3 )
		{
			Ser << mScenePos;
		}

		// 保存Slot数据
		for ( dword i = 0;i< mSlotArray.size();i++)
		{
			MaterialSlot* Slot = mSlotArray[i];
			Ser << Slot;
		}
	}

	void MaterialNode::Disconnect()
	{
		// 断开输入
		for ( dword i= 0;i< mSlotArray.size();i++)
		{
			MaterialSlot* Slot = mSlotArray[i];
			Slot->Disconnect( this );
		}


		// 断开输出
		for ( SlotList::iterator it = mOutputSlotList.begin();
			it != mOutputSlotList.end();
			++it )
		{
			MaterialSlot* Slot = *it;
			Slot->Disconnect(this);
		}

		mOutputSlotList.clear();
	}

	void MaterialNode::AddOutputSlot( MaterialSlot* Slot )
	{
		Assert( std::find( mOutputSlotList.begin(), mOutputSlotList.end(), Slot ) == mOutputSlotList.end() );

		mOutputSlotList.push_back( Slot );
	}

	void MaterialNode::RemoveOutputSlot( MaterialSlot* Slot )
	{
		mOutputSlotList.remove( Slot );
	}

	MaterialSlot* MaterialNode::GetSlotByName( const wchar* Name )
	{
		for ( dword i = 0;i< mSlotArray.size();i++)
		{
			MaterialSlot* Slot = mSlotArray[i];
			if ( StringHelper::Compare(Name, Slot->GetName() ) )
				return Slot;
		}

		return null;
	}



}