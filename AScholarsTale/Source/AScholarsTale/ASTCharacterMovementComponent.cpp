#include "ASTCharacterMovementComponent.h"
#include "ASTPlayerController.h"
#include "AScholarsTale.h"
#include "GameFramework/Character.h"

//#include "GameFramework/PhysicsVolume.h"
//Public-------------------



//Protected------------------
void UASTCharacterMovementComponent::PhysCustom(const float DeltaTime, int32 Iterations)
{
	if (!HasValidData() || !CharacterOwner)
	{
		return;

	}

	//UE_LOG(LogTemp, Log, TEXT("Offset: %01s "), *m_GlidingForceOffset.ToString());
	auto CustomMode = (EASTMovementMode)CustomMovementMode;
	if (CustomMode == decltype(CustomMode)::Gliding)
	{			
		PerformGlidingMovement(DeltaTime, Iterations);
		
	}
	else if (CustomMode == decltype(CustomMode)::Roping)
	{
		PerformRopingMovement(DeltaTime, Iterations);

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
	auto InputVector = GetLastInputVector();		
	auto LocalInput = CharacterOwner->GetActorTransform().Inverse().TransformVectorNoScale(InputVector);

	if (auto *PC = Cast<AASTPlayerController>(CharacterOwner->GetController()))
	{
		PC->AddGlidingCameraYawInput((LocalInput.Y * m_GlidingTurnSpeed * DeltaTime) / 50);
		PC->AddGlidingCameraPitchInput(LocalInput.X * DeltaTime);

	}

	//Gliding stats			
	Velocity = (CharacterOwner->GetActorForwardVector() * (m_GlidingForwardDrift + (m_GlidingLeanSpeed * LocalInput.X))) + m_GlidingForceOffset;
	Velocity.Z = GetGravityZ() + -GetGravityZ() * m_GlidingLiftAmount;

	UE_LOG(AST_Movement, Verbose, TEXT("Gliding pending forces: %s"), *PendingForceToApply.ToString());

	//State updates
	++Iterations;
	bJustTeleported = false;
	FVector OldLocation = UpdatedComponent->GetComponentLocation();

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
		//Has downward velocity and upward surface normal that is stepable
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

		}

	}

}

void UASTCharacterMovementComponent::PerformRopingMovement(const float DeltaTime, int32 Iterations)
{
	//const float Friction = 0.5f * GetPhysicsVolume()->FluidFriction;
	//CalcVelocity(DeltaTime, Friction, true, GetMaxBrakingDeceleration());

	
	bJustTeleported = false;
	++Iterations;

	auto CharLocation{ CharacterOwner->GetActorLocation() };
	auto ToCharacter{ CharLocation - m_RopingAttachmentPoint };
		
	auto ToCharacterN{ ToCharacter.GetSafeNormal() };	

	Velocity.Z += GetGravityZ() * DeltaTime;
	
	Velocity += GetLastInputVector() * m_RopingSwingForce * DeltaTime;

	auto CentrifugalScale{ ToCharacterN | Velocity };
	auto Centrifugal{ ToCharacterN * CentrifugalScale };

	Velocity += -Centrifugal;

	
	UE_LOG(AST_Movement, VeryVerbose, TEXT(" \n\tToChar: %s,\n\tVelo: %s,\n\tCentri: %s,\n\tCentriScale: %f\n "), *ToCharacter.ToString(), *Velocity.ToString(), *Centrifugal.ToString(), CentrifugalScale);

	//ApplyVelocityBraking(DeltaTime, .0125, BrakingDecelerationFlying);


/////////
	auto Adjusted = Velocity * DeltaTime;
	FHitResult Hit{ 1.f };
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

	if ( Hit.Time < 1.f )
	{
		//handle hit
		HandleImpact(Hit, DeltaTime, Adjusted);

		//Change to falling mode (end of roping == object hit)
		SetMovementMode(MOVE_Falling);


	}


}


//Private-----------------

