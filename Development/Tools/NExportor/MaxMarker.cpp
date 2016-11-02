#include "ExportPCH.h"
#include "PluginInclude.h"
//----------------------------------------------------------------------------
// MaxMarker
//----------------------------------------------------------------------------

MaxMarker::MaxMarker( INode* node ) : mNode( node )
{
}


void MaxMarker::Serialize( )
{
	//int maxsize = sizeof( int ) + sizeof( float ) * 14 + 1024;

	//Matrix3	relativetransform	= MaxNode( mNode ).GetRelativeTransform( 0, true );
	//Point3	relativetranslation	= relativetransform.GetTrans( );
	//Quat	relativerotation	= Quat( relativetransform );

	//relativetranslation.x	= - relativetranslation.x;
	//relativerotation.x		= - relativerotation.x;
	//relativerotation.w		= - relativerotation.w;

	//// Prepare Buffer
	//mBuffer = new char[ maxsize ];

	//char* pointer = mBuffer, buffer1[ 1024 ]; wchar_t buffer2[ 1024 ];

	//string info = GetName( );
	//::MultiByteToWideChar( CP_ACP, 0, info.data( ), (int) info.length( ) + 1, buffer2, 1024 );

	//// Marker Name Length
	//*( (int*) pointer ) = (int) ::wcslen( buffer2 );
	//pointer += sizeof( int );

	//// Marker Name
	//::wcscpy( (wchar_t*) pointer, buffer2 );
	//pointer += sizeof( wchar_t ) * ( (int) ::wcslen( buffer2 ) + 1 );
	//info += "\r\n";

	//// Parent ID
	//*( (int*) pointer ) = -1;
	//pointer += sizeof( int );

	//// Marker Translation
	//*( (Point3*) pointer ) = relativetranslation;
	//pointer += sizeof( Point3 );

	//::sprintf( buffer1, "Translation(%3.2f, %3.2f, %3.2f)\r\n", relativetranslation.x, relativetranslation.y, relativetranslation.z );
	//info += buffer1;

	//// Marker Rotation
	//*( (Quat*) pointer ) = relativerotation;
	//pointer += sizeof( Quat );

	//::sprintf( buffer1, "Rotation(%3.2f, %3.2f, %3.2f, %3.2f)\r\n", relativerotation.x, relativerotation.y, relativerotation.z, relativerotation.w );
	//info += buffer1;

	//// Chunk Size
	//mSize = pointer - mBuffer;

	//// Information
	//mInformation = new char[ info.length( ) + 1 ];
	//::strcpy( mInformation, info.data( ) );
}

