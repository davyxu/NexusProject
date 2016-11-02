#pragma once

namespace NekoEngine
{

	//----------------------------------------------------------------------------
	// Pool
	//----------------------------------------------------------------------------

	//! This class is template container class, represents a pool.

	//! A pool is an array in fact, and allocates a maximal size buffer to store all elements.
	//! But different from array, the buffer will never enlarge, so it can store limited elements.
	//! Each elements are not adjacent in a pool, and linked by a chain inside pool.<br>
	//! Template parameter "Type" defines the element type storaged in the array.<br>
	//! Template parameter "Key" defines the comparable type of the element type.<br>
	//! For example, Pool< ClassA, dword > defines a pool stores ClassA element, and access it
	//! by dword, means ClassA has a overload operator with dword ( ==, !=, > and < etc ), or it can convert to dword.
	template< typename Type, typename Key = Type > class Pool : public Link< Type, Key >
	{
	private:
		//! The node in the pool stores an element, and linked as a chain.
		struct PoolNode : public Node
		{
			//! The index of the node in the pool.
			dword	mIndex;

			//! Constructor, create an empty node, and set the index to -1.
			//! @param		none
			PoolNode( ) : mIndex( -1 ) { }
		};

		//! The memory pointer of the pool.
		PoolNode*	mNodes;
		//! The maximal size of the pool in number of element.
		dword		mSize;
		//! The next index of the empty node.
		dword		mNext;

	public:
		//! Constructor, create an empty pool.
		//! @param		size		The maximal size of the pool in number of element.
		Pool( dword size );
		//! Copy-Constructor, create a pool by copy from another one.
		//! @param		pool		The other pool.
		Pool( const Pool< Type, Key >& pool );
		//! Destructor, delete the pool, and release the memory used by nodes.
		//! @param		none
		~Pool( );

		//! Copy elements from another pool.
		//! @param		pool		The other pool.
		//! @return		The reference of current pool.
		Pool< Type, Key >& operator = ( const Pool< Type, Key >& pool );

		//! Get a const element reference by index.
		//! @param		index		The index of the element in pool.
		//! @return		The const element reference.
		inline const Type& operator [] ( dword index ) const;
		//! Get a mutable element reference by index.
		//! @param		index		The index of the element in pool.
		//! @return		The mutable element reference.
		inline Type& operator [] ( dword index );

		//! Get an element pointer by index.
		//! @param		index		The index of the element in pool.
		//! @return		The pointer to a element if the index is valid, null otherwise.
		//! @remarks	This is a safe way to get an element by index from the pool.
		inline Type* index_item( dword index );

		//! Get an iterator by the index of an element.
		//! @param		index		The index of the element in pool.
		//! @return		An iterator reference to the element.
		inline iterator get_iterator( dword index );

		//! Get the maximal size of the pool in number of element.
		//! @param		none
		//! @return		The size of the pool.
		inline dword max_size( );

		//! Create a node to store element in the pool.
		//! @param		element		The element to be stored in the pool.
		//! @return		The index of the element if it can be stored, or -1 indicates there is no more node to store it.
		dword create( const Type& element );
		//! Insert an element into the pool at specified index.
		//! @param		element		The element to be inserted.
		//! @param		index		The index specifies a position.
		void insert( const Type& element, dword index );
		//! Remove an element from the pool, the element is specified by an iterator.
		//! @param		iterator	The iterator specifies a position.
		void remove( iterator& iterator );
		//! Remove an element from the pool, the element is specified by an index.
		//! @param		index		The index specifies a position.
		void remove( dword index );
		//! Resize the pool by create new memory and copy old element into it.
		//! @param		newsize		The new size of the pool, should be larger than number in the pool.
		//! @remark		After resize, the index of element maybe changed.
		void resize( dword newsize );

		//! Clear pool and delete all memory used by nodes.
		//! @param		freememory	If free the memory used by array.
		void clear( bool freememory = false );
		//! Clear pool and delete all memory used by nodes.
		//! @param		size		The maximal size of the pool in number of element.
		void clear( dword size );
	};

	//----------------------------------------------------------------------------
	// Pool Implementation
	//----------------------------------------------------------------------------

	template< typename Type, typename Key >
	Pool< Type, Key >::Pool( dword size )
	{
		mSize	= size;
		mNext	= 0;
		mNodes	= new PoolNode[ mSize ];
	}

