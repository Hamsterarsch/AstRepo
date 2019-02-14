#include "ASTPlayerController.h"
#include "ASTCheatManager.h"

//Public---------------

AASTPlayerController::AASTPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bAttachToPawn = true;
	PlayerCameraManagerClass = AASTCameraManager::StaticClass(); 

	CheatClass = UASTCheatManager::StaticClass();


}

void AASTPlayerController::EnableGlidingCamera()
{	
	if (auto *CameraManager = Cast<AASTCameraManager>(PlayerCameraManager))
	{
		InputPitchScale = m_GlidingPitchInputScale;
		InputYawScale = m_GlidingYawInputScale;
		CameraManager->UseGlidingViewAngles(ControlRotation);//lerping induces cmaera manager min/max-angle error every 2nd gliding attempt

		/*
		m_ControlRotationLerpTarget = ControlRotation;
		m_ControlRotationLerpTarget.Pitch = (CameraManager->m_GlidingViewPitchMin + CameraManager->m_GlidingViewPitchMax) *.5f;
		m_bControlLerpComplete = false;
		*/
		m_bGlidingCameraEnabled = true;
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

	InputPitchScale = m_StandardPitchInputScale;
	InputYawScale = m_StandardYawInputScale;


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

void AASTPlayerController::AddGlidingCameraYawInput(float Val)
{
	if (m_bGlidingCameraEnabled)
	{
		if (auto *CM = Cast<AASTCameraManager>(PlayerCameraManager))
		{
			CM->AddYawInput(Val);
			this->AddYawInput(Val);
			//UE_LOG(LogTemp, Log, TEXT("pc yaw add"));

		}
		//UE_LOG(LogTemp, Log, TEXT("pc glding cam input"));

	}


}

void AASTPlayerController::AddGlidingCameraPitchInput(float Val)
{
	if (m_bGlidingCameraEnabled)
	{
		if (auto *CM = Cast<AASTCameraManager>(PlayerCameraManager))
		{
			CM->AddPitchInput(Val);

		}

	}
	

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

void AASTPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_StandardPitchInputScale = InputPitchScale;
	m_StandardYawInputScale = InputYawScale;

}
