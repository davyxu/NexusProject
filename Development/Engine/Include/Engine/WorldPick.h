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
		世界拾取器
	*/
	class WorldPick
	{
	public:
		/**
			构造函数
			@param
				World 需要拾取的世界	
		*/
		WorldPick( GameWorld* World );

		/**
			拾取
			@param
				PickRay 光线
			@param
				SortResult 按照从近到远的顺序排序
			@return
				是否拾取到
		*/
		bool Pick( const Ray& PickRay, bool SortResult = false );

		/** 
			在一个6面类视锥体中拾取
		*/
		bool Pick( const PlaneBoundedVolume& BoundedVolume );


		/**
			获得拾取结果
			@param
				Index 索引
			@return
				拾取到的结果
		*/
		Actor* GetActor( dword Index );

		/// 获取拾取数量
		dword GetCount( );

		/// 获取拾取对象的距离
		float GetDistance( dword Index );

		struct PickResult 
		{
			Actor*	mActor;
			float	mDistance;
		};

	private:
		void PickActor( const Ray& PickRay, Actor* A );

		void PickActor( const PlaneBoundedVolume& BoundedVolume, Actor* A );

	private:

		typedef std::vector<PickResult> PickResultArray;

		PickResultArray mPickResult;

		GameWorld* mWorld;
		
	};

	/** @} */
	/** @} */
}
