#include "EditorPCH.h"
#include "PrivateInclude.h"
//////////////////////////////////////////////////////////////////////////
// QTypeMapperManager
//////////////////////////////////////////////////////////////////////////
QTypeMapperManager* GTypeMapperManager = null;




QTypeMapperManager::~QTypeMapperManager()
{
	for ( MapperMap::iterator it( mMapperMap );it;++it)
	{
		QTypeMapper* Mapper = it.get_value();
		delete Mapper;
	}

	mMapperMap.clear();
}

void QTypeMapperManager::RegisterMapper( QTypeMapper* Mapper )
{
	mMapperMap.set( Mapper->GetTypeName(), Mapper );
}

QTypeMapper* QTypeMapperManager::QueryMapper( const wchar* TypeName )
{
	QTypeMapper** Result = mMapperMap.find( TypeName );
	if ( Result == null )
		return null;

	return *Result;
}

QtVariantProperty* QTypeMapperManager::InitValue( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid )
{
	QTypeMapper* Mapper = QueryMapper( PV->GetTypeName() );
	if ( Mapper == null )
	{
		Assert("TypeMapper not found");
		return null;
	}

	return Mapper->InitProperty( Instance, PV, PGrid );
}

bool QTypeMapperManager::SaveProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, const QVariant& Value )
{
	QTypeMapper* Mapper = QueryMapper( PV->GetTypeName() );
	if ( Mapper == null )
		return null;

	return Mapper->SaveProperty( Instance, PV, PGrid, Value );
}

// 单体类型映射，无需转换
template<class TType, int QtVariantTypeIndex>
class QMapperTemplate : public QTypeMapper
{
public:

	virtual const wchar* GetTypeName( )
	{
		return typeconv<TType>().value_typenameW();
	}

	virtual QtVariantProperty* InitProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid )
	{
		QtVariantProperty* Prop = PGrid->GetVarManager()->addProperty(QtVariantTypeIndex, QString::fromWCharArray( PV->GetName() ) );

		LoadProperty( Instance, PV, PGrid, Prop  );

		return Prop;
	}

	virtual bool SaveProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, const QVariant& Value)
	{
		TType Data = qVariantValue<TType>(Value);

		return PV->Set( Instance, &TPropertyValueRef<TType>( Data, typeconv<TType>().value_typenameW() ) );
	}

	virtual bool LoadProperty( RTTIObject* Instance, PropertyValue* PV,  QPropertyGrid* PGrid, QtVariantProperty* Prop )
	{
		TType Data;
		if ( !PV->Get( Instance,&TPropertyValueRef<TType>( Data, typeconv<TType>().value_typenameW() ) ) )
			return false;

		Prop->setValue( Data );
		return true;
	}

};

// 模板类型映射，引擎类型在Qt类型中没有对应时，需要强制转换的，保证内存序与引擎类型一致即可
template<class TType, int QtVariantTypeIndex, class TForceType >
class QMapperTemplateForce : public QTypeMapper
{
public:
	virtual const wchar* GetTypeName( )
	{
		return typeconv<TType>().value_typenameW();
	}

	virtual QtVariantProperty* InitProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid )
	{
		QtVariantProperty* Prop = PGrid->GetVarManager()->addProperty(QtVariantTypeIndex, QString::fromWCharArray( PV->GetName() ) );

		LoadProperty( Instance, PV, PGrid, Prop  );

		return Prop;
	}

	virtual bool SaveProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, const QVariant& Value)
	{
		TForceType Data = qVariantValue<TForceType>(Value);

		TType* LocalType = (TType*)&Data;
		Assert( LocalType != null );

		return PV->Set( Instance, &TPropertyValueRef<TType>( *LocalType, typeconv<TType>().value_typenameW() ) );
	}

	virtual bool LoadProperty( RTTIObject* Instance, PropertyValue* PV,  QPropertyGrid* PGrid, QtVariantProperty* Prop )
	{
		TType Data;
		if ( !PV->Get( Instance,&TPropertyValueRef<TType>( Data, typeconv<TType>().value_typenameW() ) ) )
			return false;

		Prop->setValue( *((TForceType*)&Data) );
		return true;
	}

};

// Color类型映射
class QMapperColor : public QTypeMapper
{
public:
	virtual const wchar* GetTypeName( )
	{
		return typeconv<Color>().value_typenameW();
	}

	virtual QtVariantProperty* InitProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid )
	{
		QtVariantProperty* Prop = PGrid->GetVarManager()->addProperty(QVariant::Color, QString::fromWCharArray( PV->GetName() ) );

		LoadProperty( Instance, PV, PGrid,  Prop );

		return Prop;
	}

	virtual bool SaveProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, const QVariant& Value)
	{
		QColor Data = qVariantValue<QColor>(Value);
		Color LocalData(Data.alphaF(), Data.redF(), Data.greenF(), Data.blueF() );

		return PV->Set( Instance, &PROPERTY_VALUE(Color, LocalData) );
	}

	virtual bool LoadProperty( RTTIObject* Instance, PropertyValue* PV,  QPropertyGrid* PGrid, QtVariantProperty* Prop )
	{
		Color Data;
		if ( !PV->Get( Instance,&PROPERTY_VALUE(Color, Data) ) )
			return false;

		Prop->setValue( QColor::fromRgbF( Data.r, Data.g, Data.b, Data.a ) );
		return true;
	}

};

