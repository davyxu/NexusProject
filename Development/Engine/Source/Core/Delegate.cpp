#include "CorePCH.h"
#include "NekoCore_Reflection.h"

namespace NekoEngine
{
	event_delegate::event_delegate()
		: m_callback( null )
		, m_buffer( null )
		, m_size( 0 )
	{

	}

	event_delegate::event_delegate( const event_delegate& other )
	{
		copy( other );
	}
	event_delegate::~event_delegate()
	{
		clear();
	}

	void event_delegate::rawbind( c_callback dispatcher, void* func, int sizeoffunc, const void* userdata, int sizeofud )
	{
		m_callback	= dispatcher;
		m_size = sizeoffunc + sizeofud;
		m_buffer = new char[m_size];
		::memcpy( m_buffer, (void*)func, sizeoffunc );
		::memcpy( (char*)m_buffer + sizeoffunc, &userdata, sizeofud );
	}



	void event_delegate::invoke( handle sender, handle args )
	{
		if ( valid() )
			m_callback( m_buffer, sender, args );
	}

	void event_delegate::clear()
	{
		SafeDeleteArray( m_buffer );
		m_callback = null;
	}

	event_delegate& event_delegate::operator = ( const event_delegate& other )
	{
		return copy( other );
	}

	event_delegate& event_delegate::copy( const event_delegate& other )
	{
		if ( other.m_size == 0 || other.m_buffer == null )
			return *this;

		m_callback = other.m_callback;
		m_size = other.m_size;
		m_buffer = new char[ other.m_size ];
		::memcpy( m_buffer, other.m_buffer, m_size );

		return *this;
	}

	bool event_delegate::valid()
	{
		return m_callback != null && m_size != null && m_buffer != null;
	}

}