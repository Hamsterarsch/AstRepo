#include "ASTCharacterMovementComponent.h"
#include "ASTPlayerController.h"
#include "AScholarsTale.h"
#include "GameFramework/Character.h"

//Public-------------------


//Protected------------------
void UASTCharacterMovementComponent::PhysCustom(const float DeltaTime, int32 Iterations)
{
	if (!HasValidData() || !CharacterOwner)
	{
		return;

	}
	
	//Call appropriate function to handle movement for the current custom sub-mode
	auto CustomMode = (EASTMovementMode)CustomMovementMode;
	switch (CustomMode)
	{
	case decltype(CustomMode)::Gliding:
		PerformGlidingMovement(DeltaTime, Iterations);
		break;
	case decltype(CustomMode)::Roping:
		PerformRopingMovement(DeltaTime, Iterations);
		break;

	}
	

}

void UASTCharacterMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	//Hack that prevents transition from gliding mode to flying which happens for whatever reason
	//todo: find cause
	if ( !(NewMovementMode == MOVE_Flying && (MovementMode == MOVE_Custom && CustomMovementMode == (uint8)EASTMovementMode::Gliding)) )
	{
		Super::SetMovementMode(NewMovementMode, NewCustomMode);

	}

}

void UASTCharacterMovementComponent::PerformGlidingMovement(const float DeltaTime, int32 Iterations)
{
	//auto InputVector = GetLastInputVector();		
	
	//Input local to the character owner.
	auto LocalInput = CharacterOwner->GetActorTransform().Inverse().TransformVectorNoScale(GetLastInputVector());

	//Determine camera rotation amount based on local input.
	auto *PC = Cast<AASTPlayerController>(CharacterOwner->GetController());
	if (PC)
	{
		PC->AddGlidingCameraYawInput((LocalInput.Y * m_GlidingTurnSpeed * DeltaTime) / 50);
		PC->AddGlidingCameraPitchInput(LocalInput.X * DeltaTime);

	}

	//Gliding velocity computation.	
	auto ForwardSpeed = m_GlidingForwardDrift + m_GlidingLeanSpeed * LocalInput.X;
	Velocity = CharacterOwner->GetActorForwardVector() * ForwardSpeed + m_GlidingForceOffset;
	Velocity.Z = GetGravityZ() + -GetGravityZ() * m_GlidingLiftAmount;

	UE_LOG(AST_Movement, Verbose, TEXT("Gliding pending forces: %s"), *PendingForceToApply.ToString());

	//State updates
	++Iterations;
	bJustTeleported = false;
	FVector OldLocation = UpdatedComponent->GetComponentLocation();

	//Slice velocity
	const FVector Adjusted = Velocity * DeltaTime;

	//Move and check for obstacles
	FHitResult Hit{ 1.f };
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

	//if blocking hit...
	if (Hit.Time < 1.f)
	{
		const FVector GravDir = FVector(0.f, 0.f, -1.f);
		const FVector VelDir = Velocity.GetSafeNormal();
		// | is dot
		const float UpDown = GravDir | VelDir;

		bool bSteppedUp = false;
		//...and has downward velocity and upward surface normal that is stepable
		if ((FMath::Abs(Hit.ImpactNormal.Z) < 0.2f) && (UpDown < 0.5f) && (UpDown > -0.2f) && CanStepUp(Hit))
		{
			//try to step up
			float stepZ = UpdatedComponent->GetComponentLocation().Z;
			bSteppedUp = StepUp(GravDir, Adjusted * (1.f - Hit.Time), Hit);
			if (bSteppedUp)
			{
				OldLocation.Z = UpdatedComponent->GetComponentLocation().Z + (OldLocation.Z - stepZ);

			}

		}

		//...which is not a step up case...
		if (!bSteppedUp)
		{
			//adjust and try again
			HandleImpact(Hit, DeltaTime, Adjusted);

			//Change to walking mode (end of gliding == earth touch)
			SetMovementMode(MOVE_Falling);
			PC->DisableGlidingCamera();

		}

	}


}

void UASTCharacterMovementComponent::PerformRopingMovement(const float DeltaTime, int32 Iterations)
{
	//const float Friction = 0.5f * GetPhysicsVolume()->FluidFriction;
	//CalcVelocity(DeltaTime, Friction, true, GetMaxBrakingDeceleration());
	//ApplyVelocityBraking(DeltaTime, .0125, BrakingDecelerationFlying);
		
	bJustTeleported = false;
	++Iterations;

	auto ToCharacter{ CharacterOwner->GetActorLocation() - m_RopingAttachmentPoint };		
	auto ToCharacterN{ ToCharacter.GetSafeNormal() };	

	//Account for gravity
	Velocity.Z += GetGravityZ() * DeltaTime;
	
	//Account for player input
	Velocity += GetLastInputVector() * m_RopingSwingForce * DeltaTime;

	//Compute centrifugal force
	auto CentrifugalScale{ ToCharacterN | Velocity };
	auto Centrifugal{ ToCharacterN * CentrifugalScale };

	//Add as centripetal force
	Velocity += -Centrifugal;
		
	UE_LOG(AST_Movement, VeryVerbose, TEXT(" \n\tToChar: %s,\n\tVelo: %s,\n\tCentri: %s,\n\tCentriScale: %f\n "), *ToCharacter.ToString(), *Velocity.ToString(), *Centrifugal.ToString(), CentrifugalScale);
	
	//Slice velocity
	auto Adjusted = Velocity * DeltaTime;

	//Move and check for obstacles...
	FHitResult Hit{ 1.f };
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

	//...and if hit...
	if ( Hit.Time < 1.f )
	{
		//...handle hit
		HandleImpact(Hit, DeltaTime, Adjusted);

		//and change to falling mode (end of roping == object hit)
		SetMovementMode(MOVE_Falling);


	}


}


//Private-----------------

