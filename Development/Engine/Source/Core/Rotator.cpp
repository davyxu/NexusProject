#include "CorePCH.h"
#include "NekoCore_Math.h"

namespace NekoEngine
{
	/*-----------------------------------------------------------------------------
		FRotator functions.
	-----------------------------------------------------------------------------*/

	Rotator::Rotator(const Quaternion& Quat)
	{
		*this = Matrix4().FromTranslationRotation(  Vector3::cZero, Quat ).GetRotator();
	}

	//
	// Convert a rotation into a vector facing in its direction.
	//
	Vector3 Rotator::ToVector() const
	{
		return Matrix4().FromRotator( *this ).GetAxis(0);
	}

	//
	// Convert a rotation into a quaternion.
	//

	Quaternion Rotator::ToQuaternion() const
	{
		return Quaternion().FromMatrix( Matrix4().FromRotator( *this ) );
	}

	// Convert a Rotator into floating-point Euler angles (in degrees).
	Vector3 Rotator::ToEuler() const
	{
		return Vector3( Roll * (180.f / 32768.f), Pitch * (180.f / 32768.f), Yaw * (180.f / 32768.f) );
	}

	// Convert a vector of floating-point Euler angles (in degrees) into a Rotator. 
	Rotator& Rotator::FromEuler(const Vector3& Euler)
	{
		Pitch	= int(Euler.y * (32768.f / 180.f));
		Yaw		= int(Euler.z * (32768.f / 180.f));
		Roll	= int(Euler.x * (32768.f / 180.f));

		return *this;
	}



	 
	//Decompose this Rotator into a Winding part (multiples of 65536) and a Remainder part. 
	//Remainder will always be in -32768 -> 32768 range.
	
	void Rotator::GetWindingAndRemainder(Rotator& Winding, Rotator& Remainder) const
	{
		//// YAW
		Remainder.Yaw = Yaw & 65535;
		Winding.Yaw = Yaw - Remainder.Yaw;

		if(Remainder.Yaw > 32768)
		{
			Winding.Yaw += 65536;
			Remainder.Yaw -= 65536;
		}
		else if(Remainder.Yaw < -32768)
		{
			Winding.Yaw -= 65536;
			Remainder.Yaw += 65536;
		}

		//// PITCH
		Remainder.Pitch = Pitch & 65535;
		Winding.Pitch = Pitch - Remainder.Pitch;

		if(Remainder.Pitch > 32768)
		{
			Winding.Pitch += 65536;
			Remainder.Pitch -= 65536;
		}
		else if(Remainder.Pitch < -32768)
		{
			Winding.Pitch -= 65536;
			Remainder.Pitch += 65536;
		}

		//// ROLL
		Remainder.Roll = Roll & 65535;
		Winding.Roll = Roll - Remainder.Roll;

		if(Remainder.Roll > 32768)
		{
			Winding.Roll += 65536;
			Remainder.Roll -= 65536;
		}
		else if(Remainder.Roll < -32768)
		{
			Winding.Roll -= 65536;
			Remainder.Roll += 65536;
		}
	}

	 
	//Alter this Rotator to form the 'shortest' rotation that will have the effect. 
	//First clips the rotation between +/- 65535, then checks direction for each component.
	//
	void Rotator::MakeShortestRoute()
	{
		//// YAW

		// Clip rotation to +/- 65535 range
		Yaw = Yaw & 65535;

		// Then ensure it takes the 'shortest' route.
		if(Yaw > 32768)
			Yaw -= 65535;
		else if(Yaw < -32768)
			Yaw += 65535;


		//// PITCH

		Pitch = Pitch & 65535;

		if(Pitch > 32768)
			Pitch -= 65535;
		else if(Pitch < -32768)
			Pitch += 65535;


		//// ROLL

		Roll = Roll & 65535;

		if(Roll > 32768)
			Roll -= 65535;
		else if(Roll < -32768)
			Roll += 65535;
	}


}