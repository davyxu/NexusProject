#include "EditorPCH.h"
#include "PrivateInclude.h"


class MyReflectionTest : public RTTIObject
{
	DECLARE_RTTI_CLASS( MyReflectionTest, RTTIObject, ClassFlag_Intrinsic );

	MyReflectionTest( )
	{
		mInteger = 12345;
		mBool =true;
		mFloat  = 1.2345f;
		mDword = -1;		
		mRect = Rect( 1,2,3,4);
		mPoint= Point( 6,7);
		mColor = Color::cYellow;
		mVector2 = Vector2( 1,2);
		mVector3 = Vector3( 3,4,5);
	}

	static void StaticInitProperty( )
	{
		PropertyCollection* PC = RTTI_CLASS_INFO(MyReflectionTest)->GetPropertyCollection( );

		PC->Add( PROPERTY_MEMBER_VAR( MyReflectionTest,int, mInteger ) )->SetReadOnly( true );
		PC->Add( PROPERTY_MEMBER_VAR( MyReflectionTest,bool, mBool ) );
		PC->Add( PROPERTY_MEMBER_VAR( MyReflectionTest,float, mFloat ) );
		PC->Add( PROPERTY_MEMBER_VAR( MyReflectionTest,dword, mDword ) );
		PC->Add( PROPERTY_MEMBER_VAR( MyReflectionTest,Rect, mRect ) );
		PC->Add( PROPERTY_MEMBER_VAR( MyReflectionTest,Point, mPoint ) );
		PC->Add( PROPERTY_MEMBER_VAR( MyReflectionTest,Color, mColor ) )->SetReadOnly( true );
		
		PC->Add( PROPERTY_MEMBER_VAR( MyReflectionTest,Vector2, mVector2 ) );
		PC->Add( PROPERTY_MEMBER_VAR( MyReflectionTest,Vector3, mVector3 ) )->SetReadOnly( true );
	}

private:
	int mInteger;
	bool mBool;
	float mFloat;
	dword mDword;
	Rect mRect;
	Point mPoint;
	Color mColor;
	Vector2 mVector2;
	Vector3 mVector3;
};

IMPLEMENT_RTTI_CLASS( MyReflectionTest )

//////////////////////////////////////////////////////////////////////////
// QPropertyGrid
//////////////////////////////////////////////////////////////////////////
QPropertyGrid::QPropertyGrid( QWidget* parent, bool MarginSide )
: mEnableValueChangeNotify( true )
, mNeedClear( false )
, QWidget( parent )
{
	setWindowTitle(QString("Properies"));
	mVariantManager = new QtVariantPropertyManager( this );	

	mVariantEditorFactory = new QtVariantEditorFactory( this );
	mPropertyBrowser = new QtTreePropertyBrowser( this );
	mPropertyBrowser->setFactoryForManager( mVariantManager, mVariantEditorFactory );

	mGroupManager = new QtGroupPropertyManager( this );

	// 扩展类型
	mVector2Manager = new Vector2Manager(this);
	mVector3Manager = new Vector3Manager(this);
	mVector4Manager = new Vector4Manager(this);
	mPropertyBrowser->setFactoryForManager( (QtVariantPropertyManager*)mVector2Manager, mVariantEditorFactory );
	mPropertyBrowser->setFactoryForManager( (QtVariantPropertyManager*)mVector3Manager, mVariantEditorFactory );
	mPropertyBrowser->setFactoryForManager( (QtVariantPropertyManager*)mVector4Manager, mVariantEditorFactory );


	//mTestButton = new QPushButton(tr("Test"), this);

	//connect(mTestButton, SIGNAL(clicked()), this, SLOT(OnTestButtonClicked()));

	QVBoxLayout *Layout = new QVBoxLayout( this );
	Layout->addWidget(mPropertyBrowser,1,0);

	if ( MarginSide )
		Layout->setContentsMargins(2, 2, 2, 2);
	else
		Layout->setContentsMargins(0, 0, 0, 0);

	setLayout(Layout);
}


QPropertyGrid::~QPropertyGrid()
{

}

void QPropertyGrid::OnTestButtonClicked( )
{
	/*static MyReflectionTest* TestObj = null;
	if ( TestObj == null )
	{
		TestObj = new MyReflectionTest;
	}*/
	//ReflectProperties( new QReflectionObject( GEditor->mActor.GetPtr() ) );
}

void QPropertyGrid::ReflectProperties( ReflectionObject* Object )
{
	Assert( Object != null );

	if ( mNeedClear )
	{
		ClearProperties();
	}


	mEnableValueChangeNotify = false;


	for ( dword GroupIndex = Object->GetFirstGroup();
		GroupIndex != ReflectionObject::InvalidGroupID;
		GroupIndex = Object->GetNextGroup() )
	{

		QtProperty* GroupItem = mGroupManager->addProperty( QString::fromWCharArray( Object->GetGroupName( GroupIndex ) ) );

		mPropertyBrowser->addProperty( GroupItem );

		for ( PropertyValue* PV = Object->GetFirstProperty( GroupIndex );
			PV != null; 
			PV = Object->GetNextProperty( GroupIndex ) )
		{
			RTTIObject* Instance = Object->GetGroupMappedInstance( GroupIndex );
			QtVariantProperty* Prop = GTypeMapperManager->InitValue( Instance, PV, this );
			Prop->setEnabled( !PV->GetReadOnly() );

			GroupItem->addSubProperty( Prop );

			mPropertyMapper.set( Prop, MappedData( PV, Instance ) );
		}

	}

	mEnableValueChangeNotify = true;


	connect(mVariantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),	this, SLOT(valueChanged(QtProperty *, const QVariant &)));
	connect(mVector2Manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),	this, SLOT(valueChanged(QtProperty *, const QVariant &)));
	connect(mVector3Manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),	this, SLOT(valueChanged(QtProperty *, const QVariant &)));
	connect(mVector4Manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),	this, SLOT(valueChanged(QtProperty *, const QVariant &)));

	mNeedClear = true;
}

void QPropertyGrid::ClearProperties( )
{
	disconnect(mVariantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),	this, SLOT(valueChanged(QtProperty *, const QVariant &)));
	disconnect(mVector2Manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),	this, SLOT(valueChanged(QtProperty *, const QVariant &)));
	disconnect(mVector3Manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),	this, SLOT(valueChanged(QtProperty *, const QVariant &)));
	disconnect(mVector4Manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),	this, SLOT(valueChanged(QtProperty *, const QVariant &)));


	mPropertyBrowser->clear();
	mVector2Manager->clear();
	mVector3Manager->clear();
	mVector4Manager->clear();
	mVariantManager->clear();

	mPropertyMapper.clear();

	mNeedClear = false;
}

void QPropertyGrid::valueChanged(QtProperty* Prop, const QVariant& Value)
{
	if ( !mEnableValueChangeNotify )
		return;


	// 仅在UI上编辑时才调用到这里
	MappedData* Result = mPropertyMapper.find( dynamic_cast<QtVariantProperty*>(Prop) );
	if ( Result == null )
		return;

	WString StrValue;
	if ( Result->mValue ->ToString( Result->mInstance, StrValue ) )
	{
		GCommandManager->Add( new CmdSetObjProperty( Result->mValue ->GetName(), StrValue ) );
	}

	GTypeMapperManager->SaveProperty( Result->mInstance, Result->mValue, this, Value );
}
