#pragma once


namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Platform
	*  @{
	*/


	struct TimeData
	{
		word mYear;
		word mMonth;
		word mDayOfWeek;
		word mDay;
		word mHour;
		word mMinute;
		word mSecond;
		word mMilliSeconds;
	};

	/// 时间源
	class TimeSource
	{
	public:
		/// 初始化
		static void	StaticInit( );

		/// 更新
		static void	Tick( );

		/// 获得上次渲染花费时间(ms)
		static float GetElapsedTime( );

		/// 取得自初始化到现在的时间(ms)
		static float GetAppTime( );

		/// 取得 系统时间
		static void	GetLocalTime(TimeData& Data );

		/// 取windows tick time
		static dword GetTickTime( );
	};

	/// 自动计时计算间隔
	class TimeRuler
	{
	public:
		/// 开始计时
		TimeRuler( );

		/// 获得自开始到现在花费的时间
		float GetCost( );

		/// 手动开始计时
		void Start( );

	private:
		float mStartTick;
	};

	/** @} */
	/** @} */
}