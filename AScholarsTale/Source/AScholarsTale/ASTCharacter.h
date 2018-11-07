// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASTCharacterMovementComponent.h"
#include "ASTCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractSignature);
DECLARE_DYNAMIC_DELEGATE(FInteractCallbackSignature);

UCLASS(Blueprintable, Abstract, CustomConstructor)
//Class representing a player character for AST
//Input bindings executed via PlayerInputComponent
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

	//unused feature code, todo: remove or not
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
		//Called when the range of a rope is entered.
		//@param pRope: pointer to the rope entered.
		void OnRopeEntered(const class ARope *pRope);

	UFUNCTION(BlueprintCallable)
		//Called when the range of a rope is left.
		void OnRopeLeft();

	UFUNCTION(BlueprintCallable)
		//Performs transition to roping movement if possible
		//eg. all data is valid.
		void TryEnterRopingMode();

	UFUNCTION(BlueprintCallable)
		//Transitions to falling movement mode
		//if currently using roping movement.
		void LeaveRopingMode();
	
	//override to remove jump decrement on falling movement.
	//@param DeltaTime: last frame time.
	virtual void CheckJumpInput(float DeltaTime) override;

	
	UPROPERTY()
		//The rope that is currently in proximity.
		const class ARope *m_pTargetRope;
	

protected:
	//Empty
	virtual void BeginPlay() override;

	//Called when the movement mode changes.
	//Overridden to prevent movement state resets when leaving certain custom modes.
	//(This has to be in this class because its base resets the jump state.)
	//@param PreviousMovementMode: The movement mode before the change.
	//@param PreviousCustomMode: The custom movement sub-mode prior to the change.
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

	//Enables the gliding camera and transitions to gliding movement.
	void StartGliding();

	//Disables the gliding camera and transitions to falling state if the current movement is gliding.
	void StopGliding();
	   
	//Contains old jump implementation, todo: clean
	void Jump();

	//Stops jumping, todo: maybe just use the base's function.
	void StopJumping();
	
	//Broadcasts the interact delegate. Used for input binding.
	void PerformInteract();

	//The delegate to that interaction functions should be bound to.
	FInteractSignature m_InteractDelegate;

	//The current count of delegates bound by the m_InteractDelegate.
	unsigned int m_InteractionCount;

	//todo
	unsigned int m_CurrentJumpCount;

	//The number of jumps already executed on entering gliding movement.
	uint32 m_PreGlideJumpCount;
			
	//todo
	UPROPERTY()
		unsigned int m_MaxJumpCount;


};
