#include "ASTPlayerController.h"


//Public---------------

AASTPlayerController::AASTPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bAttachToPawn = true;
	PlayerCameraManagerClass = AASTCameraManager::StaticClass(); 


}

void AASTPlayerController::EnableGlidingCamera()
{
	
	if (auto *CameraManager = Cast<AASTCameraManager>(PlayerCameraManager))
	{
		CameraManager->UseGlidingViewAngles(ControlRotation);//lerping induces cmaera manager min/max-angle error every 2nd gliding attempt

		/*
		m_ControlRotationLerpTarget = ControlRotation;
		m_ControlRotationLerpTarget.Pitch = (CameraManager->m_GlidingViewPitchMin + CameraManager->m_GlidingViewPitchMax) *.5f;
		m_bControlLerpComplete = false;
		m_bGlidingCameraEnabled = true;
		*/
	}
	
	
	
	

	
}

void AASTPlayerController::DisableGlidingCamera()
{
	if (auto *CameraManager = Cast<AASTCameraManager>(PlayerCameraManager))
	{
		CameraManager->RestoreStandardViewAngles();

	}
	m_bGlidingCameraEnabled = false;
	m_ControlRotationLerpTarget = ControlRotation;


}

void AASTPlayerController::SetControlRotationRoll(const float Roll)
{
	ControlRotation.Roll += Roll;
	

}

void AASTPlayerController::LerpControlRotationRoll(const float TargetRoll, const float LerpSpeed)
{
	m_ControlRotationLerpTarget = ControlRotation;
	m_ControlRotationLerpTarget.Roll = TargetRoll;

	m_ControlRotationLerpSpeed = LerpSpeed;
	m_bControlLerpComplete = false;


}


//Protected---------------

void AASTPlayerController::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the control lerp target and control rotation differ...
	if ( !m_bControlLerpComplete )
	{
		ControlRotation = FMath::RInterpConstantTo(ControlRotation, m_ControlRotationLerpTarget, DeltaTime, m_ControlRotationLerpSpeed);
		m_bControlLerpComplete = ControlRotation == m_ControlRotationLerpTarget;
		m_bControlLerpLastTick = true;

	}
	//If this tick is a tick after a lerp completion...
	else if(m_bControlLerpLastTick && m_bControlLerpComplete)
	{
		//... and if that was performed due to a change to gliding camera...
		if (m_bGlidingCameraEnabled)
		{
			//...set gliding angles
			if (auto *CameraManager = Cast<AASTCameraManager>(PlayerCameraManager))
			{
				CameraManager->UseGlidingViewAngles(ControlRotation);
			
			}
		
		}
		//...set correct state
		m_bControlLerpLastTick = false;

	}



}

