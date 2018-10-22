#include "ASTCameraManager.h"
#include "GameFramework/PlayerController.h"

//Public--------------------

void AASTCameraManager::UseGlidingViewAngles(const FRotator &ControlRotation)
{
	//Make new yaw borders relative to control rotation
	ViewYawMax = ControlRotation.Yaw + m_GlidingViewYawAngle * .5f;
	ViewYawMin = ControlRotation.Yaw + m_GlidingViewYawAngle * -.5f;

	//No relativization needed because ws pitch stays unnoticed
	ViewPitchMax = m_GlidingViewPitchMax;
	ViewPitchMin = m_GlidingViewPitchMin;
	

}

void AASTCameraManager::RestoreStandardViewAngles()
{
	ViewPitchMax = m_StandardViewPitchMax;
	ViewPitchMin = m_StandardViewPitchMin;

	ViewYawMax = m_StandardViewYawMax;
	ViewYawMin = m_StandardViewYawMin;


}


//Protected-------------------

void AASTCameraManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_StandardViewPitchMax = ViewPitchMax;
	m_StandardViewPitchMin = ViewPitchMin;

	m_StandardViewYawMax = ViewYawMax;
	m_StandardViewYawMin = ViewYawMin;


}

/*
void AASTCameraManager::LimitViewPitch(FRotator &ViewRotation, float InViewPitchMin, float InViewPitchMax)
{


}

void AASTCameraManager::LimitViewYaw(FRotator &ViewRotation, float InViewPitchMin, float InViewPitchMax)
{

}

void AASTCameraManager::LimitViewRoll(FRotator &ViewRotation, float InViewPitchMin, float InViewPitchMax)
{

}
*/
