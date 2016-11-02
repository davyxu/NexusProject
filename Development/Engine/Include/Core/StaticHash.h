#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Basic
	*  @{
	*/

	template< typename TKey, typename TValue,typename THashFunction, dword HASHBOUND = 64 >
	class static_hash : public Link< TValue >
	{
	private:
		struct HashNode : public Node
		{
			HashNode*	mLink;
			TKey		mKey;
		};


		HashNode*	mHashArray[HASHBOUND];

	public:
		static_hash( const static_hash& shash )
		{
			assign( shash );
		}

		static_hash( )
		{
			memset( mHashArray, 0, sizeof(mHashArray) );
		}

		~static_hash( )
		{
			clear( );
		}

		const static_hash& operator = ( const static_hash& shash )
		{
			assign( shash );
			return *this;
		}

		void assign( const static_hash& shash )
		{
			clear();

			for ( iterator it = begin();
				it != end();
				++it )
			{
				HashNode* hashnode = (HashNode*)it.mNode;
				insert( hashnode->mKey, hashnode->mElement );
			}
		}

		dword insert( const TKey& k, const TValue& v)
		{
			dword hashcode = THashFunction( )( k );
			dword index = hashcode & (HASHBOUND - 1);

			HashNode* newnode = new HashNode;
			newnode->mKey = k;
			newnode->mElement = v;

			Assert( index < HASHBOUND );
			newnode->mLink = mHashArray[index];
			mHashArray[index] = newnode;

			Link<TValue>::push_back( newnode );

			return index;
		}

		// 查询有3种状态：
		// 1. 元素不存在
		// 3. 元素多个冲突

		HashNode* find_node( const TKey& v )
		{
			THashFunction func;

			dword hashcode = func( v );
			dword index = hashcode & (HASHBOUND - 1);

			HashNode* node = mHashArray[ index ];

			while ( node != null )
			{
				// key比较函数
				if ( func( v, node->mKey ) )
				{
					return node;
				}

				node = node->mLink;
			}

			// 实在找不到了 囧
			return null;
		}

		TValue* find( const TKey& v )
		{
			HashNode* node = find_node( v );
			if ( node == null )
				return null;

			return &node->mElement;
		}

		dword get_slot( const TKey& v )
		{
			THashFunction func;

			dword hashcode = func( v );
			dword index = hashcode & (HASHBOUND - 1);

			return index;
		}


		typename Link< TValue, TValue>::iterator remove( const TKey& v )
		{
			HashNode* node = find_node( v );
			if ( node == null )
				return iterator( null );

			dword hashcode = THashFunction( )( v );
			dword index = hashcode & (HASHBOUND - 1);

			HashNode* p = mHashArray[index];

			// 删除槽上的元素，剩下的元素上移
			if ( p == node )
			{
				mHashArray[index] = node->mLink;
			}
			else
			{
				// 删除槽链表中的元素
				while ( p != null )
				{
					// 找到上一家
					if ( p->mLink == node )
					{
						// 将上家连到我的下家，断开我
						p->mLink = node->mLink;
						break;
					}

					p = p->mLink;
				}
				
			}

			Link::iterator it = Link<TValue>::remove( node );
			
			delete node;

			return it;
		}

		void clear( )
		{

			for ( iterator it = begin();
				it != end();
				++it )
			{
				delete it.mNode;
				it.mNode = null;
			}

			Link<TValue>::clear();

			::memset( mHashArray, 0, sizeof mHashArray );
		}

		dword size( )
		{
			return Link<TValue>::size();
		}
	};

	/** @} */
	/** @} */
}