#pragma once

//		  AxisZ(Blue)
//		   ^ 
//		   |
//		   |---- ScaleXZ
//		   |   |
//		   -------> AxisX(Red)
//		  /
//		 /
//      AxisY(Green)



class ActorFactoryGizmo: public ActorFactory
{
	DECLARE_RTTI_CLASS( ActorFactoryGizmo, ActorFactory, ClassFlag_Intrinsic )

public:
	ActorFactoryGizmo( );

	virtual ~ActorFactoryGizmo( );
	virtual Actor* CreateActor( const WString& Name, void* Parameter  );

	Actor* GetGizmoPart( eAdjustAxis Part );

	void SetAdjustMode( eECAdjustMode Mode );

	void SetHighLightGizmoPart( eAdjustAxis Part, bool IsHightLight );

	eAdjustAxis CheckGizmoPart( Actor* A );

	Actor* CreatePart( const wchar* Name, eAdjustAxis Part, Actor* Parent );
private:
	void PrepareMaterial( );

	MaterialInstance* GetMaterialByPart( eAdjustAxis Part, bool IsHighLight );

	MaterialInstancePtr GenerateMaterial( const wchar* MaterialName, MaterialDescriptor* Mat, const Color& C );

private:	
	typedef std::map<eAdjustAxis, Actor*> GizmoActorMap;
	GizmoActorMap mGizmoActorMap;

	MaterialDescriptorPtr mConstantColorMat; 

	MaterialInstancePtr mMIRed;
	MaterialInstancePtr mMIGreen;
	MaterialInstancePtr mMIBlue;
	MaterialInstancePtr mMIYellow;

	MaterialInstancePtr mMIRedHL;
	MaterialInstancePtr mMIGreenHL;
	MaterialInstancePtr mMIBlueHL;
	MaterialInstancePtr mMIYellowHL;
};
