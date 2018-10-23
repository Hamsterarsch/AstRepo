#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ASTCameraManager.generated.h"

UCLASS()
class ASCHOLARSTALE_API AASTCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
		
public:
	void UseGlidingViewAngles(const FRotator &ControlRotation);

	void RestoreStandardViewAngles();


	UPROPERTY(EditAnywhere)
		float m_GlidingViewYawAngle{ 90 };

	UPROPERTY(EditAnywhere)
		float m_GlidingViewPitchMax{ 33 };

	UPROPERTY(EditAnywhere)
		float m_GlidingViewPitchMin{ -66 };

	UPROPERTY(EditAnywhere)
		float m_GlidingLeanAngle{ 15 };

	UPROPERTY(EditAnywhere)
		float m_GlidingTurnCameraRollAngle{ 15 };

	void AddYawInput(float Val);

	void AddPitchInput(float Val);

protected:
	virtual void PostInitializeComponents() override;


	float	m_StandardViewYawMax,
			m_StandardViewYawMin,
			m_StandardViewPitchMax,
			m_StandardViewPitchMin;
		

};
