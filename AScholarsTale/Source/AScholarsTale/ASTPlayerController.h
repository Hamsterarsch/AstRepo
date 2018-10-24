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
	//Ctor.
	//Configures CameraManager aggregate class.
	AASTPlayerController();

	UFUNCTION(BlueprintCallable)
		//unstable
		void EnableGlidingCamera();

	UFUNCTION(BlueprintCallable)
		//unstable
		void DisableGlidingCamera();

	UFUNCTION(BlueprintCallable)
		//unstable
		void SetControlRotationRoll(float Roll);

	UFUNCTION(BlueprintCallable)
		//unstable
		void LerpControlRotationRoll(float TargetRoll, float LerpSpeed);

	//Used during gliding movement.
	//Yaws the camera while accounting for camera maximum angles.
	//@param Val: The yaw angle to be applied. Typically an input value.
	void AddGlidingCameraYawInput(float Val);

	//Used during gliding movement.
	//Pitches the camera while accounting for camera maximum angles.
	//@param Val: The input value.
	void AddGlidingCameraPitchInput(float Val);


	UPROPERTY(BlueprintReadWrite, DisplayName="ControlRotationLerpTarget")
		//unstable. intended to serve as lerp target in glide-transition
		FRotator m_ControlRotationLerpTarget{ EForceInit::ForceInitToZero };

	UPROPERTY(EditAnywhere)
		//Pitch input scale while gliding.
		float m_GlidingPitchInputScale{ -1 };
	
	UPROPERTY(EditAnywhere)
		//Yaw input scale while gliding.
		float m_GlidingYawInputScale{ 1.75 };


protected:
	//unstable. lerp executions
	virtual void Tick(float DeltaTime) override;

	//Saves customized input pitch/yaw scales.
	virtual void PostInitializeComponents() override;


	float m_ControlRotationLerpSpeed{ 200 };
	bool	m_bControlLerpComplete{ true },
			m_bControlLerpLastTick{ false },
			m_bGlidingCameraEnabled{ false };

	float	m_StandardPitchInputScale,
			m_StandardYawInputScale;


};
