#include "Rope.h"
#include "ASTCharacter.h"

//Public-----------------

ARope::ARope()
{
	OnActorBeginOverlap.AddDynamic(this, &ARope::OnRopeBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ARope::OnRopeEndOverlap);


}


//Protected--------------------

void ARope::OnRopeBeginOverlap(AActor *OverlappedActor, AActor *OtherActor)
{
	if( auto *Player = Cast< AASTCharacter >(OtherActor) )
	{
		Player->OnRopeEntered(this);

	}


}

void ARope::OnRopeEndOverlap(AActor *OverlappedActor, AActor *OtherActor)
{
	if (auto *Player = Cast< AASTCharacter >(OtherActor))
	{
		Player->OnRopeLeft();

	}


}


