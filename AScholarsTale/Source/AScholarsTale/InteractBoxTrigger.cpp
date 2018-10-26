// Fill out your copyright notice in the Description page of Project Settings.
#include "InteractBoxTrigger.h"
#include "AScholarsTale.h"


//Public--------------

UInteractBoxTrigger::UInteractBoxTrigger()
{
	PrimaryComponentTick.bCanEverTick = false;

	//Overlap bindings.
	OnComponentBeginOverlap.AddDynamic(this, &UInteractBoxTrigger::OnTriggerBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractBoxTrigger::OnTriggerEndOverlap);
	

}


//Private--------------

void UInteractBoxTrigger::OnTriggerBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//If overlapped was player...
	if (auto *Player = Cast<AASTCharacter>(OtherActor))
	{		
		UE_LOG(AST_LogInfo, Log, TEXT("UInteractBoxTrigger:: Try add"));
		//...add to interact delegate if bound.
		if (m_InteractEvent.IsBound())
		{
			Player->AddInteraction(m_InteractEvent);
		
		}
		
	}


}

void UInteractBoxTrigger::OnTriggerEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	//If other actor was player...
	if (auto *Player = Cast<AASTCharacter>(OtherActor))
	{
		//...remove event from interact delegate.
		UE_LOG(AST_LogInfo, Log, TEXT("UInteractBoxTrigger:: Try remove"));
		Player->RemoveInteraction(m_InteractEvent);

	}


}