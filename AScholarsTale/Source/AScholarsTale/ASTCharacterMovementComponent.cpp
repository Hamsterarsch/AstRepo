// Fill out your copyright notice in the Description page of Project Settings.
#include "ASTCharacterMovementComponent.h"


//Public-------------------



//Protected------------------
void UASTCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
{
	if (CharacterOwner)
	{		
		auto CustomMode = (ASTMovementMode)CustomMovementMode;
		if (CustomMode == decltype(CustomMode)::Gliding)
		{
			UE_LOG(LogTemp, Warning, TEXT("UASTCharacterCustomMovementComponent:: GlidingMode"));

		}

			
	}


}


//Private-----------------

