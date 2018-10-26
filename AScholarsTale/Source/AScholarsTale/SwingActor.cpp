#include "SwingActor.h"
#include "Components/SphereComponent.h"

//Public----------------------

ASwingActor::ASwingActor()
{ 	
	PrimaryActorTick.bCanEverTick = true;

	m_pCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	SetRootComponent(m_pCollisionSphere);



}

void ASwingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASwingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

