#pragma once

//----------------------------------------------------------------------------
// MaxNode
//----------------------------------------------------------------------------

class MaxNode
{
public:
	INode*	mNode;

	MaxNode( INode* node );

	char*	GetName( );
	char*	GetGroupName( );
	char*	GetParentName( );
	bool	IsLight( );
	bool	IsBone( );
	bool	IsDummy( );
	bool	IsMesh( );
	bool	IsRoot( );

	Matrix3	GetTransform( INode* node, int tick, bool noscale );
	Matrix3	GetTransform( int tick = 0, bool noscale = false );
	Matrix3 GetRelativeTransform( int tick = 0, bool noscale = false );
	bool IsVisible( );
	bool IsSelected( );
};


class MaxObject
{
public:

};