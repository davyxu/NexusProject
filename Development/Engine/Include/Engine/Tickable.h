#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/


	/** 
		可更新对象
		@remark
			派生就可自动更新
	*/
	class Tickable
	{
	public:
		Tickable( );

		virtual ~Tickable( );

		virtual void Tick( float Elapse ) = 0;

		virtual bool IsTickable( ){ return true; }

		static void StaticTick( float Elapse );

	public:
		typedef std::list<Tickable*> TickableList;
		static TickableList mTickableList;
	};

	/** @} */
	/** @} */
}
