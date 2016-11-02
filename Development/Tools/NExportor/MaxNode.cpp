#include "ExportPCH.h"
#include "PluginInclude.h"
//----------------------------------------------------------------------------
// MaxNode
//----------------------------------------------------------------------------

MaxNode::MaxNode( INode* node ) : mNode( node )
{
}

char* MaxNode::GetName( )
{
	return mNode->GetName( );
}

char* MaxNode::GetGroupName( )
{
	if ( mNode->IsGroupMember( ) )
		return mNode->GetParentNode( )->GetName( );

	return null;
}

char* MaxNode::GetParentName( )
{
	return mNode->GetParentNode( )->GetName( );
}

bool MaxNode::IsLight( )
{
	if ( mNode->Selected( ) == 0 )
		return false;

	Object* object = mNode->EvalWorldState( 0 ).obj;

	if ( object->ClassID( ) == Class_ID( OMNI_LIGHT_CLASS_ID, 0 ) )
		return true;

	if ( object->ClassID( ) == Class_ID( 0x0001b669, 0x000875c1 ) )
		return true;

	return false;
}

bool MaxNode::IsBone( )
{
	Object* object = mNode->EvalWorldState( 0 ).obj;
	if ( object == null )
		return false;

	if ( object->SuperClassID( ) == HELPER_CLASS_ID )
		return false;

	if ( object->ClassID( ) == BONE_OBJ_CLASSID )
		return true;

	Control* control = mNode->GetTMController( );

	if ( control->ClassID( ) == FOOTPRINT_CLASS_ID )
		return false;

	if ( control->ClassID( ) == BIPBODY_CONTROL_CLASS_ID )
		return true;

	if ( control->ClassID( ) == BIPSLAVE_CONTROL_CLASS_ID )
		return true;

	return false;
}

bool MaxNode::IsDummy( )
{
	if ( GMaxCollector->mExportSelected && mNode->Selected( ) == 0 )
		return false;

	if ( mNode->IsGroupHead( ) )
		return false;

	Object* object = mNode->EvalWorldState( 0 ).obj;

	if ( object->SuperClassID( ) != HELPER_CLASS_ID )
		return false;

	Control* control = mNode->GetTMController( );

	if ( object->ClassID( ) == BONE_OBJ_CLASSID )
		return false;

	if ( control->ClassID( ) == BIPSLAVE_CONTROL_CLASS_ID )
		return false;

	return true;
}

bool MaxNode::IsMesh( )
{
	if ( GMaxCollector->mExportSelected && mNode->Selected( ) == 0 )
		return false;

	Object* object = mNode->EvalWorldState( 0 ).obj;

	if ( object->CanConvertToType( triObjectClassID ) == 0 )
		return false;

	if ( object->ClassID( ) == BONE_OBJ_CLASSID )
		return false;

	Control* control = mNode->GetTMController( );

	if ( control->ClassID( ) == FOOTPRINT_CLASS_ID )
		return false;

	if ( control->ClassID( ) == BIPBODY_CONTROL_CLASS_ID )
		return false;

	if ( control->ClassID( ) == BIPSLAVE_CONTROL_CLASS_ID )
		return false;

	return true;
}

bool MaxNode::IsRoot( )
{
	return mNode->GetParentNode( )->IsRootNode( ) != 0;
}

bool MaxNode::IsVisible( )
{
	return !mNode->IsHidden( );
}

bool MaxNode::IsSelected( )
{
	return mNode->Selected() != 0 ;
}

Matrix3	MaxNode::GetTransform( INode* node, int tick, bool noscale )
{
	Matrix3 transform;

	if ( noscale )
	{
		transform = node->GetNodeTM( tick );
		transform.NoScale( );
	}
	else
	{
		transform = node->GetObjectTM( tick );
	}

	return transform;
}

Matrix3	MaxNode::GetTransform( int tick, bool noscale )
{
	return GetTransform( mNode, tick, noscale );
}

Matrix3 MaxNode::GetRelativeTransform( int tick, bool noscale )
{
	return GetTransform( mNode, tick, noscale ) * Inverse( GetTransform( mNode->GetParentNode( ), tick, noscale ) );
}