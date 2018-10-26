#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwingActor.generated.h"


UCLASS()
class ASCHOLARSTALE_API ASwingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASwingActor();


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
		class USphereComponent  *m_pCollisionSphere;

	
};
