#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Render
	*  @{
	*/


	class GeometryComponent;

	/** 
		渲染队列分组
	*/
	enum eRenderQueueGroup
	{
		RQF_Background = 0,		///< 背景
		RQF_SkiesEarly = 5,		///< 低于天空
		RQF_Default= 50,		///< 默认
		RQF_SkiesLater = 95,	///< 高于天空
		RQF_Overlay = 100,		///< 界面
	};

	/** 
		渲染队列分组
	*/
	class RenderQueueGroup
	{
	public:
		/** 
			构造函数
			@param
				GroupID 分组ID
		*/
		RenderQueueGroup( dword GroupID );

		/** 
			添加一个可绘制对象
		*/
		void Add( GeometryComponent* Component );

		/** 
			绘制这个组内的对象
		*/
		void Draw( SceneView* RV );

		/** 
			清空组内对象
		*/
		void Clear();

		/** 
			获取组ID
		*/
		dword GetGroupID( ){ return mGroupID; }

	private:
		typedef std::vector<GeometryComponent*> RenderList;

		void DrawRenderList( SceneView* RV, RenderList& RList );
		
		dword		mGroupID;

		RenderList	mOpaqueRenderList;
		RenderList	mTransparentRenderList;
		RenderList	mSolidRenderList;
	};

	/** 
		渲染队列
	*/
	class RenderQueue
	{
	public:
		RenderQueue( );

		~RenderQueue( );

		/** 
			添加一个可绘制对象
		*/
		void Add( GeometryComponent* Comp );

		/** 
			清空组内对象
		*/
		void Clear( );

		/** 
			绘制对象
		*/
		void Draw( SceneView* RV );

	private:
		RenderQueueGroup* GetRenderGroup( dword GroupID );
		
	private:
		typedef std::map<dword, RenderQueueGroup*> RenderGroupMap;
		RenderGroupMap mRenderGroupMap;
		typedef std::vector<RenderQueueGroup*> RenderGroupArray;
		RenderGroupArray mRenderGroupArray;

		bool mNeedUpdateGroupOrder;

	};

	/** @} */
	/** @} */
}
