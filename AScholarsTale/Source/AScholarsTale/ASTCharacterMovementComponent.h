// Fill out your copyright notice in the Description page of Project Settings
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ASTCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EASTMovementMode : uint8
{
	None UMETA(DisplayName="None"),
	Gliding UMETA(DisplayName="Gliding")


};

//Extended movement component. 
//Includes gliding movment mode
UCLASS()
class ASCHOLARSTALE_API UASTCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:


protected:
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;

	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0) override;
	//virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UPROPERTY(EditAnywhere)
		float m_GravityCoeff{ 1 };

	UPROPERTY(EditAnywhere)
		float m_LiftAmount{ 0.85 };

	UPROPERTY(EditAnywhere)
		float m_ForwardDrift{ 500 };

	UPROPERTY(EditAnywhere)
		float m_GlidingLeanSpeed{ 400 };

	UPROPERTY(EditAnywhere)
		float m_GlidingTurnSpeed{ 1.125 };
		

private:
	int32 m_PreGlidingJumpCount;


	
};
