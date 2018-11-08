// Fill out your copyright notice in the Description page of Project Settings.

#include "TeleballBase.h"
#include "AScholarsTale.h"
#include "GameFramework/ProjectileMovementComponent.h"


//Public-----------------------

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

void ATeleballBase::OnTeleballRecast()
{
	ReceiveOnTeleballRecast();
	UE_LOG(AST_Teleport, Log, TEXT("OnTelleballRecast::AfterBlueprint"));
	Destroy();


}

void ATeleballBase::OnTeleportTo()
{
	ReceiveOnTeleportTo(); 
	UE_LOG(AST_Teleport, Log, TEXT("OnTeleportTo::AfterBlueprint")); 
	Destroy();


}

//Protected--------------

void ATeleballBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATeleballBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ATeleballBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Ctor deactivate does not last
	m_pProjectileMovement->Deactivate();


}

void ATeleballBase::StopMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop"));
	m_pProjectileMovement->Deactivate();

}

