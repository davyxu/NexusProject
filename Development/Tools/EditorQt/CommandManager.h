#pragma once

class CmdBase
{
public:
	virtual ~CmdBase( ){ }
	virtual void Execute( ) = 0;

	virtual void OnHistoryChanged( ){}

	virtual WString GetDescription( ){ return L""; }
};

typedef std::vector<CmdBase*> CommandArray;


class CmdCollection : public CmdBase
{
public:
	CmdCollection( const WString& Description );

	virtual ~CmdCollection( );

	void Add( CmdBase* Cmd );

	virtual void Execute( );

	virtual void OnHistoryChanged( );

	bool IsEmpty( ){ return mBuffer.size() == 0; }

private:
	CommandArray mBuffer;
	WString mDescription;
};

class CommandManager
{
public:
	CommandManager();

	~CommandManager( );

	void Add( CmdBase* Cmd );

	void Clear( );

	void Redo( );

	void Undo( );

	bool CanUndo( ){ return mCurrentIndex > 0; }

	bool CanRedo( ){ return mCurrentIndex < mBuffer.size();}

	void BeginCollection( const WString& Description );

	CmdCollection* EndCollection( bool IsAdd = false , bool Ignore = false);

private:
	
	CommandArray mBuffer;
	dword mCurrentIndex;

	bool mCollectionMode;

	typedef std::vector<CmdCollection*> CmdCollectionArray;
	CmdCollectionArray mCollections;
};

extern CommandManager* GCommandManager;