#pragma once

class QPropertyGrid;

// 实现引擎属性到Qt属性的转换封装
class QTypeMapper
{
public:
	// 映射的引擎类型
	virtual const wchar* GetTypeName( ) = 0;

	// 初始创建属性
	virtual QtVariantProperty* InitProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid  ) = 0;

	// 引擎属性到Qt属性设置
	virtual bool LoadProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, QtVariantProperty* Prop ) = 0;

	// Qt属性设置到引擎属性
	virtual bool SaveProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, const QVariant& Value) = 0;
};



// 类型映射器管理器
class QTypeMapperManager
{
public:
	QTypeMapperManager( );
	virtual ~QTypeMapperManager( );

	// 注册类型映射器
	void RegisterMapper( QTypeMapper* Mapper );

	// 查询类型映射器
	QTypeMapper* QueryMapper( const wchar* TypeName );

	// 初始创建属性
	QtVariantProperty* InitValue( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid  );

	// Qt属性设置到引擎属性
	bool SaveProperty( RTTIObject* Instance, PropertyValue* PV, QPropertyGrid* PGrid, const QVariant& Value );

private:
	typedef dynamic_hash<WString, QTypeMapper*> MapperMap;

	// 各种引擎类型到Qt类型的映射
	MapperMap mMapperMap;

};

extern QTypeMapperManager* GTypeMapperManager;