// Fill out your copyright notice in the Description page of Project Settings
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ASTCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ASTMovementMode : uint8
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



private:
	


	
};
