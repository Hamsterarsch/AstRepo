// Fill out your copyright notice in the Description page of Project Settings.

#include "StoryBook.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"

AStoryBook::AStoryBook()
{ 
	PrimaryActorTick.bCanEverTick = true;
	m_pSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Book"));
	//m_pSkelMesh->bEditableWhenInherited = true;
	SetRootComponent(m_pSkelMesh);
	
	m_pPageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Page"));
	m_pPageMesh->SetupAttachment(m_pSkelMesh);

	m_pTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	m_pTrigger->SetupAttachment(GetRootComponent());

	m_pTrigger->OnComponentBeginOverlap.AddDynamic(this, &AStoryBook::OnTriggerBeginOverlap);
	m_pTrigger->OnComponentEndOverlap.AddDynamic(this, &AStoryBook::OnTriggerEndOverlap);

	m_InteractDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AStoryBook, OnPageSelect));

}

void AStoryBook::StartOpenBook()
{
	m_bIsOpen = true;


}

void AStoryBook::StartCloseBook()
{
	m_bIsOpen = false;


}

void AStoryBook::OnPageSelect()
{
	UE_LOG(LogTemp, Log, TEXT("OnPage"));

}

void AStoryBook::OnTriggerBeginOverlap
(
	UPrimitiveComponent *OverlappedComponent,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult
)
{
	if (OtherActor->IsA<AASTCharacter>())
	{
		m_bIsOverlapping = true;
		StartOpenBook();
				
		Cast<AASTCharacter>(OtherActor)->AddInteraction(m_InteractDelegate);
	}


}

void AStoryBook::OnTriggerEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AASTCharacter>())
	{
		m_bIsOverlapping = false;
		StartCloseBook();

		Cast<AASTCharacter>(OtherActor)->RemoveInteraction(m_InteractDelegate);
		
	}


}


