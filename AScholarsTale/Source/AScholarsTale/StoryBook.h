// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASTCharacter.h"
#include "StoryBook.generated.h"


UCLASS()
class ASCHOLARSTALE_API AStoryBook : public AActor
{
	GENERATED_BODY()
	
public:	
	AStoryBook();


protected:
	void StartOpenBook();
	
	void StartCloseBook();
		
	UFUNCTION()
		void OnPageSelect();

	UFUNCTION()
		void OnTriggerBeginOverlap
		(
			UPrimitiveComponent *OverlappedComponent,
			AActor *OtherActor,
			UPrimitiveComponent *OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult
		);

	UFUNCTION()
		void OnTriggerEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
	

	UPROPERTY(VisibleDefaultsOnly)
		USkeletalMeshComponent *m_pSkelMesh;

	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent *m_pPageMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent *m_pTrigger;
	
	UPROPERTY(BlueprintReadOnly)
		bool m_bIsOpen;

	bool m_bIsOverlapping;

	UPROPERTY(BlueprintReadWrite)
		FInteractCallbackSignature m_InteractDelegate;


};
