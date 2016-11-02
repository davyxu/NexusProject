#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Basic
	*  @{
	*/


	/** 
		Hash函数
		@remarks
			定义全局类型重载函数，对指定类型的函数计算Hash值
		@note
			定义为frind全局时，可以忽略代码前后关联，更为方便
	*/
	inline dword GetTypeHash( const word A )
	{
		return A;
	}
	
	inline dword GetTypeHash( const int A )
	{
		return A;
	}
	inline dword GetTypeHash( const dword A )
	{
		return A;
	}
	inline dword GetTypeHash( const qword A )
	{
		return (dword)A+((dword)(A>>32) * 23);
	}

	inline dword GetTypeHash( const wchar* s )
	{
		return CheckSum::StrHashNoCase( s );
	}

	inline dword GetTypeHash( const char* s )
	{
		return CheckSum::StrHashNoCase( s );
	}

	inline dword GetTypeHash( const AString& s )
	{
		return CheckSum::StrHashNoCase( s.c_str() );
	}

	inline dword GetTypeHash( const WString& s )
	{
		return CheckSum::StrHashNoCase( s.c_str() );
	}

	template<class T>
	inline dword PointerHash(const T* Key,dword C = 0)
	{
#define mix(a,b,c) \
		{ \
		a -= b; a -= c; a ^= (c>>13); \
		b -= c; b -= a; b ^= (a<<8); \
		c -= a; c -= b; c ^= (b>>13); \
		a -= b; a -= c; a ^= (c>>12);  \
		b -= c; b -= a; b ^= (a<<16); \
		c -= a; c -= b; c ^= (b>>5); \
		a -= b; a -= c; a ^= (c>>3);  \
		b -= c; b -= a; b ^= (a<<10); \
		c -= a; c -= b; c ^= (b>>15); \
	}

		dword A;
		dword B;
		A = B = 0x9e3779b9;
		A += (*(dword*)&Key);
		mix(A,B,C);
		return C;

#undef mix
	}

	/** 
		动态Hash表基类
	*/
	template< class TKey, class TValue , int INIT_HASH_COUNT = 8 > class map_base
	{
	protected:
		class TPair
		{
		public:
			int HashNext;
			TKey Key;
			TValue Value;
			TPair( TKey InKey, TValue InValue )
			: Key( InKey ), Value( InValue )
			{}
			TPair()
			{

			}
		};

		void rehash()
		{
			Assert(!(HashCount&(HashCount-1)));
			delete[] Hash;
			Hash = new int[HashCount];
			for( int i=0; i<HashCount; i++ )
			{
				Hash[i] = -1;
			}
			for( int i=0; i< (int)Pairs.size(); i++ )
			{
				TPair& Pair    = Pairs[i];
				int    iHash   = (GetTypeHash(Pair.Key) & (HashCount-1));
				Pair.HashNext  = Hash[iHash];
				Hash[iHash] = i;
			}
		}

		void allochash()
		{
			if(!Hash)
			{
				rehash();
			}
		}


		void relax()
		{
			while( HashCount>(int)Pairs.size()*2+INIT_HASH_COUNT )
			{
				HashCount /= 2;
			}
			rehash();
		}

		TValue& add( const TKey& InKey, const TValue& InValue )
		{			
			size_t beforesize = Pairs.size();
	
			Pairs.push_back( TPair() );

			TPair& Pair = Pairs.back();
			Pair.Key = InKey;
			Pair.Value = InValue;

			int    iHash  = (GetTypeHash(Pair.Key) & (HashCount-1));
			Pair.HashNext = Hash[iHash];
			Hash[iHash]   = beforesize;

			if( HashCount*2 + INIT_HASH_COUNT < (int)Pairs.size() )
			{
				HashCount *= 2;
				rehash();
			}
			return Pair.Value;
		}

		typedef std::vector<TPair> PairsArray;
		PairsArray Pairs;

		int* Hash;
		int HashCount;

	public:
		/** 
			构造函数
			@note
				使用模板参数INIT_HASH_COUNT进行初始hash槽数量设置
		*/
		map_base()
		:	Hash( null )
		,	HashCount( INIT_HASH_COUNT )
		{}
		
		/**
			拷贝构造函数
		*/
		map_base( const map_base& Other )
		:	Pairs( Other.Pairs )
		,	Hash( null )
		,	HashCount( Other.HashCount )
		{
			rehash();
		}

		/**
			析构函数
		*/
		~map_base()
		{
			delete [] Hash;
			Hash = null;
			HashCount = 0;
		}

		map_base& operator=( const map_base& Other )
		{
			Pairs     = Other.Pairs;
			HashCount = Other.HashCount;
			rehash();
			return *this;
		}

		/**	
			清除所有元素
			@remarks
				此函数会删除内存	
		*/
		void clear()
		{
			Assert(!(HashCount&(HashCount-1)));
			Pairs.clear();
			HashCount = INIT_HASH_COUNT;
			delete [] Hash;
			Hash = null;
		}

		/** 
			重置所有元素
			@remarks 此函数不会造成内存的重新分配
		 */
		void reset()
		{
			Assert(!(HashCount&(HashCount-1)));
			Pairs.Reset();
			if( Hash )
			{
				for( int i=0; i<HashCount; i++ )
				{
					Hash[i] = -1;
				}
			}
		}

		/** 
			设置Hash元素
			@remark
				此函数将使用key的hash函数选择槽，并放置元素
			@param
				InKey key类型值
			@param
				InValue 变量值
		*/
		TValue& set( const TKey& InKey, const TValue& InValue )
		{
			allochash();
			if ( Pairs.size() > 0 )
			{
				for( int i=Hash[(GetTypeHash(InKey) & (HashCount-1))]; i!= -1; i=Pairs[i].HashNext )
				{
					if( Pairs[i].Key==InKey )
					{
						Pairs[i].Value=InValue;
						return Pairs[i].Value;
					}
				}
			}
			return add( InKey, InValue );
		}

		/** 
			使用key移除一个hash元素
			@param
				InKey 输入Key类型值
			@return
				返回Key对应的Hash元素移除数量
			@note
				注意在必要时提供key的==操作符重载
		*/
		int remove( const TKey& InKey )
		{
			int Count=0;
			for( int i=Pairs.size()-1; i>=0; i-- )
			{
				if( Pairs[i].Key==InKey )
				{
					Pairs.erase(Pairs.begin() + i); Count++;
				}
			}
			if( Count )
			{
				relax();
			}
			return Count;
		}

		/** 
			根据Value查找Key
			@param
				value 输入值
			@return
				未找到结果时，返回null。反之，返回结果的指针
			@note
				此函数性能较差，注意使用环境
		*/
		TKey* findkey( const TValue& Value )
		{
			for( int i = 0 ; i < Pairs.size() ; ++i )
			{
				if( Pairs[i].Value == Value )
				{
					return &Pairs[i].Key;
				}
			}
			return null;
		}

		/** 
			根据Value查找const Key
			@param
				value 输入值
			@return
				未找到结果时，返回null。反之，返回结果的指针
			@note
				此函数性能较差，注意使用环境
		*/
		const TKey* findkey( const TValue& Value ) const
		{
			for( int i = 0 ; i < Pairs.size() ; ++i )
			{
				if( Pairs[i].Value == Value )
				{
					return &Pairs[i].Key;
				}
			}
			return null;
		}

		/** 
			根据Key查找Value
			@param
				Key 需要查找Key值
			@return
				未找到结果时，返回null。反之，返回结果的指针
		*/
		TValue* find( const TKey& Key )
		{
			if( Hash && Pairs.size() > 0 )
			{
				for( int i=Hash[(GetTypeHash(Key) & (HashCount-1))]; i!= -1; i=Pairs[i].HashNext )
				{
					if( Pairs[i].Key==Key )
					{
						return &Pairs[i].Value;
					}
				}
			}
			return null;
		}

		/** 
			根据Key查找Value的引用
			@param
				Key 需要查找Key值
			@return
				未找到结果时，返回null。反之，返回类型空
		*/
		TValue findref( const TKey& Key ) const
		{
			if( Hash && Pairs.size() > 0 )
			{
				for( int i=Hash[(GetTypeHash(Key) & (HashCount-1))]; i!= -1; i=Pairs[i].HashNext )
				{
					if( Pairs[i].Key==Key )
					{
						return Pairs[i].Value;
					}
				}
			}
			return (TValue) null;
		}

		/** 
			根据Key查找const Value
			@param
				Key 需要查找Key值
			@return
				未找到结果时，返回null。反之，返回结果的指针
		*/
		const TValue* find( const TKey& Key ) const
		{
			if( Hash && Pairs.size() > 0 )
			{
				for( int i=Hash[(GetTypeHash(Key) & (HashCount-1))]; i!= -1; i=Pairs[i].HashNext )
				{
					if( Pairs[i].Key==Key )
					{
						return &Pairs[i].Value;
					}
				}
			}
			return null;
		}

		/** 
			检查Key对应的元素是否存在
			@param
				Key 需要查找Key值
			@return
				结果是否存在
		*/
		const bool exist( const TKey& Key ) const
		{
			bool bHasKey = false;
			if( Hash && Pairs.size() > 0 )
			{
				for( int i=Hash[(GetTypeHash(Key) & (HashCount-1))];
					i!= -1 && bHasKey == false; i=Pairs[i].HashNext )
				{
					bHasKey = Pairs[i].Key==Key;
				}
			}
			return bHasKey;
		}

		/// 迭代器
		class iterator
		{
		public:
			/** 
				迭代器构造函数
				@param
					InMap hashmap对象直接进行引用传递
			*/
			iterator( map_base& InMap ) : Map( InMap ), Pairs( InMap.Pairs ), Index( 0 ), Removed(0) {}


			/** 
				迭代器析构函数
				@remark
					如果本次迭代有元素删除操作，将重置hash表索引
			*/
			~iterator()               { if( Removed ) Map.relax(); }

			/// 迭代器指针移动
			void operator++()          { ++Index; }

			/// 移除一个元素
			void remove()       { Pairs.erase(Pairs.begin() + Index--); Removed++; }

			/// 迭代器是否有效
			operator bool() const     { return Index >=0 && Index < (int)Pairs.size() ; }

			/// 获取当前的key
			TKey& get_key() const            { return Pairs[Index].Key; }

			/// 获取当前的value
			TValue& get_value() const          { return Pairs[Index].Value; }
		private:
			map_base& Map;
			PairsArray& Pairs;
			int Index;
			int Removed;
		};

		/** 
			只读迭代器
			参见 map_base::iterator
		*/
		class const_iterator
		{
		public:
			const_iterator( const map_base& InMap ) : Map(InMap), Pairs( InMap.Pairs ), Index( 0 ) {}
			~const_iterator() {}
			void operator++()          { ++Index; }
			operator bool() const     { return  Index >=0 && Index < (int)Pairs.size(); }
			const TKey& get_key() const      { return Pairs[Index].Key; }
			const TValue& get_value() const    { return Pairs[Index].Value; }
		private:
			const map_base& Map;
			const PairsArray& Pairs;
			int Index;
		};
		friend class iterator;
		friend class const_iterator;
	};

	/** 
		动态hash表
		参见 map_base
	*/
	template< class TKey, class TValue, int INIT_HASH_COUNT = 8  > class dynamic_hash : public map_base<TKey,TValue, INIT_HASH_COUNT>
	{
	public:
		dynamic_hash& operator=( const dynamic_hash& Other )
		{
			map_base<TKey,TValue>::operator=( Other );
			return *this;
		}

		int size() const
		{
			return this->Pairs.size();
		}
	};


	/** @} */
	/** @} */
}