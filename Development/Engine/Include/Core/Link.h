#pragma once

namespace NekoEngine
{
	template< typename Type, typename Key = Type > class Link
	{
	protected:

		class Node
		{
		public:
			Type	mElement;
			Node*	mPrev;
			Node*	mNext;

			Node( ) : mPrev( null ), mNext( null ) { }

			Node( const Type& element ) : mElement( element ), mPrev( null ), mNext( null ) { }

			operator Type& ( )
			{ return mElement; }
		};

	public:
		class iterator
		{
		public:
			Node*	mNode;
			iterator( ) : mNode( null ){ }

			iterator( Node* node ) : mNode( node ) { }

			operator Type& ( )
			{ 
				Assert(mNode);
				return mNode->mElement;
			}

			Type& operator*( )
			{
				Assert(mNode);
				return mNode->mElement;
			}

			void operator ++ ( )
			{ mNode = mNode != null ? mNode->mNext : null; }

			void operator ++ ( int )
			{ mNode = mNode != null ? mNode->mNext : null; }

			void operator -- ( int )
			{ mNode = mNode != null ? mNode->mPrev : null; }

			bool operator==(const iterator& it)
			{
				return mNode == it.mNode;
			}

			bool operator!=(const iterator& it)
			{
				return mNode != it.mNode;
			}

			bool valid( )
			{ return mNode != null; }
		};

	protected:
		Node*	mHead;
		Node*	mTail;
		int	mNumber;

		inline Link( );

		void push_front( Node* node );

		void push_back( Node* node );

		void insert_before( Node* node, iterator& iterator );

		void insert_after( Node* node, iterator& iterator );

		iterator remove( Node* node );

		void clear( );

		bool empty( );

	public:

		inline int size( );

		inline iterator begin( );

		inline iterator end( );

		inline iterator tail();

		iterator find( const Key& key );
	};

	//----------------------------------------------------------------------------
	// Link Implementation
	//----------------------------------------------------------------------------

	template< typename Type, typename Key >
	Link< Type, Key >::Link( ) : mHead( null ), mTail( null ), mNumber( 0 )
	{
	}

	template< typename Type, typename Key >
	void Link< Type, Key >::push_front( Node* node )
	{
		insert_before( node, iterator( mHead ) );
	}

	template< typename Type, typename Key >
	void Link< Type, Key >::push_back( Node* node )
	{
		insert_after( node, iterator( mTail ) );
	}

	template< typename Type, typename Key >
	void Link< Type, Key >::insert_before( Node* node, iterator& it )
	{
		Assert( node != null )

			if ( it.valid( ) == false )
			{
				mHead = mTail = node;
			}
			else
			{
				node->mPrev = it.mNode->mPrev;
				node->mNext = it.mNode;

				// Relink the previous one and next one both to this node.
				if ( node->mPrev != null )
					node->mPrev->mNext = node;
				if ( node->mNext != null )
					node->mNext->mPrev = node;

				// Adjuset head and tail pointer.
				if ( node->mPrev == null )
					mHead = node;
				if ( node->mNext == null )
					mTail = node;
			}

			// Increase the element number.
			mNumber ++;
	}

	template< typename Type, typename Key >
	void Link< Type, Key >::insert_after( Node* node, iterator& it )
	{
		Assert( node != null );

			if ( it.valid() == false )
			{
				mHead = mTail = node;
			}
			else
			{
				node->mPrev = it.mNode;
				node->mNext = it.mNode->mNext;

				// Relink the previous one and next one both to this node.
				if ( node->mPrev != null )
					node->mPrev->mNext = node;
				if ( node->mNext != null )
					node->mNext->mPrev = node;

				// Adjuset head and tail pointer.
				if ( node->mPrev == null )
					mHead = node;
				if ( node->mNext == null )
					mTail = node;
			}

			// Increase the element number.
			mNumber ++;
	}

	template< typename Type, typename Key >
	typename Link< Type, Key >::iterator Link< Type, Key >::remove( Node* node )
	{
		Assert( node != null );

			// Relink the previous one to next one of this node.
			if ( node->mPrev != null )
				node->mPrev->mNext = node->mNext;
		if ( node->mNext != null )
			node->mNext->mPrev = node->mPrev;

		// Adjuset head and tail pointer.
		if ( mHead == node )
			mHead = node->mNext;
		if ( mTail == node )
			mTail = node->mPrev;

		iterator it( node->mNext );

		// Remove the link from the node.
		node->mPrev = null;
		node->mNext = null;

		// Decrease the element number.
		mNumber --;

		return it;
	}

	template< typename Type, typename Key >
	void Link< Type, Key >::clear( )
	{
		mHead	= null;
		mTail	= null;
		mNumber	= 0;
	}

	template< typename Type, typename Key >
	bool Link< Type, Key >::empty( )
	{
		return mNumber == 0;
	}


	template< typename Type, typename Key >
	int Link< Type, Key >::size( )
	{
		return mNumber;
	}

	template< typename Type, typename Key >
	typename Link< Type, Key >::iterator Link< Type, Key >::begin( )
	{
		return iterator( mHead );
	}

	template< typename Type, typename Key >
	typename Link< Type, Key >::iterator Link< Type, Key >::end( )
	{
		return iterator( null );
	}

	template< typename Type, typename Key >
	typename Link< Type, Key >::iterator Link< Type, Key >::tail( )
	{
		return iterator( mTail );
	}

	template< typename Type, typename Key >
	typename Link< Type, Key >::iterator Link< Type, Key >::find( const Key& key )
	{
		for ( Node* temp = mHead; temp != null; temp = temp->mNext )
		{
			if ( temp->mElement == key )
				return iterator( temp );
		}

		return iterator( null );
	}
}
