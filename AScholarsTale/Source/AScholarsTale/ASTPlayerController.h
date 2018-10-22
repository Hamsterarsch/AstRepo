#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "ASTCameraManager.h"

#include "ASTPlayerController.generated.h"


UCLASS(Blueprintable, Abstract, CustomConstructor)
class ASCHOLARSTALE_API AASTPlayerController : public APlayerController
{
	GENERATED_BODY()
		
public:
	AASTPlayerController();

	UFUNCTION(BlueprintCallable)
		void EnableGlidingCamera();

	UFUNCTION(BlueprintCallable)
		void DisableGlidingCamera();

	UFUNCTION(BlueprintCallable)
		void SetControlRotationRoll(float Roll);

	UFUNCTION(BlueprintCallable)
		void LerpControlRotationRoll(float TargetRoll, float LerpSpeed);


	UPROPERTY(BlueprintReadWrite, DisplayName="ControlRotationLerpTarget")
		FRotator m_ControlRotationLerpTarget{ EForceInit::ForceInitToZero };


protected:
	virtual void Tick(float DeltaTime) override;


private:
	float m_ControlRotationLerpSpeed{ 200 };
	bool	m_bControlLerpComplete{ true },
			m_bControlLerpLastTick{ false },
			m_bGlidingCameraEnabled{ false };
	
};
