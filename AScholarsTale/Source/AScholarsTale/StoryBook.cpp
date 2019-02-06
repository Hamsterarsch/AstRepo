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

void AStoryBook::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	   
	m_PageForwardFlipCountMax = FMath::RoundToInt(m_aPageTextures.Num() % 4);
	m_PageForwardFlipCountMax = m_PageForwardFlipCountMax == 0 ? 1 : m_PageForwardFlipCountMax;

	UE_LOG(LogTemp, Log, TEXT("Book max flip: %i"), m_PageForwardFlipCountMax);

	if (m_pSkelMesh)
	{
		if (auto *pSourceMat{ m_pSkelMesh->GetMaterial(1) })
		{
			m_pSkelFrontMat = m_pSkelMesh->CreateDynamicMaterialInstance(1, pSourceMat);
			m_pSkelMesh->SetMaterial(1, m_pSkelFrontMat);
		}

		if (auto *pSourceMat{ m_pSkelMesh->GetMaterial(2) })
		{
			m_pSkelBackMat = m_pPageMesh->CreateDynamicMaterialInstance(2, pSourceMat);
			m_pSkelMesh->SetMaterial(2, m_pSkelBackMat);
		}

	}

	if (m_pPageMesh)
	{	
		m_PageInitialTransform = m_pPageMesh->GetRelativeTransform();
		m_PageTargetRoll = m_PageInitialTransform.GetRotation().Rotator().Pitch;

		if ( auto *pSourceMat{ m_pPageMesh->GetMaterial(0) } )
		{
			m_pPageMat = m_pPageMesh->CreateDynamicMaterialInstance(0, pSourceMat);
		}
	}

	UpdatePageTextures(0);


}

void AStoryBook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	auto CurrentRotator{ m_pPageMesh->GetRelativeTransform().GetRotation().Rotator() };
	auto NewRoll{ FMath::FInterpTo(CurrentRotator.Pitch, m_PageTargetRoll, DeltaTime, 12) };
	CurrentRotator.Pitch = NewRoll;
	m_pPageMesh->SetRelativeRotation(CurrentRotator);


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
	m_PageTargetRoll = m_PageInitialTransform.Rotator().Pitch;
	OnCloseBook();


}

void AStoryBook::UpdatePageTextures(const uint32 FirstPageIndex)
{
	if (static_cast<uint32>(m_aPageTextures.Num()) >= (FirstPageIndex + 4))
	{
		m_pSkelFrontMat->SetTextureParameterValue(m_SkelMaterialTargetParameterName,	m_aPageTextures[FirstPageIndex    ]);
		m_pPageMat->SetTextureParameterValue(m_PageFrontMaterialTargetParameterName,	m_aPageTextures[FirstPageIndex + 1]);
		m_pPageMat->SetTextureParameterValue(m_PageBackMaterialTargetParameterName,		m_aPageTextures[FirstPageIndex + 2]);
		m_pSkelBackMat->SetTextureParameterValue(m_SkelMaterialTargetParameterName,		m_aPageTextures[FirstPageIndex + 3]);
	}

}

void AStoryBook::FlipPageForward()
{
	if (m_PageForwardFlipCountCurrent >= m_PageForwardFlipCountMax)
	{
		return;
	}
	++m_PageForwardFlipCountCurrent;

	if( m_PageForwardFlipCountCurrent % 2 )
	{
		//roll lerp
		m_PageTargetRoll = m_PageFlippedRoll;
	}
	else
	{
		//Reset page
		m_PageTargetRoll = m_PageFlippedRoll;// m_PageInitialTransform.Rotator().Pitch;
		m_pPageMesh->SetRelativeTransform(m_PageInitialTransform);

		//material changes
		UpdatePageTextures(m_PageForwardFlipCountCurrent);		
	}


}

void AStoryBook::FlipPageBack()
{
	if (m_PageForwardFlipCountCurrent < 1)
	{
		return;
	}
	--m_PageForwardFlipCountCurrent;

	if (m_PageForwardFlipCountCurrent % 2)
	{
		//Roll lerp
		m_PageTargetRoll = m_PageInitialTransform.Rotator().Pitch;// m_PageInitialTransform.Rotator().Pitch;
	}
	else
	{
		auto Rotator{ m_PageInitialTransform.Rotator() };
		Rotator.Pitch = m_PageFlippedRoll;
		m_pPageMesh->SetRelativeRotation(Rotator);
		m_PageTargetRoll = m_PageInitialTransform.Rotator().Pitch;

		//material changes
		UpdatePageTextures(m_PageForwardFlipCountCurrent);			   
	}


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


