#pragma once

enum eAdjustAxis
{
	AAxis_None		= 0x00000000,
	AAxis_X			= 0x00000001,
	AAxis_Y			= 0x00000010,
	AAxis_XY		= 0x00000011,
	AAxis_Z			= 0x00000100,
	AAxis_XZ		= 0x00000101,
	AAxis_YZ		= 0x00000110,
	AScaleMask		= 0x00001000,
	AAxisBox_X		= 0x00001001,
	AAxisBox_Y		= 0x00001010,
	ATriangle_XY	= 0x00001011,
	AAxisBox_Z		= 0x00001100,
	ATriangle_XZ	= 0x00001101,
	ATriangle_YZ	= 0x00001110,
	ATriBox_XYZ		= 0x00001111,
	ARotateMask		= 0x00010000,
	ARing_X			= 0x00011001,
	ARing_Y			= 0x00011010,
	ARing_XY		= 0x00011011,
	ARing_Z			= 0x00011100,
	ARing_XZ		= 0x00011101,
	ARing_YZ		= 0x00011110,
	Adust_Max,
};


enum eECAdjustMode
{
	AdjustMode_None,
	AdjustMode_Position,
	AdjustMode_Scale,
	AdjustMode_Rotation,
};

enum eECAdjustSpace
{
	AdjustSpace_Local,
	AdjustSpace_World,
};




typedef std::vector<Actor*> SelectedActorArray;
