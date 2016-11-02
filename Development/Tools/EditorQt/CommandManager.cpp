#include "EditorPCH.h"
#include "PrivateInclude.h"



//////////////////////////////////////////////////////////////////////////
// QCmdCollection
//////////////////////////////////////////////////////////////////////////
CmdCollection::CmdCollection( const WString& Description ) 
: mDescription( Description )
{

}

CmdCollection::~CmdCollection()
{
	for ( dword i = 0;i < mBuffer.size();i++)
	{
		delete mBuffer[i];
	}

	mBuffer.clear();
}

void CmdCollection::Add( CmdBase* Cmd )
{
	mBuffer.push_back( Cmd );
}

void CmdCollection::Execute()
{
	/// 这里仅是倒退历史的情形, 所以多个历史事件必须倒序执行!
	for ( int i = (int)mBuffer.size() - 1;i >= 0 ;i--)
	{
		mBuffer[i]->Execute();
	}
}

void CmdCollection::OnHistoryChanged()
{
	for ( dword i = 0;i < mBuffer.size();i++)
	{
		mBuffer[i]->OnHistoryChanged();
	}
}





//////////////////////////////////////////////////////////////////////////
// QCommandManager
//////////////////////////////////////////////////////////////////////////
CommandManager* GCommandManager = null;

CommandManager::CommandManager() 
: mCurrentIndex( 0 )
, mCollectionMode( false )
{

}

CommandManager::~CommandManager()
{
	Clear();
}

void CommandManager::Add( CmdBase* Cmd )
{
	if ( mCollectionMode )
	{
		mCollections[ mCollections.size() - 1 ]->Add( Cmd );
	}
	else
	{
		// 修改了历史
		if ( mCurrentIndex < mBuffer.size() )
		{
			for ( dword i = mCurrentIndex;i < mBuffer.size();i++)
			{
				CmdBase* Cmd = mBuffer[i];

				Cmd->OnHistoryChanged();

				delete Cmd;
			}

			mBuffer.erase( mBuffer.begin() + mCurrentIndex, mBuffer.end() );
		}

		mBuffer.push_back( Cmd );
		++mCurrentIndex;
	}

}

void CommandManager::Clear()
{
	for ( dword i = 0;i < mBuffer.size();i++)
	{
		CmdBase* Cmd = mBuffer[i];

		//在场景内添加一个茶壶,加载一个茶壶,退出,会造成Clear命令中的茶壶没有释放
		Cmd->OnHistoryChanged();

		delete Cmd;
	}

	mBuffer.clear();

	for ( dword i = 0;i < mCollections.size();i++)
	{
		CmdBase* Cmd = mCollections[i];

		Cmd->OnHistoryChanged();

		delete Cmd;
	}

	mCollections.clear();

	mCollectionMode = false;

	mCurrentIndex = 0 ;
}

void CommandManager::Redo()
{
	
	CmdBase*& Cmd = mBuffer[mCurrentIndex];
	BeginCollection( Cmd->GetDescription() );
	Cmd->Execute();
	delete Cmd;
	Cmd = EndCollection( );

	if ( Cmd == null )	
		mBuffer.erase(mBuffer.begin() + mCurrentIndex);
	else
		++mCurrentIndex;
}

void CommandManager::Undo()
{
	--mCurrentIndex;
	CmdBase*& Cmd = mBuffer[mCurrentIndex];

	BeginCollection( Cmd->GetDescription() );
	Cmd->Execute();
	delete Cmd;
	Cmd = EndCollection( );
	if ( Cmd == null )
	{
		 mBuffer.erase(mBuffer.begin() + mCurrentIndex);
	}
}


void CommandManager::BeginCollection( const WString& Description )
{
	mCollections.push_back( new CmdCollection( Description ) );
	mCollectionMode = true;
}

CmdCollection* CommandManager::EndCollection( bool IsAdd, bool Ignore )
{
	if(mCollections.size() < 1) 
		return null;

	CmdCollection *Coll = mCollections[mCollections.size() -1];

	mCollections.erase(mCollections.end() - 1);

	mCollectionMode = (mCollections.size() > 0);

	if(Coll->IsEmpty() || Ignore)
	{
		delete Coll;
		Coll = null;
	}
	else if(IsAdd)
	{
		Add(Coll);
	}

	return Coll;
}

