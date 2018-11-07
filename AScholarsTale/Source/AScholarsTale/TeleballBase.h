// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Slingable.h"
#include "TeleballBase.generated.h"

UCLASS()
class ASCHOLARSTALE_API ATeleballBase : public AActor, public ISlingable
{
	GENERATED_BODY()
	
public:	
	ATeleballBase();

	virtual void Launch(FVector Velocity) override;
	

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
		void StopMovement();


	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent *m_pProjectileMovement;
	
	
};
