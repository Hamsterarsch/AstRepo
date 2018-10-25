#include "Airstream.h"
#include "ASTCharacter.h"



//Public--------------

AAirstream::AAirstream()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = .125f;

	OnActorBeginOverlap.AddDynamic(this, &AAirstream::OnAirstreamBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AAirstream::OnAirstreamEndOverlap);
	

}

//Protected--------------------

void AAirstream::OnAirstreamBeginOverlap(AActor *OverlappedActor, AActor *OtherActor)
{	
	if (auto *Player = Cast<AASTCharacter>(OtherActor))
	{
		m_pMovementComp = Cast< std::remove_pointer_t<decltype(m_pMovementComp)> >(Player->GetMovementComponent());
		if (m_pMovementComp)
		{
			UE_LOG(LogTemp, Log, TEXT("Begin"));
			m_bIsOverlapping = true;
			m_pMovementComp->AddGlidingForceOffset({ 0, 0, m_Drift });
		}

	}


}

void AAirstream::OnAirstreamEndOverlap(AActor *OverlappedActor, AActor *OtherActor)
{
	if ( OtherActor->IsA< AASTCharacter >() )
	{
		m_bIsOverlapping = false;
		if (m_pMovementComp)
		{
			UE_LOG(LogTemp, Log, TEXT("End"));
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

