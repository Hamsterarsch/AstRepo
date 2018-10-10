// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "ASTCharacter.h"
#include "InteractBoxTrigger.generated.h"


UCLASS(ClassGroup=(Custom), Meta=(BlueprintSpawnableComponent))
class ASCHOLARSTALE_API UInteractBoxTrigger : public UBoxComponent
{
	GENERATED_BODY()

public:
	UInteractBoxTrigger();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, DisplayName="InteractEvent")
		FInteractCallbackSignature m_InteractCallback;


protected:
	virtual void BeginPlay() override;
	
	
private:
	UFUNCTION()
		void OnTriggerBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	UFUNCTION()
		void OnTriggerEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

};
