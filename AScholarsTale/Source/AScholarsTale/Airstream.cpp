#include "Airstream.h"
#include "ASTCharacter.h"
#include "Components/ShapeComponent.h"



//Public--------------

AAirstream::AAirstream()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = .125f;

	SetActorHiddenInGame(false);

	GetCollisionComponent()->bHiddenInGame = true;
	GetCollisionComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAirstream::OnAirstreamBeginOverlap);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &AAirstream::OnAirstreamEndOverlap);
	

}

//Protected--------------------

void AAirstream::OnAirstreamBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{	
	UE_LOG(LogTemp, Log, TEXT("Begin"));
	if (auto *Player = Cast<AASTCharacter>(OtherActor))
	{
		m_pMovementComp = Cast< std::remove_pointer_t<decltype(m_pMovementComp)> >(Player->GetMovementComponent());
		if (m_pMovementComp)
		{
			UE_LOG(LogTemp, Log, TEXT("Exec force add"));
			m_bIsOverlapping = true;
			m_pMovementComp->AddGlidingForceOffset({ 0, 0, m_Drift });
		}

	}


}

void AAirstream::OnAirstreamEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("End"));
	if ( OtherActor->IsA< AASTCharacter >() )
	{
		m_bIsOverlapping = false;
		if (m_pMovementComp)
		{
			UE_LOG(LogTemp, Log, TEXT("Exec Force remove"));
			m_pMovementComp->AddGlidingForceOffset({ 0,0, -m_Drift });

		}

	}


}

void AAirstream::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bIsOverlapping)
	{
		

	}


}

