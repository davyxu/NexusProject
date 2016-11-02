#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup Model
	*  @{
	*/

	/** 
		骨骼动画关键帧
	*/
	struct SkeletonAnimKeyframe
	{
		// 时间点
		float		mTime;

		// 位移
		Vector3		mTranslation;

		// 旋转
		Quaternion	mRotation;

		friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, SkeletonAnimKeyframe*& Data )
		{
			if ( Ser.IsLoading() )
			{
				Data = new SkeletonAnimKeyframe;
			}

			Ser << Data->mTime
				<< Data->mTranslation
				<< Data->mRotation;

			return Ser;
		}
	};

	typedef std::vector< SkeletonAnimKeyframe* > SkeletonAnimKeyframeArray;

	/** 
		骨骼动画轨道,对应骨头
	*/
	struct SkeletonAnimTrack
	{
		// 骨头
		int							mBoneID;

		// 关键帧数组
		SkeletonAnimKeyframeArray	mKeyframeArray;

		virtual ~SkeletonAnimTrack( );

		friend BinarySerializer& operator <<( NekoEngine::BinarySerializer& Ser, SkeletonAnimTrack*& Data )
		{
			if ( Ser.IsLoading() )
			{
				Data = new SkeletonAnimTrack;
			}

			Ser << Data->mBoneID
				<< Data->mKeyframeArray;

			return Ser;
		}
	};

	
	/** 
		骨骼动画资源
	*/
	class SkeletonAnimResource : public ManagedObject
	{
		DECLARE_RTTI_CLASS( SkeletonAnimResource, ManagedObject, ClassFlag_Intrinsic| ClassFlag_Serializable )
	public:
		SkeletonAnimResource();
		virtual ~SkeletonAnimResource( );
		
		/// 设置动画长度,插件保存时用
		void SetDuration( float Duration ){ mDuration = Duration; }

		/// 获得动画长度
		float GetDuration( ){ return mDuration; }

		/// 获得轨道数量(对应骨头数)
		dword GetTrackCount( ){ return mTrackArray.size(); }

		/// 添加轨道
		dword AddTrack( SkeletonAnimTrack* Track );

		virtual void Serialize( BinarySerializer& Ser );		

	private:
		friend class SkeletonAnimController;


		typedef std::vector< SkeletonAnimTrack* > SkAnimTrackArray;


		SkAnimTrackArray	mTrackArray;
		float		mDuration;

		void GetKeyframe( SkeletonAnimTrack* track, float time, SkeletonAnimKeyframe& frame1, SkeletonAnimKeyframe& frame2 );
		void SetKeyframe( SkeletonAnimTrack* track, float time, Vector3& translation, Quaternion& rotation );
	};

	typedef TRefPtr<SkeletonAnimResource> SkeletonAnimResourcePtr;
}
