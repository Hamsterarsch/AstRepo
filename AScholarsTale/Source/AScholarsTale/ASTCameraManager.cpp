#include "ASTCameraManager.h"
#include "GameFramework/PlayerController.h"

//Public--------------------

void AASTCameraManager::AddYawInput(float Val)
{
	//Adjust angle borders
	ViewYawMax += Val;
	ViewYawMin += Val;


}

void AASTCameraManager::AddPitchInput(float Val)
{
	//Delta of input values
	auto InputDelta = m_LastPitchInput - Val;
	m_LastPitchInput = Val;
	
	//Delta weighted by speed
	auto AdjustedVal = InputDelta * m_GlidingLeanCameraSpeed;
	auto AngleSlice = m_GlidingMaxLeanAngle * AdjustedVal;
	
	auto NewLeanAngle = m_GlidingCurrentLeanAngle + AngleSlice;
	//If the angle after the update does not exceed the limit
	if (FMath::Abs(NewLeanAngle) < m_GlidingMaxLeanAngle)
	{
		if (PCOwner)
		{
			//Add an angle that matches the delta slice
			PCOwner->AddPitchInput(m_GlidingMaxLeanAngle * AdjustedVal);

		}

		//Set fov based on input. (Does not need delta because its not additive).
		SetFOV(DefaultFOV + 3.5 * Val);

		//Update to new angle
		m_GlidingCurrentLeanAngle = NewLeanAngle;

		//Adjust angle limits
		ViewPitchMax += m_GlidingMaxLeanAngle * AdjustedVal;
		ViewPitchMin += m_GlidingMaxLeanAngle * AdjustedVal;

	}

	
}

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
	//Reset pitch stats 
	ViewPitchMax = m_StandardViewPitchMax;
	ViewPitchMin = m_StandardViewPitchMin;

	//Reset yaw stats
	ViewYawMax = m_StandardViewYawMax;
	ViewYawMin = m_StandardViewYawMin;


}


//Protected-------------------

void AASTCameraManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Save standards
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
