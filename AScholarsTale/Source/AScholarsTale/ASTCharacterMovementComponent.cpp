// Fill out your copyright notice in the Description page of Project Settings.
#include "ASTCharacterMovementComponent.h"
#include "GameFramework/Character.h"


//Public-------------------



//Protected------------------
void UASTCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	if (CharacterOwner)
	{
		auto CustomMode = (EASTMovementMode)CustomMovementMode;
		if (CustomMode == decltype(CustomMode)::Gliding)
		{
			UE_LOG(LogTemp, Warning, TEXT("UASTCharacterCustomMovementComponent:: GlidingMode"));

			//Gliding stats
			Velocity = CharacterOwner->GetActorForwardVector() * ForwardDrift;			
			Velocity.Z = GetGravityZ() * 0.125f * GravityCoeff;
					
			//State updates
			++Iterations;
			bJustTeleported = false;
			FVector OldLocation = UpdatedComponent->GetComponentLocation();

			const FVector Adjusted = Velocity * DeltaTime;

			//Move and check for obstacles
			FHitResult Hit(1.f);
			SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

			//if blocking hit...
			if (Hit.Time < 1.f)
			{
				const FVector GravDir = FVector(0.f, 0.f, -1.f);
				const FVector VelDir = Velocity.GetSafeNormal();
				//Dot
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
					MovementMode = MOVE_Walking;

				}
			
			}
			//PhysFlying(DeltaTime, Iterations);
			
		}
					
	}


}


//Private-----------------

