#include "Airstream.h"
#include "ASTCharacter.h"



//Public--------------

AAirstream::AAirstream()
{
	OnActorBeginOverlap.AddDynamic(this, &AAirstream::OnAirstreamBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AAirstream::OnAirstreamEndOverlap);
	

}

//Protected--------------------

void AAirstream::OnAirstreamBeginOverlap(AActor *OverlappedActor, AActor *OtherActor)
{	
	m_pCharacter = Cast< std::remove_pointer_t<decltype(m_pCharacter)> >(OtherActor);
	if ( m_pCharacter )
	{
		m_bIsOverlapping = true;
		

	}


}

void AAirstream::OnAirstreamEndOverlap(AActor *OverlappedActor, AActor *OtherActor)
{
	if ( OtherActor->IsA< std::remove_pointer_t<decltype(m_pCharacter)> >() )
	{
		m_bIsOverlapping = false;

	}


}

void AAirstream::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bIsOverlapping)
	{
		m_pCharacter->GetCharacterMovement()->AddForce({ 0,0, m_Drift });

	}


}

