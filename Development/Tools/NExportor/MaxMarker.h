#pragma once

//----------------------------------------------------------------------------
// MaxBone
//----------------------------------------------------------------------------

class MaxMarker : public MaxObject
{
public:
	INode*		mNode;

	MaxMarker( INode* node );

	virtual void	Serialize( );
};

typedef vector< MaxMarker* > MarkerArray;