	template< typename Type, typename Key >
	Pool< Type, Key >::Pool( const Pool< Type, Key >& pool )
	{
		mSize	= pool.mSize;
		mNext	= pool.mNext;
		mNodes	= new PoolNode[ mSize ];

		for ( Node* temp = pool.mHead; temp != null; temp = temp->mNext )
			insert( temp->mElement, ( (PoolNode*) temp )->mIndex );
	}

	template< typename Type, typename Key >
	Pool< Type, Key >::~Pool( )
	{
		delete[] mNodes;
	}

	template< typename Type, typename Key >
	Pool< Type, Key >& Pool< Type, Key >::operator = ( const Pool< Type, Key >& pool )
	{
		clear( );

		for ( Node* temp = pool.mHead; temp != null; temp = temp->mNext )
			insert( temp->mElement, ( (PoolNode*) temp )->mIndex );

		return *this;
	}

	template< typename Type, typename Key >
	const Type& Pool< Type, Key >::operator [] ( dword index ) const
	{
		Assert( index < mSize )
			Assert( mNodes[ index ].mIndex != -1 )

			return mNodes[ index ].mElement;
	}

	template< typename Type, typename Key >
	Type& Pool< Type, Key >::operator [] ( dword index )
	{
		Assert( index < mSize )
			Assert( mNodes[ index ].mIndex != -1 )

			return mNodes[ index ].mElement;
	}

	template< typename Type, typename Key >
	Type* Pool< Type, Key >::index_item( dword index )
	{
		if ( index >= mSize )
			return null;

		if ( mNodes[ index ].mIndex == -1 )
			return null;

		return &( mNodes[ index ].mElement );
	}

	template< typename Type, typename Key >
	typename Pool< Type, Key >::iterator Pool< Type, Key >::get_iterator( dword index )
	{
		if ( index >= mSize )
			return iterator( null );

		if ( mNodes[ index ].mIndex == -1 )
			return iterator( null );

		return iterator( &( mNodes[ index ] ) );
	}

	template< typename Type, typename Key >
	dword Pool< Type, Key >::max_size( )
	{
		return mSize;
	}

	template< typename Type, typename Key >
	dword Pool< Type, Key >::create( const Type& element )
	{
		if ( mNumber == mSize )
			return -1;

		// Find a empty node to place the new element.
		while ( mNodes[ mNext ].mIndex != -1 )
			mNext = ( mNext + 1 ) % mSize;

		insert( element, mNext );

		dword index = mNext;

		// Update next index.
		mNext = ( mNext + 1 ) % mSize;

		return index;
	}

	template< typename Type, typename Key >
	void Pool< Type, Key >::insert( const Type& element, dword index )
	{
		Assert( index < mSize )
			Assert( mNodes[ index ].mIndex == -1 )

			mNodes[ index ].mElement	= element;
		mNodes[ index ].mIndex		= index;

		Link< Type, Key >::push_back( &( mNodes[ index ] ) );
	}

	template< typename Type, typename Key >
	void Pool< Type, Key >::remove( iterator& iterator )
	{
		Assert( iterator.valid( ) )

			remove( ( (PoolNode*) iterator.mNode )->mIndex );

		( (PoolNode*) iterator.mNode )->mIndex = -1;

		iterator.mNode = null;
	}

	template< typename Type, typename Key >
	void Pool< Type, Key >::remove( dword index )
	{
		Assert( index < mSize )
			Assert( mNodes[ index ].mIndex != -1 )

			mNodes[ index ].mIndex = -1;

		Link< Type, Key >::remove( &( mNodes[ index ] ) );
	}

	template< typename Type, typename Key >
	void Pool< Type, Key >::resize( dword newsize )
	{
		PoolNode* newnodes = new PoolNode[ newsize ];

		Link< Type, Key >::clear( );

		dword oldsize = mSize;

		mNext = 0;
		mSize = newsize;

		delete[] mNodes;
		mNodes = newnodes;
	}

	template< typename Type, typename Key >
	void Pool< Type, Key >::clear( bool freememory )
	{
		mNext = 0;

		if ( freememory == true )
		{
			delete[] mNodes;

			mNodes = new PoolNode[ mSize ];
		}
		else
		{
			for ( Node* node = mHead; node != null; )
			{
				Node* temp = node->mNext;

				node->mPrev = null;
				node->mNext = null;

				( (PoolNode*) node )->mIndex = -1;

				node = temp;
			}
		}

		Link< Type, Key >::clear( );
	}

	template< typename Type, typename Key >
	void Pool< Type, Key >::clear( dword size )
	{
		delete[] mNodes;

		mSize	= size;
		mNext	= 0;
		mNodes	= new PoolNode[ mSize ];

		Link< Type, Key >::clear( );
	}

};