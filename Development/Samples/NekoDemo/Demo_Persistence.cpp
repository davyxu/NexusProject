#include "EngineDemoPCH.h"
#include "PCH_Scene.h"

IMPLEMENT_RTTI_CLASS(Demo_Persistence)

struct SomeStruct 
{
	float mPosition;
	Vector3 mNormal;

	friend BinarySerializer& operator <<( BinarySerializer& Ser, SomeStruct& Data )
	{
		Ser << Data.mPosition << Data.mNormal;

		return Ser;
	}
};

class MyClass1 : public RTTIObject
{
	DECLARE_RTTI_CLASS(MyClass1, RTTIObject, ClassFlag_Intrinsic|ClassFlag_Serializable)

	SomeStruct mStruct;

	virtual void Serialize( BinarySerializer& Ser )
	{
		Ser << mStruct;
	}
};

IMPLEMENT_RTTI_CLASS( MyClass1);

class MyClass2 : public RTTIObject
{
	DECLARE_RTTI_CLASS(MyClass2, RTTIObject, ClassFlag_Intrinsic|ClassFlag_Serializable )

	AString mData;
	int mData2;

	virtual void Serialize( BinarySerializer& Ser )
	{
		Ser << mData << mData2;
	}
};
IMPLEMENT_RTTI_CLASS( MyClass2);


Demo_Persistence::Demo_Persistence()
{
	RTTI_REGISTER( MyClass1 );
	RTTI_REGISTER( MyClass2 );


}


class ClassA : public RTTIObject
{
public:
	DECLARE_RTTI_CLASS( ClassA, RTTIObject, ClassFlag_Intrinsic )

	virtual void Serialize( BinarySerializer& Ser )
	{
		__super::Serialize( Ser );

		Ser << a;
	}

	int a;
private:
};
IMPLEMENT_RTTI_CLASS( ClassA )

class ClassB: public ClassA
{
	DECLARE_RTTI_CLASS( ClassB,ClassA, ClassFlag_Intrinsic|ClassFlag_Serializable )

	virtual void Serialize( BinarySerializer& Ser )
	{
		__super::Serialize( Ser );

		Ser << b;
	}

public:
	float b;
private:
};
IMPLEMENT_RTTI_CLASS( ClassB)

class ClassC : public ClassB
{
	DECLARE_RTTI_CLASS( ClassC, ClassB, ClassFlag_Intrinsic|ClassFlag_Serializable )
	virtual void Serialize( BinarySerializer& Ser )
	{
		__super::Serialize( Ser );

		Ser << c << d;
	}
public:
	_byte c;
	WString d;
private:
};
IMPLEMENT_RTTI_CLASS( ClassC )

class ClassD: public ClassA
{
	DECLARE_RTTI_CLASS( ClassD, ClassA, ClassFlag_Intrinsic|ClassFlag_Serializable )
	virtual void Serialize( BinarySerializer& Ser )
	{
		__super::Serialize( Ser );

		Ser << d;
	}
public:
	WString d;
private:
};

IMPLEMENT_RTTI_CLASS( ClassD)

void CastBenchMark( )
{
	ClassC c;
	ClassD d;


	ClassA* fakeC = &c;
	ClassA* fakeD = &d;

	Assert( c.IsKindOf( RTTI_CLASS_INFO(ClassA) ) );
	Assert( c.IsKindOf( RTTI_CLASS_INFO(ClassB) ) );
	Assert( c.IsKindOf( RTTI_CLASS_INFO(ClassC) ) );
	Assert( c.IsKindOf( RTTI_CLASS_INFO(ClassD) ) == false);
	Assert( d.IsKindOf( RTTI_CLASS_INFO(ClassA) ) );
	Assert( d.IsKindOf( RTTI_CLASS_INFO(ClassB) ) == false  );

	const int TestTimes = 10000;

	float t1 = TimeSource::GetAppTime();

	for ( int i = 0;i<TestTimes;i++)
	{
		ClassC* realC = dynamic_cast<ClassC*>(fakeC);
		ClassD* realD = dynamic_cast<ClassD*>(fakeD);
	}

	float t2 = TimeSource::GetAppTime() - t1;

	for ( int i = 0;i<TestTimes;i++)
	{
		ClassC* realC = fakeC->Cast<ClassC>( );
		ClassD* realD = fakeD->Cast<ClassD>( );
	}

	float t3 = TimeSource::GetAppTime() - t2;

	SimpleLog log;
	log.Debug(L"%f  %f", t2, t3);
}


void SerializeTest( )
{
	ClassC SObjWrite;

	SObjWrite.a = 1;
	SObjWrite.b = 2.0f;
	SObjWrite.c = 3;
	SObjWrite.d = L"4";

	HeapStream Stream;
	BinarySerializer SerWrite( &Stream, false );
	SerWrite.SerializerFileHeader( 1 );

	SerWrite << SObjWrite;

	BinarySerializer SerRead( &Stream, true );
	SerRead.SerializerFileHeader( 0 );
	RTTIObject* SObjRead;
	SerRead << SObjRead;

	delete SObjRead;
}



void Demo_Persistence::OnEnter()
{
	// 序列化输出
	{
		FileStream Stream(L"t",FAM_Write );

		BinarySerializer Ser( &Stream, false );

		MyClass1 a1;
		a1.mStruct.mNormal = Vector3(1,2,3);
		a1.mStruct.mPosition = 5.0f;

		MyClass2 a2;
		a2.mData = "hello";
		a2.mData2 = 109;

		Ser << a1 << a2;
	}

	// 序列化读入
	{
		FileStream Stream(L"t",FAM_Read );

		BinarySerializer Ser( &Stream, true );

		MyClass1* a1;
		MyClass2* a2;

		Ser << a1 << a2;
		delete a1;
		delete a2;
	}

	
	{
		typedef std::vector<HeapStream*> MBuffer;
		MBuffer mMBuffer;

		// 指针类型存储
		HeapStream Data;
		Data.WriteUNICODEString(L"hello");
		mMBuffer.push_back( &Data );

		HeapStream OutStream;
		BinarySerializer WriteBS( &OutStream, false );
		WriteBS << mMBuffer ;

		// 复位
		OutStream.Seek( 0, MSO_Begin );
		mMBuffer.clear();

		// 指针类型读取
		BinarySerializer ReadBS( &OutStream, true );
		ReadBS << mMBuffer;
		delete mMBuffer[0];
	}


	RTTI_REGISTER( ClassA );
	RTTI_REGISTER( ClassB );
	RTTI_REGISTER( ClassC );
	RTTI_REGISTER( ClassD );

}

void Demo_Persistence::OnLeave()
{
	
}

