#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "Rope.generated.h"


UCLASS()
class ASCHOLARSTALE_API ARope : public ATriggerSphere
{
	GENERATED_BODY()
	
public:
	ARope();


protected:
	UFUNCTION()
		void OnRopeBeginOverlap(AActor *OverlappedActor, AActor *OtherActor);

	UFUNCTION()
		void OnRopeEndOverlap(AActor *OverlappedActor, AActor *OtherActor);

	
};