// Vector2类型映射
class QMapperVector2 : public QTypeMapper
{
public:
	virtual const wchar* GetTypeName( )
	{
		return typeconv<Vector2>().value_typenameW();
	}


	virtual QtVariantProperty* InitProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid )
	{
		QtVariantProperty* Prop = PGrid->GetVar2Manager()->addProperty(QVariant::PointF, QString::fromWCharArray( PV->GetName() ) );

		LoadProperty( Instance, PV, PGrid, Prop  );

		return Prop;
	}

	virtual bool SaveProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, const QVariant& Value)
	{
		QPointF Data = qVariantValue<QPointF>(Value);
		Vector2 LocalData(Data.x(), Data.y() );

		return PV->Set( Instance, &PROPERTY_VALUE(Vector2, LocalData) );
	}

	virtual bool LoadProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, QtVariantProperty* Prop )
	{
		Vector2 Data;
		if ( PV->Get( Instance,&PROPERTY_VALUE(Vector2, Data) ) )
		{
			PGrid->GetVar2Manager()->setValue( Prop, QPointF( Data.x, Data.y ));
		}

		return true;
	}
};

// Vector3类型映射
class QMapperVector3 : public QTypeMapper
{
public:
	virtual const wchar* GetTypeName( )
	{
		return typeconv<Vector3>().value_typenameW();
	}


	virtual QtVariantProperty* InitProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid )
	{
		QtVariantProperty* Prop = PGrid->GetVar3Manager()->addProperty(QVariant::RectF, QString::fromWCharArray( PV->GetName() ) );


		LoadProperty( Instance, PV, PGrid, Prop  );

		return Prop;
	}

	virtual bool SaveProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, const QVariant& Value)
	{
		QRectF Data = qVariantValue<QRectF>(Value);
		Vector3 LocalData(Data.x(), Data.y(), Data.width());

		return PV->Set( Instance, &PROPERTY_VALUE(Vector3, LocalData) );
	}

	virtual bool LoadProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, QtVariantProperty* Prop )
	{
		Vector3 Data;
		if ( PV->Get( Instance,&PROPERTY_VALUE(Vector3, Data) ) )
		{
			PGrid->GetVar3Manager()->setValue( Prop, QRectF( Data.x, Data.y, Data.z, 0.0f ));
		}

		return true;
	}
};


// WString类型映射
class QMapperWString : public QTypeMapper
{
public:
	virtual const wchar* GetTypeName( )
	{
		return typeconv<WString>().value_typenameW();
	}


	virtual QtVariantProperty* InitProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid )
	{
		QtVariantProperty* Prop = PGrid->GetVar3Manager()->addProperty(QVariant::String, QString::fromWCharArray( PV->GetName() ) );


		LoadProperty( Instance, PV, PGrid, Prop  );

		return Prop;
	}

	virtual bool SaveProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, const QVariant& Value)
	{
		QChar Data = qVariantValue<QChar>(Value);
		

		return PV->Set( Instance, &PROPERTY_VALUE(WString, Value.toString().toStdWString() ) );
	}

	virtual bool LoadProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, QtVariantProperty* Prop )
	{
		WString Data;
		if ( PV->Get( Instance,&PROPERTY_VALUE(WString, Data) ) )
		{
			Prop->setValue( QString::fromStdWString( Data ) );
		}

		return true;
	}
};


#define QMAPPER_TYPE( EngineType, QtType ) new QMapperTemplate<EngineType, QtType >( )
#define QMAPPER_TYPE_FORCE( EngineType, QtType, ForceType ) new QMapperTemplateForce<EngineType, QtType, ForceType >(  )


QTypeMapperManager::QTypeMapperManager()
{
	RegisterMapper( QMAPPER_TYPE( int, QVariant::Int ) );
	RegisterMapper( QMAPPER_TYPE( bool, QVariant::Bool ) );
	RegisterMapper( QMAPPER_TYPE( float, QVariant::Double ) );
	RegisterMapper( QMAPPER_TYPE_FORCE( dword, QVariant::Int, int ) );
	RegisterMapper( QMAPPER_TYPE_FORCE( Rect, QVariant::Rect, QRect ) );
	RegisterMapper( QMAPPER_TYPE_FORCE( Point, QVariant::Point,QPoint ) );
	RegisterMapper( new QMapperColor );
	RegisterMapper( new QMapperVector2 );
	RegisterMapper( new QMapperVector3 );
	RegisterMapper( new QMapperWString );
}
