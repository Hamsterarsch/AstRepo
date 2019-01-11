// Fill out your copyright notice in the Description page of Project Settings.

#include "StoryBook.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"


//Public-------------------------

AStoryBook::AStoryBook() :
	m_PageForwardFlipCountCurrent{ 0 }
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

	m_InteractDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AStoryBook, ReceiveOnPageSelect));


}

void AStoryBook::BeginPlay()
{
	Super::BeginPlay();
	
	auto *PC{ GetWorld()->GetFirstPlayerController() };
	EnableInput(PC);
	
	if (InputComponent)
	{
		InputComponent->BindAction(TEXT("PreviousPage"), EInputEvent::IE_Pressed, this, &AStoryBook::FlipPageBack);
		InputComponent->BindAction(TEXT("NextPage"), EInputEvent::IE_Pressed, this, &AStoryBook::FlipPageForward);

	}


}


//Protected---------------------

void AStoryBook::ReceiveOnOpenBook()
{
	m_bIsOpen = true;
	OnOpenBook();
	

}

void AStoryBook::ReceiveOnCloseBook()
{
	m_bIsOpen = false;
	OnCloseBook();


}

void AStoryBook::FlipPageForward()
{
	if (m_PageForwardFlipCountCurrent < m_PageForwardFlipCountMax)
	{
		++m_PageForwardFlipCountCurrent;

	}

	//material changes
}

void AStoryBook::FlipPageBack()
{
	if (m_PageForwardFlipCountCurrent > 1)
	{
		--m_PageForwardFlipCountCurrent;

	}

	//material changes

}


void AStoryBook::ReceiveOnPageSelect()
{
	OnPageSelect(m_PageForwardFlipCountCurrent);


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
		ReceiveOnOpenBook();
				
		Cast<AASTCharacter>(OtherActor)->AddInteraction(m_InteractDelegate);
	}


}

void AStoryBook::OnTriggerEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AASTCharacter>())
	{
		m_bIsOverlapping = false;
		ReceiveOnCloseBook();

		Cast<AASTCharacter>(OtherActor)->RemoveInteraction(m_InteractDelegate);
		
	}


}


