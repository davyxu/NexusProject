#include "CorePCH.h"
#include "NekoCore_Reflection.h"

namespace NekoEngine
{
	class_register::class_register( )
		: m_header( null )
	{

	}

	void class_register::free_list( classdata_base* header )
	{
		classdata_base* p = header;
		while ( p )
		{
			classdata_base* b = p;

			p = b->m_next;

			delete b;
		}
	}

	class_register::~class_register( )
	{
		free_list( m_header ); m_header = null;
	}

	void class_register::register_class( dword type, const char* name, pfn_classcreate func )
	{
		classdata_base* ins = new classdata_base;
		strcpy_s(ins->m_name, name );
		ins->m_pfn_create = func;
		ins->m_next = null;
		ins->m_type = type;
		if ( m_header == null )
		{
			m_header = ins;
		}
		else
		{
			ins->m_next = m_header;
			m_header = ins;
		}
	}


	void* class_register::create_instance( const char* name )
	{
		classdata_base* p = m_header;
		while ( p )
		{
			if ( !strcmp( p->m_name, name ) )
				return p->m_pfn_create();

			p = (classdata_base*)p->m_next;
		}

		return null;
	}

	bool class_register::exist( const char* name )
	{
		classdata_base* p = m_header;
		while ( p )
		{
			if ( !strcmp( p->m_name, name ) )
				return true;

			p = (classdata_base*)p->m_next;
		}

		return false;
	}

	void* class_register::create_instance( dword type )
	{
		classdata_base* p = m_header;
		while ( p )
		{
			if ( type == p->m_type )
				return p->m_pfn_create();

			p = (classdata_base*)p->m_next;
		}

		return null;
	}

	bool class_register::exist( dword type )
	{
		classdata_base* p = m_header;
		while ( p )
		{
			if ( type == p->m_type )
				return true;

			p = (classdata_base*)p->m_next;
		}

		return false;
	}
}