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

void UInteractBoxTrigger::BroadcastInteractEvent()
{
	if (m_InteractEvent.IsBound())
	{
		m_InteractEvent.Broadcast();

	}
	else
	{
		UE_LOG(AST_Interact, Warning, TEXT("Interact box %s attached to %s has no interact event on broadcast."), *GetName(), *GetAttachParent()->GetName());

	}


}

//Protected------------

void UInteractBoxTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_InteractEvent.IsBound())
	{
		m_InteractDelegateWrapped.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UInteractBoxTrigger, BroadcastInteractEvent));

		if (!m_InteractDelegateWrapped.IsBound())
		{
			UE_LOG(AST_Interact, Error, TEXT("UInteractBoxTrigger:: Could not bind interact event to broadcast function."));

		}
	
	}
	else
	{
		UE_LOG(AST_Interact, Warning, TEXT("Interact box %s attached to %s has no interact event on construction."), *GetName(), *GetAttachmentRoot()->GetName());

	}

	
}

//Private--------------

void UInteractBoxTrigger::OnTriggerBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//If overlapped was player...
	if (auto *Player = Cast<AASTCharacter>(OtherActor))
	{		
		//...add to interact delegate if bound.
		if (m_InteractDelegateWrapped.IsBound())
		{
			UE_LOG(AST_Interact, Log, TEXT("UInteractBoxTrigger:: Try add bound delegate wrapper."));
			Player->AddInteraction(m_InteractDelegateWrapped);
					
		}
		
	}


}

void UInteractBoxTrigger::OnTriggerEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	//If other actor was player...
	if (auto *Player = Cast<AASTCharacter>(OtherActor))
	{
		//...remove event from interact delegate.
		UE_LOG(AST_LogInfo, Log, TEXT("UInteractBoxTrigger:: Try remove bound delegate wrapper."));
		Player->RemoveInteraction(m_InteractDelegateWrapped);

	}


}