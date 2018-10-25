#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerCapsule.h"
#include "Airstream.generated.h"


UCLASS(Blueprintable, CustomConstructor)
class ASCHOLARSTALE_API AAirstream : public ATriggerCapsule
{
	GENERATED_BODY()
	
public:
	AAirstream();

protected:
	UFUNCTION()
		void OnAirstreamBeginOverlap(AActor *OverlappedActor, AActor *OtherActor);

	UFUNCTION()
		void OnAirstreamEndOverlap(AActor *OverlappedActor, AActor *OtherActor);

	virtual void Tick(float DeltaTime) override;

	bool m_bIsOverlapping;

	UPROPERTY()
		class UASTCharacterMovementComponent *m_pMovementComp;

	UPROPERTY(EditAnywhere)
		float m_Drift{ 1800 };

	
};
