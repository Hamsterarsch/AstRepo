// Fill out your copyright notice in the Description page of Project Settings.
#include "InteractBoxTrigger.h"


//Public--------------

UInteractBoxTrigger::UInteractBoxTrigger()
{
	PrimaryComponentTick.bCanEverTick = false;
	//m_pTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));

	OnComponentBeginOverlap.AddDynamic(this, &UInteractBoxTrigger::OnTriggerBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractBoxTrigger::OnTriggerEndOverlap);
	//m_pTrigger->OnComponentEndOverlap


}

void UInteractBoxTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}


//Protected---------------------------

void UInteractBoxTrigger::BeginPlay()
{
	Super::BeginPlay();

	
}


//Private--------------

void UInteractBoxTrigger::OnTriggerBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (auto *Player = Cast<AASTCharacter>(OtherActor))
	{		
		UE_LOG(LogTemp, Warning, TEXT("UInteractBoxTrigger:: Try add"));
		if (m_InteractCallback.IsBound())
		{
			Player->AddInteraction(m_InteractCallback);
		
		}
		
	}


}

void UInteractBoxTrigger::OnTriggerEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (auto *Player = Cast<AASTCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("UInteractBoxTrigger:: Try remove"));
		Player->RemoveInteraction(m_InteractCallback);

	}

}