// Fill out your copyright notice in the Description page of Project Settings.
#include "ASTCharacter.h"
#include "AScholarsTale.h"
#include "ASTPlayerController.h"
#include "Engine/World.h"
#include "Rope.h"
#include "Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Slingable.h"
#include "TeleballBase.h"
#include "Components/InputComponent.h"
#include "ASTGameInstance.h"
#include "ASTSaveGame.h"
#include "UserWidget.h"

#include "Components/CapsuleComponent.h"


//Public---------------------------
AASTCharacter::AASTCharacter(const FObjectInitializer &Initializer) :
	Super(Initializer.SetDefaultSubobjectClass<UASTCharacterMovementComponent>(Super::CharacterMovementComponentName)),
	m_MaxJumpCount{ 2 },
	m_WalkingDistSinceFootstep{ 0 },
	m_bIsGlidingEnabled{ true },
	m_bIsJumpingEnabled{ true }
{
	PrimaryActorTick.bCanEverTick = true;
	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_pCamera->SetupAttachment(GetRootComponent());
	m_pCamera->SetRelativeLocation({ 0,0,50 });

	m_pGrabbedRoot = CreateDefaultSubobject<USceneComponent>(TEXT("GrabbableRoot"));
	m_pGrabbedRoot->SetupAttachment(m_pCamera);
	m_pGrabbedRoot->SetRelativeLocation({ 25,0,0 });
		
	
}

void AASTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_CurrentJumpCount = m_MaxJumpCount;

	UClass *pWidgetObject = nullptr;
	if ((pWidgetObject = m_pInteractWidgetAsset.Get()) == nullptr)
	{
		pWidgetObject = m_pInteractWidgetAsset.LoadSynchronous();

	}

	if (pWidgetObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *pWidgetObject->GetName());
		m_pInteractWidget = CreateWidget(GetWorld(), pWidgetObject);


	}

	m_WalkingLastFramePos = GetActorLocation();


}

void AASTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Not falling but not having max jump charges
	if (m_CurrentJumpCount != m_MaxJumpCount && !GetCharacterMovement()->IsFalling())
	{
		m_CurrentJumpCount = m_MaxJumpCount;

	}

	//(this is used)
	ProcessFootsteps();
	
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

	//Teleball
	PlayerInputComponent->BindAction(TEXT("Teleball"), IE_Pressed, this, &AASTCharacter::SpawnTeleball);
	PlayerInputComponent->BindAction(TEXT("Teleball"), IE_Released, this, &AASTCharacter::TryLaunchCurrentSlingable);
	PlayerInputComponent->BindAction(TEXT("TeleballPort"), IE_Pressed, this, &AASTCharacter::TryTeleportToTeleball);

}

void AASTCharacter::AddInteraction(const FInteractCallbackSignature &Event)
{
	m_InteractDelegate.Add(Event);

	if (++m_InteractionCount == 1)
	{
		m_pInteractWidget->AddToViewport();

	}
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

		if (--m_InteractionCount == 0)
		{
			m_pInteractWidget->RemoveFromViewport();

		}
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

	if( auto *pGI{ static_cast<UASTGameInstance *>(GetGameInstance())} )
	{
		TScriptDelegate<> Delegate;
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AASTCharacter, OnLoadGame));
		pGI->m_OnLoadGame.Add(Delegate);

	}

	m_WalkingLastGroundedTf = m_WalkingNextToLastGroundedTf = GetActorTransform();

}

void AASTCharacter::OnLoadGame(const class UASTSaveGame *pSavegame)
{
	SetActorTransform(pSavegame->m_CurrentPlayerTransform);

}

