#pragma once


namespace NekoEngine
{
	class IInputDriver
	{
	public:
		virtual void		Tick( float elapse ) = 0;

		virtual bool		IsMouseDown(eMouseButton Button) = 0;

		virtual bool		IsKeyDown(eKeyButton Button) = 0;

		virtual bool		IsMouseUp(eMouseButton Button) = 0;

		virtual bool		IsKeyUp(eKeyButton Button) = 0;

		virtual Point		GetMouseOffset( ) = 0;

		virtual int			GetMouseScroller( ) = 0;

		virtual Point		GetMousePoint( ) = 0;
	};

}
