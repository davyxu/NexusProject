#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Reflection
	*  @{
	*/

	// Work with RTTIObject
	class DataStream;

	/** 
		二进制序列化类
	*/
	class BinarySerializer
	{
	public:
		/** 
			构造时作为序列化的流来源
			@param
				Stream 流来源
			@param
				IsLoading 是否为加载
		*/
		BinarySerializer( DataStream* Stream, bool IsLoading ); 

		static void StaticInit( );

		static void StaticExit( );

		/// 是否为加载模式
		bool IsLoading( ){ return mIsLoading; }

		/// 是否为保存模式
		bool IsSaving( ){ return !mIsLoading; }

		/// 读取文件头后,是否正常

		bool IsStreamOK( ){ return mStreamOK; }
		/** 
			序列化文件头
			@remark
				Loading时，Version填0
		*/
		void SerializerFileHeader( dword  Version );

		/** 
			写RTTI对象头
		*/
		void WriteObject( RTTIObject* Obj );

		/** 
			读取一个RTTI对象
		*/
		RTTIObject* ReadObject(  );

		/** 
			取序列化所用的流
		*/
		DataStream* GetStream( );

		/** 
			取得版本号
		*/
		dword GetVersion(){ return mVersion; }

	public:

		/// 解决Constant的输出问题
		template<typename TType>
		friend BinarySerializer& operator<<( BinarySerializer& Ser, const TType& Data )
		{
			Assert ( Ser.IsSaving() );

			Ser.GetStream()->WriteTypes( Data );

			return Ser;
		}

		/// 各种类型（不保护动态分配内存成员）
		template<typename TType>
		friend BinarySerializer& operator<<( BinarySerializer& Ser, TType& Data )
		{
			if ( Ser.IsLoading() )
			{
				Ser.GetStream()->ReadTypes( Data );
			}
			else
			{
				Ser.GetStream()->WriteTypes( Data );
			}

			return Ser;
		}

		/// 数组类型
		template<typename TType>
		friend BinarySerializer& operator<<( NekoEngine::BinarySerializer& Ser, std::vector<TType>& Data )
		{
			dword Size = 0;

			if ( Ser.IsLoading() )
			{
				// 取size
				Ser << Size;
				Data.resize( Size );
			}
			else
			{
				// 存size
				Size = Data.size();
				Ser << Size;
			}

			// 存取内容
			for ( dword i = 0;i< Size;i++)
			{
				Ser << Data[i];
			}

			return Ser;
		}

		/// 数组指针类型
		template<typename TType>
		friend BinarySerializer& operator<<( NekoEngine::BinarySerializer& Ser, std::vector<TType*>& Data )
		{
			dword Size = 0;

			if ( Ser.IsLoading() )
			{
				// 取size
				Ser << Size;
				Data.resize( Size );

				// 存取内容
				for ( dword i = 0;i< Size;i++)
				{
					TType* Mem = null;		// 在自己的类型重载中需要添加new
					Ser << Mem;
					Data[i] = Mem;
				}
			}
			else
			{
				// 存size
				Size = Data.size();
				Ser << Size;

				// 存取内容
				for ( dword i = 0;i< Size;i++)
				{
					Ser << Data[i];
				}
			}

			return Ser;
		}

		/// 列表指针类型
		template<typename TType>
		friend BinarySerializer& operator<<( NekoEngine::BinarySerializer& Ser, std::list<TType*>& Data )
		{
			if ( Ser.IsLoading() )
			{
				dword Size = 0;
				// 取size
				Ser << Size;

				// 存取内容
				for ( dword i = 0;i< Size;i++)
				{
					TType* Mem;
					Ser << Mem;
					Data.push_back( Mem );
				}
			}
			else
			{
				// 存size
				Ser << (dword)Data.size();

				// 存取内容
				for ( std::list<TType*>::iterator it = Data.begin();
					it != Data.end();
					++it)
				{
					TType* Mem = *it;
					Ser << Mem;
				}
			}

			return Ser;
		}

		/// 列表类型
		template<typename TType>
		friend BinarySerializer& operator<<( NekoEngine::BinarySerializer& Ser, std::list<TType>& Data )
		{
			if ( Ser.IsLoading() )
			{
				dword Size = 0;
				// 取size
				Ser << Size;

				// 存取内容
				for ( dword i = 0;i< Size;i++)
				{
					TType Mem;
					Ser << Mem;
					Data.push_back( Mem );
				}
			}
			else
			{
				// 存size
				Ser << (dword)Data.size();

				// 存取内容
				for ( std::list<TType>::iterator it = Data.begin();
					it != Data.end();
					++it)
				{
					TType& Mem = *it;
					Ser << Mem;
				}
			}

			return Ser;
		}


		/// map类型
		template<typename TKey, typename TValue >
		friend BinarySerializer& operator<<( NekoEngine::BinarySerializer& Ser, std::map<TKey, TValue>& Data )
		{

			if ( Ser.IsLoading() )
			{
				dword Size = 0;
				Ser << Size;
				for ( dword i = 0;i< Size;i++)
				{
					TKey Key;
					TValue Value;
					Ser << Key << Value;

					Data[ Key ] = Value;
				}
			}
			else
			{
				Ser << (dword)Data.size();

				// 存取内容
				for ( std::map<TKey, TValue>::iterator it = Data.begin();
					it != Data.end();
					++it)
				{
					Ser << it->first << it->second;
				}
			}
	

			return Ser;
		}

		/// Hash类型
		template<typename TKey, typename TValue >
		friend BinarySerializer& operator<<( NekoEngine::BinarySerializer& Ser, dynamic_hash<TKey, TValue>& Data )
		{

			if ( Ser.IsLoading() )
			{
				dword Size = 0;
				Ser << Size;
				for ( dword i = 0;i< Size;i++)
				{
					TKey Key;
					TValue Value;
					Ser << Key << Value;

					Data.set( Key, Value );
				}
			}
			else
			{
				Ser << (dword)Data.size();

				// 存取内容
				for ( dynamic_hash<TKey, TValue>::iterator it( Data );
					it;
					++it)
				{
					Ser << it.get_key( ) << it.get_value();
				}
			}


			return Ser;
		}

		/// 基类序列化
		template<typename TClass>
		BinarySerializer& SerializeObject( TClass*& Obj )
		{
			if ( IsLoading() ) 
			{ 
				RTTIObject* ReadedObj = ReadObject(  );

				Obj = RTTI_CAST(TClass, ReadedObj );

				// 给定的接收类型与读取的类型无法转换
				Assert( ReadedObj != null && Obj != null && "Can not cast to given object type while serialization");

				// 释放已经创建的对象
				if ( ReadedObj != null && Obj == null )
				{
					delete ReadedObj;
				}
			} 
			else 
			{	
				WriteObject( Obj );
			} 

			if ( Obj != null ) 
				Obj->Serialize( *this ); 

			return *this;
		}
		

	private:
		DataStream* mStream;
		bool mIsLoading;
		dword mVersion;
		bool mStreamOK;
		static Logger* mLogger;
	};

	/** @} */
	/** @} */
}