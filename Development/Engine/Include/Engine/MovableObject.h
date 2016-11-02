#pragma once

namespace NekoEngine
{
	/** \addtogroup Engine
	*  @{
	*/
	/** \addtogroup System
	*  @{
	*/


	/// 变换空间
	enum eTransformSpace
	{
		TS_Local,		///< 对象本地
		TS_Parent,		///< 相对父级
		TS_World,		///< 世界
	};

	/// 变换类型
	enum eTransformType
	{
		TT_None				= 0,		///< 没有
		TT_Position			= 1,		///< 位置
		TT_Rotation			= 2,		///< 旋转
		TT_Scale			= 4,		///< 缩放
		TT_InheritRotate	= 8,		///< 派生旋转
		TT_InheritScale		= 16,		///< 派生缩放
		TT_All = TT_Position + TT_Rotation + TT_Scale + TT_InheritScale + TT_InheritRotate,
	};

	/** 
		可变换对象
		@remark
			封装对象矩阵变换
	*/
	class MovableObject
	{
	public:
		MovableObject( );

		~MovableObject( );

		//////////////////////////////////////////////////////////////////////////
		// 变换
		//////////////////////////////////////////////////////////////////////////

		/**
			位移累加
			@param
				Value 累加量
			@param
				Space 变换空间
		*/
		void				Translate( const Vector3& Value, eTransformSpace Space = TS_Parent );

		/**
			旋转累加
			@param
				Value 累加量
			@param
				Space 变换空间
		*/
		void				Rotate( const Quaternion& Value, eTransformSpace Space = TS_Parent );

		/**
			缩放累加
			@param
				Value 累加量
			@param
				Space 变换空间
		*/
		void				Scale( const Vector3& Value );

		/// 设置相对父级位置
		void				SetPosition( const Vector3& Value );

		/// 获取相对父级位置
		const Vector3&		GetPosition( );

		/// 设置相对父级旋转
		void				SetRotation( const Quaternion& Value );

		/// 获取相对父级旋转
		const Quaternion&	GetRotation( );

		/// 设置相对父级缩放
		void				SetScale( const Vector3& Value );

		/// 获取相对父级缩放
		const Vector3&		GetScale( );

		/// 设置是否继承缩放
		void				SetInheriteScale( bool IsInherite );

		/// 获取是否继承缩放
		bool				GetInheriteScale( );

		/// 设置是否继承旋转
		void				SetInheriteRotate( bool IsInherite );

		/// 获取是否继承旋转
		bool				GetInheriteRotate( );

		/// 重置旋转
		void				ResetRotation( );

		/// 合并了父级所有变换的变换
		const Matrix4&		GetFullTransform( );

		/// 获取派生旋转
		const Quaternion&	GetDerivedRotation( );

		/// 获取派生位置
		const Vector3&		GetDerivedPosition( );

		/// 获取派生缩放
		const Vector3&		GetDerivedScale( );

		/// 设置派生位置
		void				SetDerivedPosition( const Vector3& Value );

		/// 设置派生旋转
		void				SetDerivedRotation( const Quaternion& Value );

		/// 设置派生缩放
		void				SetDerivedScale( const Vector3& Value );

		/// 世界坐标转局部位置
		Vector3				WorldToLocalPosition( const Vector3 &WorldPos );

		/// 局部坐标赚世界位置
		Vector3				LocalToWorldPosition( const Vector3 &LocalPos );

		/// 世界坐标转局部旋转
		Quaternion			WorldToLocalRotation( const Quaternion &WorldRotation );

		/// 局部坐标赚世界旋转
		Quaternion			LocalToWorldRotation( const Quaternion &LocalRotation );		

		/**
			面向指定的一点
			@param
				Value 面向的点
			@param
				UpFixed 是否保持上方向不变(不绕物体正前方的X方向旋转),摄像机模式需要选true
		*/
		void SetViewTarget( const Vector3& Value, bool UpFixed = true );

		/// 获取朝向
		Vector3 GetViewDir( );

		void Serialize( BinarySerializer& Ser );

	protected:
		/// 需要实现:获取父类接口
		virtual MovableObject*	GetParentMovable( ){ return null; }

		/// 需要实现:遍历第一个子类
		virtual MovableObject* GetFirstMovable( ){ return null; }

		/// 需要实现:遍历下一个子类
		virtual MovableObject* GetNextMovable( ){ return null; }

		/// 需要实现: 变换后通知
		virtual void OnObjectTransformed( dword TransformType ){ }

		void NeedUpdateTransform( dword TransformType );

		void UpdateFromParent( );
	
	private:
		// 相对于父级的变换
		Vector3		mPosition;
		Quaternion	mRotation;

		// 自身缩放
		Vector3		mScale;

		// 决定自己是否派生自父级的旋转
		bool		mInheritRotate;

		// 决定自己是否派生自父级缩放
		bool		mInheritScale;

		// 合并了父级所有变换的变量缓冲
		Quaternion	mDerivedRotation;
		Vector3		mDerivedPosition;
		Vector3		mDerivedScale;

		dword		mNeedParentUpdateType;

		bool		mNeedRebuildMatrix;
		Matrix4		mFullTransform;

		bool		mNotifyTransform;
	};

	/** @} */
	/** @} */
}