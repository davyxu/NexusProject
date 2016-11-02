#pragma once

namespace NekoEngine
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Math
	*  @{
	*/

	class Quaternion;
	class Vector3;

	/** 
		旋转量	
	*/
	class Rotator
	{
	public:
		// Variables.
		int Pitch; // Looking up and down (0=Straight Ahead, +Up, -Down).
		int Yaw;   // Rotating around (running in circles), 0=East, +North, -South.
		int Roll;  // Rotation about axis of screen, 0=Straight, +Clockwise, -CCW.

		/// 构造函数
		Rotator() {}

		/// 赋值构造
		Rotator( int InPitch, int InYaw, int InRoll )
			:	Pitch(InPitch), Yaw(InYaw), Roll(InRoll) {}

		/// 拷贝构造
		Rotator( const Quaternion& Quat );

		/// 加操作
		Rotator operator+( const Rotator &R ) const
		{
			return Rotator( Pitch+R.Pitch, Yaw+R.Yaw, Roll+R.Roll );
		}

		/// 减操作
		Rotator operator-( const Rotator &R ) const
		{
			return Rotator( Pitch-R.Pitch, Yaw-R.Yaw, Roll-R.Roll );
		}

		/// 乘操作
		Rotator operator*( float Scale ) const
		{
			return Rotator( int(Pitch*Scale), int(Yaw*Scale), int(Roll*Scale) );
		}

		/// 自乘
		Rotator operator*= (float Scale)
		{
			Pitch = int(Pitch*Scale); Yaw = int(Yaw*Scale); Roll = int(Roll*Scale);
			return *this;
		}
		
		/// 相等判断
		bool operator==( const Rotator &R ) const
		{
			return Pitch==R.Pitch && Yaw==R.Yaw && Roll==R.Roll;
		}

		/// 不等判断
		bool operator!=( const Rotator &V ) const
		{
			return Pitch!=V.Pitch || Yaw!=V.Yaw || Roll!=V.Roll;
		}
		
		/// 自加
		Rotator operator+=( const Rotator &R )
		{
			Pitch += R.Pitch; Yaw += R.Yaw; Roll += R.Roll;
			return *this;
		}

		/// 自减
		Rotator operator-=( const Rotator &R )
		{
			Pitch -= R.Pitch; Yaw -= R.Yaw; Roll -= R.Roll;
			return *this;
		}
		
		// Functions.
		Rotator Reduce() const
		{
			return Rotator( MathTable::ReduceAngle(Pitch), MathTable::ReduceAngle(Yaw), MathTable::ReduceAngle(Roll) );
		}


		/// 是否为零
		int IsZero() const
		{
			return ((Pitch&65535)==0) && ((Yaw&65535)==0) && ((Roll&65535)==0);
		}

		/// 分量加
		Rotator Add( int DeltaPitch, int DeltaYaw, int DeltaRoll )
		{
			Yaw   += DeltaYaw;
			Pitch += DeltaPitch;
			Roll  += DeltaRoll;
			return *this;
		}

		Vector3 ToVector() const;

		Quaternion ToQuaternion() const;

		Vector3 ToEuler() const;

		Rotator& FromEuler(const Vector3& Euler);

		/// 约束分量在范围 -65535,65535
		Rotator Clamp() const
		{
			return Rotator( Pitch&65535, Yaw&65535, Roll&65535 );
		}

		Rotator ClampPos() const
		{
			return Rotator( Math::Abs(Pitch)&65535, Math::Abs(Yaw)&65535, Math::Abs(Roll)&65535 );
		}

		static int NormalizeAxis(int Angle)
		{
			Angle &= 0xFFFF;
			if( Angle > 32767 ) 
			{
				Angle -= 0x10000;
			}
			return Angle;
		}

		/** Normalize stuff from execNormalize so we can use it in C++ */
		Rotator Normalize() const
		{
			Rotator Rot = *this;
			Rot.Pitch = NormalizeAxis(Rot.Pitch);
			Rot.Roll = NormalizeAxis(Rot.Roll);
			Rot.Yaw = NormalizeAxis(Rot.Yaw);
			return Rot;
		}

		Rotator Denormalize() const
		{
			Rotator Rot = *this;
			Rot.Pitch	= Rot.Pitch & 0xFFFF;
			Rot.Yaw		= Rot.Yaw & 0xFFFF;
			Rot.Roll	= Rot.Roll & 0xFFFF;
			return Rot;
		}

		void MakeShortestRoute();

		void GetWindingAndRemainder(Rotator& Winding, Rotator& Remainder) const;
	};

	/** @} */
	/** @} */
}
