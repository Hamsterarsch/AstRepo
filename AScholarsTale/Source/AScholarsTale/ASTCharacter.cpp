// Fill out your copyright notice in the Description page of Project Settings.
#include "ASTCharacter.h"
#include "AScholarsTale.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"


//Public---------------------------
AASTCharacter::AASTCharacter() :
	m_bCanJump{ true },
	m_MaxJumpCount{ 2 }
{
	PrimaryActorTick.bCanEverTick = true;


}

void AASTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_CurrentJumpCount = m_MaxJumpCount;


}

void AASTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Not falling but not having max jump charges
	if (m_CurrentJumpCount != m_MaxJumpCount && !GetCharacterMovement()->IsFalling())
	{
		m_CurrentJumpCount = m_MaxJumpCount;

	}


}

void AASTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AASTCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AASTCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("CameraX"), this, &AASTCharacter::AddControlRotationYaw);
	PlayerInputComponent->BindAxis(TEXT("CameraY"), this, &AASTCharacter::AddControlRotationPitch);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AASTCharacter::PerformInteract);

}

void AASTCharacter::AddInteraction(const FInteractCallbackSignature &Event)
{
	m_InteractDelegate.Add(Event);
	++m_InteractionCount;
	//Todo: show interact interface here if interact count was previously 0


}

void AASTCharacter::RemoveInteraction(const FInteractCallbackSignature &Event)
{
	if (m_InteractDelegate.Contains(Event))
	{
		if (m_InteractionCount <= 0)
		{			
			UE_LOG(AST_LogLogicError, Error, TEXT("AASTCharacter:: No interaction callbacks remaining on remove!"));
		
		}

		m_InteractDelegate.Remove(Event);
		--m_InteractionCount;

		//Todo: hide interact widget on 0 interactions

	}
	else
	{
		UE_LOG(AST_LogInfo, Log, TEXT("AASTCharacter:: Tried to remove an interaction event that was not bound."));

	}


}

//Protected-------------------------------------------
void AASTCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}


//Private-------------------------------
void AASTCharacter::MoveRight(const float AxisValue)
{
	if (!FMath::IsNearlyEqual(AxisValue, 0) && InputEnabled())
	{
		GetMovementComponent()->AddInputVector(GetActorRightVector() * AxisValue);

	}


}

void AASTCharacter::MoveForward(float AxisValue)
{
	if (!FMath::IsNearlyEqual(AxisValue, 0) && InputEnabled())
	{
		GetMovementComponent()->AddInputVector(GetActorForwardVector() * AxisValue);

	}


}

void AASTCharacter::AddControlRotationPitch(float AxisValue)
{
	if (!FMath::IsNearlyEqual(AxisValue, 0) && InputEnabled())
	{
		AddControllerPitchInput(AxisValue);

	}


}

void AASTCharacter::AddControlRotationYaw(float AxisValue)
{
	if (!FMath::IsNearlyEqual(AxisValue, 0) && InputEnabled())
	{
		AddControllerYawInput(AxisValue);

	}


}

void AASTCharacter::Jump()
{
	/*
	if (InputEnabled() && m_bCanJump)
	{
		//Actions to make multiple jumps possible
		if (m_CurrentJumpCount > 0)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			--m_CurrentJumpCount;

		}
		//Try to issue a jump request to the character base
		Super::Jump();

	}
	*/
	Super::Jump();

}

void AASTCharacter::StopJumping()
{
	if (InputEnabled())
	{
		Super::StopJumping();

	}


}

void AASTCharacter::PerformInteract()
{
	if (InputEnabled())
	{
		m_InteractDelegate.Broadcast();

	}


}