#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Model
	*  @{
	*/

	class SkeletonInstance;

	/** 
		骨骼动画控制器
	*/
	class SkeletonAnimController
	{
	public:
		/**
			构造函数
			@param
				AnimResource 骨骼动画资源
			@param
				Skeleton 骨骼
			@remark
				资源将被引用
		*/
		SkeletonAnimController(SkeletonAnimResource* AnimResource, SkeletonInstance* Skeleton);

		/// 根据动画更新骨骼
		void	Update( float elapse );

		/// 动画长度
		float	GetDuration( );

		/// 获取动画播放位置
		float	GetCurrent( );

		/// 设置动画播放位置
		void	SetCurrent( float current, bool autoupdate = true );

		/// 获得动画播放速度
		float	GetSpeed( );

		/// 设置动画播放速度,默认为1
		void	SetSpeed( float speed );

		/// 获取动画是否可以循环
		bool	GetLoop( );

		/// 设置动画是否可以循环
		void	SetLoop( bool circle );

	private:
		void	SetBlendOption( bool affecttransform, bool fixroot );

		void	InitDeltaTransform( );

		Matrix4 GetDeltaTransform( );


		void	Update(float last,float current,float weight = 1.0f);

		SkeletonAnimResource*			mAnimationLoader;
		SkeletonInstance*				mSkeleton;

		float					mCurrent;
		float					mSpeed;
		bool					mCircle;
		bool					mAffectTransform;
		bool					mFixRoot;

		Vector3					mLastTranslation;
		Quaternion				mLastRotation;

		Vector3					mAccumTranslation;
		Quaternion				mAccumRotation;
		Vector3					mDeltaTranslation;
		Quaternion				mDeltaRotation;
	};
}
