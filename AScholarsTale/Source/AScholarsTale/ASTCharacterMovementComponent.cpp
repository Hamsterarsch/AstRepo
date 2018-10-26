// Fill out your copyright notice in the Description page of Project Settings.
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

	//UE_LOG(LogTemp, Log, TEXT("Offset: %01s "), *m_GlidingForceOffset.ToString());
	auto CustomMode = (EASTMovementMode)CustomMovementMode;
	if (CustomMode == decltype(CustomMode)::Gliding)
	{			
		PerformGlidingMovement(DeltaTime, Iterations);
		//PhysFlying(DeltaTime, Iterations);
		
	}
	else if (CustomMode == decltype(CustomMode)::Roping)
	{
		PerformRopingMovement(DeltaTime, Iterations);

	}
	

}

void UASTCharacterMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
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
		PC->AddGlidingCameraYawInput((LocalInput.Y * m_GlidingTurnSpeed) / 50);
		PC->AddGlidingCameraPitchInput(LocalInput.X);

	}

	//Gliding stats			
	Velocity = (CharacterOwner->GetActorForwardVector() * (m_ForwardDrift + (m_GlidingLeanSpeed * LocalInput.X))) + m_GlidingForceOffset;
	Velocity.Z = (GetGravityZ() * m_GravityCoeff + -GetGravityZ() * m_LiftAmount) + m_GlidingForceOffset.Z;

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
	//Velocity.X *= 0.95f;
	//Velocity.Y *= 0.95f;

	auto InputVectorWS = GetLastInputVector();

	auto CharLocationWS = CharacterOwner->GetActorLocation();
	auto ToSwingPointWS = m_RopingAttachmentPoint - CharLocationWS;

	auto XYMovement = FVector{ ToSwingPointWS.X, ToSwingPointWS.Y, 0 };

	auto DistToCenter = ToSwingPointWS.Size();
	auto VelocityCorrectedMagnitude = Velocity | ToSwingPointWS;

	//UE_LOG(LogTemp, Warning, TEXT("xy: %01s, v: %02s, c: %03f, %04f"), *XYMovement.ToString(), *Velocity.ToString(), DistToCenter, VelocityCorrectedMagnitude);
	FVector GravityDir{ 0, 0, -1 };
	auto DotOfToCenterAndGravity = (GravityDir | ToSwingPointWS.GetSafeNormal()) + 1;


	XYMovement.Normalize();

	XYMovement *= DistToCenter;// DistToCenter;
	Velocity += XYMovement * DeltaTime;
	//Velocity.Z += GetGravityZ() * DeltaTime * DotOfToCenterAndGravity;
	UE_LOG(LogTemp, Warning, TEXT("%01f"), VelocityCorrectedMagnitude);

	//Velocity += MovementDir * DeltaTime;
	//Velocity.Z += GetGravityZ() * DeltaTime;

	
	/*
	auto GravityFactor = (ToSwingPointWS | Velocity.GetSafeNormal()) + 1;
	FVector MovementDir{ ToSwingPointWS };
	MovementDir.Z = 0;
	MovementDir.Normalize();
	//MovementDir *= 980;
	MovementDir.Z = -1;// GetGravityZ();
	MovementDir += InputVectorWS;// *m_RopingSwingSpeed;

	auto d = (FVector(0, 0, - 1) | FVector(0, 0, 1)) + 1;
	UE_LOG(LogTemp, Warning, TEXT("%01f"), d);
	UE_LOG(AST_Movement, VeryVerbose, TEXT("Roping:----------------------\n\t\t InputWS: %01s,\n\t\t ToSwingPointWS: %02s,\n\t\t Velocity: %03s\n\t\t GravityFac: %04f"), 
	*InputVectorWS.ToString(), *ToSwingPointWS.ToString(), *Velocity.ToString(), GravityFactor);
	*/

	/*
	Velocity *= 0.125;
	Velocity.X += m_RopingSwingSpeed * InputVectorWS.X * 980;
	Velocity.Y += m_RopingSwingSpeed * InputVectorWS.Y * 980;

	//Ignores all applied forces in terms of z
	Velocity.Z += GetGravityZ() * GravityFactor;
	//Velocity.Z = 0;
	Velocity = MovementDir * GravityFactor;
	*/

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

