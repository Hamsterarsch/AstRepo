// Fill out your copyright notice in the Description page of Project Settings.
#include "ASTCharacter.h"
#include "AScholarsTale.h"
#include "ASTPlayerController.h"
#include "Engine/World.h"
#include "Rope.h"
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
	
	//Rope
	PlayerInputComponent->BindAction(TEXT("RopeGrab"), IE_Pressed, this, &AASTCharacter::TryEnterRopingMode); 
	PlayerInputComponent->BindAction(TEXT("RopeGrab"), IE_Released, this, &AASTCharacter::LeaveRopingMode);

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

void AASTCharacter::TryEnterRopingMode()
{
	if (m_pTargetRope)
	{
		if (auto *MC = Cast< UASTCharacterMovementComponent >(GetCharacterMovement()))
		{
			MC->m_RopingAttachmentPoint = m_pTargetRope->GetActorLocation();
			MC->SetMovementMode(EMovementMode::MOVE_Custom, (uint8)EASTMovementMode::Roping);

		}
	
	}


}

void AASTCharacter::LeaveRopingMode()
{
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Custom && (EASTMovementMode)GetCharacterMovement()->CustomMovementMode == EASTMovementMode::Roping)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	
	}


}

void AASTCharacter::OnRopeEntered(const ARope *pRope)
{
	m_pTargetRope = pRope;
	UE_LOG(LogTemp, Log, TEXT("RopeEntered"));

}

void AASTCharacter::OnRopeLeft()
{
	LeaveRopingMode();
	m_pTargetRope = nullptr;
	UE_LOG(LogTemp, Log, TEXT("RopeLeft"));

}

void AASTCharacter::CheckJumpInput(float DeltaTime)
{
	//Implementation from UCharacter

	auto CharacterMovement = GetCharacterMovement();
	if (CharacterMovement)
	{
		if (bPressedJump)
		{
			///////Commented block is used in UCharacter to prevent jumps while falling when having no base
			///////but we want the same amount of jumps in air and on ground

			// If this is the first jump and we're already falling,
			// then increment the JumpCount to compensate.
			/*
			const bool bFirstJump = JumpCurrentCount == 0;
			if (bFirstJump && CharacterMovement->IsFalling())
			{
				++JumpCurrentCount;
			}
			*/

			const bool bDidJump = CanJump() && CharacterMovement->DoJump(bClientUpdating);
			if (bDidJump)
			{
				// Transition from not (actively) jumping to jumping.
				if (!bWasJumping)
				{
					++JumpCurrentCount;
					JumpForceTimeRemaining = GetJumpMaxHoldTime();
					OnJumped();
				}
			}

			bWasJumping = bDidJump;
		}
	}



}



//Protected-------------------------------------------

void AASTCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void AASTCharacter::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	UE_LOG(AST_Movement, Verbose, TEXT("---------OnMovementChanged_Stats------------------\n CurrentMode: %01i, LastMode: %02i, CurrentCustom: %03i, LastCustom: %04i"), 
		GetCharacterMovement()->MovementMode.GetValue(), PreviousMovementMode, GetCharacterMovement()->CustomMovementMode, PreviousCustomMode);
	
	//Handling for custom movement modes
	if ( PreviousMovementMode == MOVE_Custom )
	{
		auto PreviousCustomModeTyped = (EASTMovementMode)PreviousCustomMode;
		if (PreviousCustomModeTyped == EASTMovementMode::Gliding)
		{
			JumpCurrentCount = m_PreGlideJumpCount;
			UE_LOG(AST_Movement, Log, TEXT("Leaving custom movement, jump restore"));

		}
		else if (PreviousCustomModeTyped == EASTMovementMode::Roping)
		{
			UE_LOG(AST_Movement, Log, TEXT("Leaving roping movement, jump restore to max"));
			JumpCurrentCount = 0;

		}
		UE_LOG(AST_Movement, Verbose, TEXT("Current jump count: %i"), JumpCurrentCount);

	}

	if (GetCharacterMovement()->MovementMode == MOVE_Custom)
	{
		auto CurrentCustomModeTyped = (EASTMovementMode)GetCharacterMovement()->CustomMovementMode;

		//Enter cases
		switch (CurrentCustomModeTyped)
		{
		case EASTMovementMode::Gliding:

			//Null velocity so it does not influence gliding computations.
			GetCharacterMovement()->Velocity = FVector::ZeroVector;

			//Safe the remaining jumps on gliding for restore.
			m_PreGlideJumpCount = JumpCurrentCount;
			UE_LOG(AST_Movement, Verbose, TEXT("Entering gliding movement"));
			break;

		case EASTMovementMode::Roping:
			//m_PreGlideJumpCount = JumpCurrentCount;
			//GetCharacterMovement()->Velocity = FVector::ZeroVector;
			UE_LOG(AST_Movement, Verbose, TEXT("Entering roping movement"));
			break;

		default:
			UE_LOG(AST_Movement, Warning, TEXT("Entering custom movement: None"));
			break;

		}
		UE_LOG(AST_Movement, Verbose, TEXT("Saved jump count: %i"), m_PreGlideJumpCount);

	}

	if (GetCharacterMovement()->MovementMode != MOVE_Custom && PreviousMovementMode != MOVE_Custom)
	{
		if( !bPressedJump || !GetCharacterMovement()->IsFalling() )
		{
			ResetJumpState();

			UE_LOG(AST_Movement, Verbose, TEXT("Reseting jump state: MovementMode: %01i, PreviousMovementMode: %02i"),
			GetCharacterMovement()->MovementMode.GetValue(), PreviousMovementMode);

		}

	}

	// Recored jump force start time for proxies. Allows us to expire the jump even if not continually ticking down a timer.
	if (bProxyIsJumpForceApplied && GetCharacterMovement()->IsFalling())
	{
		ProxyJumpForceStartedTime = GetWorld()->GetTimeSeconds();

	}

	//Propagate blueprint events.
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
		if 
		(
			GetCharacterMovement()->MovementMode == MOVE_Custom 
			&& (EASTMovementMode)GetCharacterMovement()->CustomMovementMode == EASTMovementMode::Gliding
		)
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			PC->DisableGlidingCamera();
		
		}

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