// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleballBase.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ATeleballBase::ATeleballBase()
{
	PrimaryActorTick.bCanEverTick = true;
	 	
	m_pProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	m_pProjectileMovement->Deactivate();

	
}

void ATeleballBase::Launch(FVector Velocity) 
{
	m_pProjectileMovement->Velocity = Velocity;
	m_pProjectileMovement->Activate(true);


}

// Called when the game starts or when spawned
void ATeleballBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleballBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ATeleballBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_pProjectileMovement->Deactivate();


}

void ATeleballBase::StopMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop"));
	m_pProjectileMovement->Deactivate();

}

