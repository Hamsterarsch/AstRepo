#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ASTCameraManager.generated.h"

UCLASS()
//Class that cointains camera settings and
//functions to enable and handle the camera during
//gliding. Needs a class of type AASTPlayerController as its owner.
class ASCHOLARSTALE_API AASTCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
		
public:
	//Moves the camera yaw max and min angles by the input amount.
	//@param Val: the amount to move.
	void AddYawInput(float Val);

	//If adding the input value does not exceed m_GlidingMaxLeanAngle
	//this function moves the pitch max/min angles and applies fov changes.
	//Also adds pitch control input to the owning player controller.
	//@param Val: The input value from the player.
	void AddPitchInput(float Val);

	//unstable
	void UseGlidingViewAngles(const FRotator &ControlRotation);

	//unstable
	void RestoreStandardViewAngles();


	UPROPERTY(EditAnywhere)
		//The yaw angle that the camera can move in during gliding.
		float m_GlidingViewYawAngle{ 90 };

	UPROPERTY(EditAnywhere)
		//The maximum border of the pitch angle that the camera
		//can use during gliding.
		float m_GlidingViewPitchMax{ 33 };

	UPROPERTY(EditAnywhere)
		//The minimum border of the pitch angle that the camera
		//can use during gliding.
		float m_GlidingViewPitchMin{ -66 };

	UPROPERTY(EditAnywhere)
		//The angle that the camera is pitched by while leaning.
		float m_GlidingMaxLeanAngle{ 10 };

	UPROPERTY(EditAnywhere)
		//unstable
		float m_GlidingTurnCameraRollAngle{ 10 };

	UPROPERTY(EditAnywhere)
		//Coefficient of camera pitching speed during leaning.
		float m_GlidingLeanCameraSpeed{ 0.25 };


protected:
	//Saves the configured standard view angles for state restore.
	virtual void PostInitializeComponents() override;


	//The last input to AddPitchInput.
	float	m_LastPitchInput{ 0 };

	//The current amount of leaning (pitch) applied.
	float	m_GlidingCurrentLeanAngle{ 0 };

	//The yaw max set from editor.
	float	m_StandardViewYawMax;
			
	//The yaw min set from editor.
	float	m_StandardViewYawMin;

	//The pitch max set from editor.
	float	m_StandardViewPitchMax;

	//The pitch min set from editor.
	float	m_StandardViewPitchMin;
		

};
