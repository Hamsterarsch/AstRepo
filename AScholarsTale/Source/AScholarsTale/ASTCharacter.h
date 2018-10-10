// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASTCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractSignature);
DECLARE_DYNAMIC_DELEGATE(FInteractCallbackSignature);

UCLASS()
class ASCHOLARSTALE_API AASTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AASTCharacter();
	virtual void PostInitializeComponents() override;  
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void AddInteraction(const FInteractCallbackSignature &Event);
	
	UFUNCTION(BlueprintCallable)
		void RemoveInteraction(const FInteractCallbackSignature &Event);
	
protected:
	virtual void BeginPlay() override;



private:
	void MoveRight(float AxisValue);
	void MoveForward(float AxisValue);
	void AddControlRotationPitch(float AxisValue);
	void AddControlRotationYaw(float AxisValue);
	void Jump();
	void StopJumping();
	void PerformInteract();


	unsigned int m_InteractionCount;
	FInteractSignature m_InteractDelegate;
	unsigned int m_CurrentJumpCount;
	bool m_bCanJump;
	
	UPROPERTY(EditDefaultsOnly)
		unsigned int m_MaxJumpCount;


};
