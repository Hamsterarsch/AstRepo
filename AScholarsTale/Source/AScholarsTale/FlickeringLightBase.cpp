// Fill out your copyright notice in the Description page of Project Settings.

#include "FlickeringLightBase.h"
#include "Components/PointLightComponent.h"


AFlickeringLightBase::AFlickeringLightBase()
{
	PrimaryActorTick.bCanEverTick = true;
	


}

void AFlickeringLightBase::BeginPlay()
{
	Super::BeginPlay();
	

}

void AFlickeringLightBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AFlickeringLightBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	

}
