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
		void OnAirstreamBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnAirstreamEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	virtual void Tick(float DeltaTime) override;

	bool m_bIsOverlapping;

	UPROPERTY()
		class UASTCharacterMovementComponent *m_pMovementComp;

	UPROPERTY(EditAnywhere)
		float m_Drift{ 1800 };

	
};
