// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASTCharacterMovementComponent.h"
#include "ASTCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractSignature);
DECLARE_DYNAMIC_DELEGATE(FInteractCallbackSignature);

//Class representing a player character for AST
//Input bindings executed via PlayerInputComponent
UCLASS(Blueprintable, Abstract)
class ASCHOLARSTALE_API AASTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//Ctor
	AASTCharacter(const class FObjectInitializer &Initializer);

	//Property dependent initialization
	virtual void PostInitializeComponents() override;  

	//Setup of input bindings for player
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Empty
	virtual void Tick(float DeltaTime) override;

	//Adds an event to the delegate called on interaction.
	//@param Event: The event to add to the interaction callback, void().
	UFUNCTION(BlueprintCallable)
		void AddInteraction(const FInteractCallbackSignature &Event);
	
	//Tries to remove a event from the characters interaction delegate.
	//@param Event: The event that should be removed from the interact delegate.
	UFUNCTION(BlueprintCallable)
		void RemoveInteraction(const FInteractCallbackSignature &Event);

	UFUNCTION(BlueprintCallable)
		void OnRopeEntered(const class ARope *pRope);

	UFUNCTION(BlueprintCallable)
		void OnRopeLeft();

	UFUNCTION(BlueprintCallable)
		void TryEnterRopingMode();

	UFUNCTION(BlueprintCallable)
		void LeaveRopingMode();
	
	UPROPERTY()
		const class ARope *m_pTargetRope;
	

protected:
	//Empty
	virtual void BeginPlay() override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;



private:
	//Adds an input value to the movement component.
	//@param AxisValue: The magnitude of the movement. 
	//Positive values produce movement to the right of the actor.
	void MoveRight(float AxisValue);

	//Adds an input value to the movement component.
	//@param AxisValue: The magnitude of the movement. 
	//Positive values produce movement in direction of the front of the actor.
	void MoveForward(float AxisValue);

	//Adds input to the control rotation of the controller of this character.
	//@param AxisValue: The magnitude of the movement.
	void AddControlRotationPitch(float AxisValue);

	//Adds input to the control rotation of the controller of this character.
	//@param AxisValue: The magnitude of the movement.
	void AddControlRotationYaw(float AxisValue);

	void StartGliding();

	void StopGliding();



	//todo
	void Jump();

	//todo
	void StopJumping();
	
	//Broadcasts the interact delegate. Used for input binding.
	void PerformInteract();

	//The delegate to that interaction functions should be bound to.
	FInteractSignature m_InteractDelegate;

	//The current count of delegates bound by the m_InteractDelegate.
	unsigned int m_InteractionCount;

	//todo
	unsigned int m_CurrentJumpCount;

	uint32 m_PreGlideJumpCount;
			

	//todo
	UPROPERTY(EditDefaultsOnly)
		unsigned int m_MaxJumpCount;


};
