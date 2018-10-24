// Fill out your copyright notice in the Description page of Project Settings.
#include "ASTCharacter.h"
#include "AScholarsTale.h"
#include "ASTPlayerController.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"


//Public---------------------------
AASTCharacter::AASTCharacter(const FObjectInitializer &Initializer) :
	Super(Initializer.SetDefaultSubobjectClass<UASTCharacterMovementComponent>(Super::CharacterMovementComponentName)),
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

	//Movement bindings
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AASTCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AASTCharacter::MoveRight);

	//Camera bindings
	PlayerInputComponent->BindAxis(TEXT("CameraX"), this, &AASTCharacter::AddControlRotationYaw);
	PlayerInputComponent->BindAxis(TEXT("CameraY"), this, &AASTCharacter::AddControlRotationPitch);

	//Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);

	//Interact
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AASTCharacter::PerformInteract);

	//Glide
	PlayerInputComponent->BindAction(TEXT("Gliding"), IE_Pressed, this, &AASTCharacter::StartGliding);
	PlayerInputComponent->BindAction(TEXT("Gliding"), IE_Released, this, &AASTCharacter::StopGliding);
	

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

void AASTCharacter::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	if (!bPressedJump || !GetCharacterMovement()->IsFalling())
	{
		//UE_LOG(LogTemp, Log, TEXT("Stats------------------ : Cur- %01i, Las- %02i, CurCm- %03i, LasCm- %04i"), GetCharacterMovement()->MovementMode.GetValue(), PreviousMovementMode, GetCharacterMovement()->CustomMovementMode, PreviousCustomMode);
		if ( PreviousMovementMode == MOVE_Custom || GetCharacterMovement()->MovementMode == MOVE_Custom )
		{
			//UE_LOG(LogTemp, Log, TEXT("CUSTOM"));
			if( (EASTMovementMode)GetCharacterMovement()->CustomMovementMode == EASTMovementMode::Gliding )
			{
				// Salakis
				m_PreGlideJumpCount = JumpCurrentCount;
				//UE_LOG(LogTemp, Log, TEXT("PRE_GLIDING"));

			}
			else if ( (EASTMovementMode)PreviousCustomMode == EASTMovementMode::Gliding)
			{
				JumpCurrentCount = m_PreGlideJumpCount;
				//UE_LOG(LogTemp, Log, TEXT("POST_GLIDING"));

			}
			else
			{
				ResetJumpState();
				//UE_LOG(LogTemp, Log, TEXT("NONGLIDING"));

			}

			//UE_LOG(LogTemp, Log, TEXT("CustomAR_0 : %01i, %02i"), GetCharacterMovement()->MovementMode.GetValue(), PreviousMovementMode);

		}
		else
		{
			ResetJumpState();
			//UE_LOG(LogTemp, Log, TEXT("Reset_1"));
			//UE_LOG(LogTemp, Log, TEXT("Reset_0 : %01i, %02i"), GetCharacterMovement()->MovementMode.GetValue(), PreviousMovementMode);

		}
		

	}

	// Recored jump force start time for proxies. Allows us to expire the jump even if not continually ticking down a timer.
	if (bProxyIsJumpForceApplied && GetCharacterMovement()->IsFalling())
	{
		ProxyJumpForceStartedTime = GetWorld()->GetTimeSeconds();
	}

	K2_OnMovementModeChanged(PreviousMovementMode, GetCharacterMovement()->MovementMode, PreviousCustomMode, GetCharacterMovement()->CustomMovementMode);
	MovementModeChangedDelegate.Broadcast(this, PreviousMovementMode, PreviousCustomMode);

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

void AASTCharacter::StartGliding()
{
	if (auto *PC = Cast<AASTPlayerController>(Controller))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Click"));
		PC->EnableGlidingCamera();
		GetCharacterMovement()->SetMovementMode(MOVE_Custom, (uint8)EASTMovementMode::Gliding);

	}

}

void AASTCharacter::StopGliding()
{
	if (auto *PC = Cast<AASTPlayerController>(Controller))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Unclick"));
		
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		PC->DisableGlidingCamera();

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