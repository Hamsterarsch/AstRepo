// Fill out your copyright notice in the Description page of Project Settings.

#include "StoryBook.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "Engine/Classes/Animation/AnimSingleNodeInstance.h"
#include "ASTGameInstance.h"


//Public-------------------------

AStoryBook::AStoryBook() :
	m_PageForwardFlipCountCurrent{ 0 },
	m_AnimSlotBody{ TEXT("SlotBody") },
	m_AnimSlotPage{ TEXT("SlotPage") }
{ 
	PrimaryActorTick.bCanEverTick = true;
	m_pSkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Book"));
	//m_pSkelMesh->bEditableWhenInherited = true;
	SetRootComponent(m_pSkelMesh);
	
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
			m_pSkelBackMat = m_pSkelMesh->CreateDynamicMaterialInstance(2, pSourceMat);
			m_pSkelMesh->SetMaterial(2, m_pSkelBackMat);
		}

		if (auto *pSourceMat{ m_pSkelMesh->GetMaterial(3) })
		{
			m_pPageMat = m_pSkelMesh->CreateDynamicMaterialInstance(3, pSourceMat);
			m_pSkelMesh->SetMaterial(3, m_pPageMat);
		}
	}

	UpdatePageTextures(0);
	
	if (m_pAnimSequenceOpen)
	{
		//init close
		m_pSkelMesh->PlayAnimation(m_pAnimSequenceOpen,	false);
		m_pSkelMesh->bPauseAnims = true;
		
	}


}

void AStoryBook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	auto CurrentRotator{ m_pPageMesh->GetRelativeTransform().GetRotation().Rotator() };
	auto NewRoll{ FMath::FInterpTo(CurrentRotator.Pitch, m_PageTargetRoll, DeltaTime, 12) };
	CurrentRotator.Pitch = NewRoll;
	m_pPageMesh->SetRelativeRotation(CurrentRotator);
	*/

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
	if (m_pAnimSequenceOpen)
	{
		m_pSkelMesh->PlayAnimation(m_pAnimSequenceOpen, false);
		m_pSkelMesh->SetPlayRate(1);
		m_pSkelMesh->bPauseAnims = false;

		/*
		m_pSkelMesh->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage
		(
			m_pAnimSequenceOpen,
			m_AnimSlotBody,
			ANIM_BLEND_TIME,
			ANIM_BLEND_TIME
		);*/

	}
	OnOpenBook();
	

}

void AStoryBook::ReceiveOnCloseBook()
{
	if (m_pAnimSequenceOpen)
	{
		if (auto *pInst{ Cast<UAnimSingleNodeInstance>(m_pSkelMesh->GetAnimInstance()) })
		{
			m_pSkelMesh->PlayAnimation(m_pAnimSequenceOpen, false);
			pInst->SetPosition(1, false);		
			m_pSkelMesh->SetPlayRate(-1);
			m_pSkelMesh->bPauseAnims = false;
		}
	}
	OnCloseBook();


}

void AStoryBook::UpdatePageTextures(const uint32 FirstPageIndex)
{
#if WITH_EDITOR
	if (!m_pSkelBackMat || !m_pSkelFrontMat || !m_pPageMat)
	{
		return;
	}
#endif

	TArray<class UTexture2D *> *paActiveTextureSet;
	auto *pGI{ Cast<UASTGameInstance>(GetGameInstance()) };

	if (m_aPageTexturesOverride.Num() > 0)
	{
		paActiveTextureSet = &m_aPageTexturesOverride;
	}
	else if( pGI && pGI->IsMineLevelDone())
	{
		paActiveTextureSet = &m_aPageTexturesMineDone;
	}
	else
	{
		paActiveTextureSet = &m_aPageTextures;
	}
	
	if (static_cast<uint32>(paActiveTextureSet->Num()) >= (FirstPageIndex + 4))
	{
		
		m_pSkelFrontMat->SetTextureParameterValue(m_SkelMaterialTargetParameterName,	(*paActiveTextureSet)[FirstPageIndex    ]);
		m_pPageMat->SetTextureParameterValue(m_PageFrontMaterialTargetParameterName,	(*paActiveTextureSet)[FirstPageIndex + 1]);
		m_pPageMat->SetTextureParameterValue(m_PageBackMaterialTargetParameterName,		(*paActiveTextureSet)[FirstPageIndex + 2]);
		m_pSkelBackMat->SetTextureParameterValue(m_SkelMaterialTargetParameterName,		(*paActiveTextureSet)[FirstPageIndex + 3]);
	}


}

void AStoryBook::FlipPageForward()
{
	if (m_PageForwardFlipCountCurrent >= m_PageForwardFlipCountMax)
	{
		return;
	}
	++m_PageForwardFlipCountCurrent;

	PlayFlipAnim(true);

	
	if( m_PageForwardFlipCountCurrent % 2 )
	{
		//roll lerp
		m_PageTargetRoll = m_PageFlippedRoll;
	}
	else
	{

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

	PlayFlipAnim(false);
	if (m_PageForwardFlipCountCurrent % 2)
	{
		//Roll lerp
		m_PageTargetRoll = m_PageInitialTransform.Rotator().Pitch;// m_PageInitialTransform.Rotator().Pitch;
	}
	else
	{
		auto Rotator{ m_PageInitialTransform.Rotator() };
		Rotator.Pitch = m_PageFlippedRoll;
		//m_pPageMesh->SetRelativeRotation(Rotator);
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

void AStoryBook::PlayFlipAnim(bool bPlayForward)
{
	if (!m_pAnimSequenceFlip)
	{
		return;
	}

	if (bPlayForward)
	{
		m_pSkelMesh->PlayAnimation(m_pAnimSequenceFlip, false);
		m_pSkelMesh->SetPlayRate(1);
		m_pSkelMesh->bPauseAnims = false;

		/*
		m_pSkelMesh->GetAnimInstance()->Montage_Stop(0);
		m_pSkelMesh->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage
		(
			m_pAnimSequenceFlip,
			m_AnimSlotPage,
			ANIM_BLEND_TIME,
			ANIM_BLEND_TIME
		);
		*/
	}
	else
	{
		m_pSkelMesh->PlayAnimation(m_pAnimSequenceFlip, false);
		Cast<UAnimSingleNodeInstance>(m_pSkelMesh->GetAnimInstance())->SetPosition(1, false);
		m_pSkelMesh->SetPlayRate(-1);
		m_pSkelMesh->bPauseAnims = false;

		/*
		m_pSkelMesh->GetAnimInstance()->Montage_Stop(0);
		m_pSkelMesh->GetAnimInstance()->PlaySlotAnimationAsDynamicMontage
		(
			m_pAnimSequenceFlip,
			m_AnimSlotPage,
			ANIM_BLEND_TIME,
			ANIM_BLEND_TIME,
			ANIM_PLAYRATE_BACKWARD,
			1,
			-1,
			ANIM_POSITION_END
		);
		*/
	}
	

}