void AASTCharacter::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	UE_LOG(AST_Movement, Verbose, TEXT("---------OnMovementChanged_Stats------------------\n CurrentMode: %01i, LastMode: %02i, CurrentCustom: %03i, LastCustom: %04i"), 
		GetCharacterMovement()->MovementMode.GetValue(), PreviousMovementMode, GetCharacterMovement()->CustomMovementMode, PreviousCustomMode);
	
	if (PreviousMovementMode == MOVE_Walking)
	{
		//m_WalkingLastGroundedTf = GetActorTransform();

	}

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
	else if (GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		ReceiveOnLanded();

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

FTransform AASTCharacter::GetSavedWalkingTransform() const
{
	static uint32 GetTimes{};
	++GetTimes;

	return GetTimes % 2 ? m_WalkingLastGroundedTf : m_WalkingNextToLastGroundedTf;


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
	//Prevents instant transition from walking to gliding
	//(and subsequently back to walking, which triggers all vx assets == bad)
	if (GetCharacterMovement()->MovementMode == MOVE_Walking)
	{
		return;
	}

	auto *PC = Cast<AASTPlayerController>(Controller);
	if (PC && m_bIsGlidingEnabled)
	{
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
	if (m_bIsJumpingEnabled)
	{
		Super::Jump();

	}


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

void AASTCharacter::SpawnTeleball()
{
	if (m_pLastGrabbed)
	{
		if (auto *pOldTeleball = Cast<ATeleballBase>(m_pLastGrabbed))
		{
			pOldTeleball->OnTeleballRecast();
		
		}
		
	}

	if (!m_pCurrentlyGrabbed)
	{
		UClass *pClass = nullptr;
		if ((pClass = m_pTeleballAsset.Get()) == nullptr)
		{	
			pClass = m_pTeleballAsset.LoadSynchronous();//.ToSoftObjectPath().TryLoad();
			
			/*
			if (auto *pBlueprint = Cast<UBlueprint>(pClass))
			{
				pClass = pBlueprint->GeneratedClass;
				pClass->Generated
			}
			else
			{
				pClass = pClass->StaticClass();

			}
			*/

			//pClass = m_pTeleballAsset.LoadSynchronous();
			
		}
		UE_LOG(AST_Teleport, Log, TEXT("Teleball class = %s"), *pClass->GetName());

		if (pClass)
		{			
			auto Transform = m_pGrabbedRoot->GetComponentTransform();
			m_pCurrentlyGrabbed = GetWorld()->SpawnActor(pClass, &Transform);
			m_pCurrentlyGrabbed->AttachToComponent(m_pGrabbedRoot, FAttachmentTransformRules::KeepWorldTransform);
			
		}
		else
		{
			UE_LOG(AST_Teleport, Error, TEXT("ASTCharacter::Could not load teleball class."));

		}
	
	}
	
	   
}

void AASTCharacter::TryLaunchCurrentSlingable()
{
	if (auto *pSlingable = Cast<ISlingable>(m_pCurrentlyGrabbed))
	{		
		m_pCurrentlyGrabbed->DetachAllSceneComponents(m_pGrabbedRoot, FDetachmentTransformRules::KeepWorldTransform);
		pSlingable->Launch(m_pCamera->GetForwardVector() * m_SlingForce);
		
		m_pLastGrabbed = m_pCurrentlyGrabbed;
		m_pCurrentlyGrabbed = nullptr;

	}


}

void AASTCharacter::TryTeleportToTeleball()
{
	if (auto *pTargetTeleball = Cast<ATeleballBase>(m_pLastGrabbed))
	{
		TeleportTo(pTargetTeleball->GetActorLocation(), GetActorRotation());
		pTargetTeleball->OnTeleportTo();

	}


}

void AASTCharacter::ProcessFootsteps()
{
	static uint32 StepCount{};
	++StepCount;
	if( auto *pCharMovement = Cast<UCharacterMovementComponent>(GetMovementComponent()) )
	{
		if ( pCharMovement->MovementMode == MOVE_Walking )
		{			
			m_WalkingDistSinceFootstep += (m_WalkingLastFramePos - GetActorLocation()).Size();
			m_WalkingLastFramePos = GetActorLocation();

			if (m_WalkingDistSinceFootstep >= m_WalkingStepSize)
			{
				m_WalkingDistSinceFootstep = 0;
				if (StepCount % 2)
				{
					m_WalkingLastGroundedTf = GetActorTransform();
				}
				else
				{
					m_WalkingNextToLastGroundedTf = GetActorTransform();
				}

				OnFootstep();

			}

		}

	}


}

void AASTCharacter::ReceiveOnLanded()
{
	m_WalkingLastFramePos = GetActorLocation();
	OnCharLanded();

}
