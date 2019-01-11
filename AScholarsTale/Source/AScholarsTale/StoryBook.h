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
	virtual void BeginPlay() override;

	void FlipPageForward();

	void FlipPageBack();

	void ReceiveOnOpenBook();

	void ReceiveOnCloseBook();
	
	UFUNCTION()
		void ReceiveOnPageSelect();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnOpenBook();
			
	UFUNCTION(BlueprintImplementableEvent)
		void OnCloseBook();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnPageSelect(int32 PageForwardFlipCount);

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

	
		FInteractCallbackSignature m_InteractDelegate;

	UPROPERTY(EditDefaultsOnly)
		int32 m_PageForwardFlipCountMax;

	int32 m_PageForwardFlipCountCurrent;
	
	bool m_bIsOverlapping;



};
