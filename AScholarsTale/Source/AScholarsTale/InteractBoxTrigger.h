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
	//Ctor. Adds overlap bindings.
	UInteractBoxTrigger();

	//The interact event that will be added to the players
	//interact delegate during overlap. (Set by blueprint).
	UPROPERTY(BlueprintReadWrite, DisplayName="InteractEvent")
		FInteractCallbackSignature m_InteractEvent;


private:
	//Called on begin overlap, tries to add the m_InteractEvent to the players interact delegate.
	UFUNCTION()
		void OnTriggerBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	//Called on begin overlap, tries to remove the m_InteractEvent from the players interactt delegate.
	UFUNCTION()
		void OnTriggerEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

};
