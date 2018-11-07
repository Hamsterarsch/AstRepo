//

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectSlinger.generated.h"


UCLASS( ClassGroup=(Custom), Meta=(BlueprintSpawnableComponent) )
class ASCHOLARSTALE_API UObjectSlinger : public UActorComponent
{
	GENERATED_BODY()

public:	
	UObjectSlinger();

	void SlingActor(class ISlingable *pActor, FVector &Strength)
	{
		//launch
		//pActor->Launch

	}

protected:
	virtual void BeginPlay() override;


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

			
};
