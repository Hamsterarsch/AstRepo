// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlickeringLightBase.generated.h"

UCLASS()
class ASCHOLARSTALE_API AFlickeringLightBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlickeringLightBase();


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	
};
