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

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
	
	void UpdatePageTextures(uint32 FirstPageIndex);

	//should be const
	TArray<class UTexture2D *> *DetermineActiveTexSet();

	UFUNCTION(BlueprintCallable)
		void ReceiveOnOpenBook();

	UFUNCTION(BlueprintCallable)
		void ReceiveOnCloseBook();
	
	UFUNCTION(BlueprintCallable)
		void FlipPageForward();

	UFUNCTION(BlueprintCallable)
		void FlipPageBack();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnOpenBook();
			
	UFUNCTION(BlueprintImplementableEvent)
		void OnCloseBook();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnPageSelect(int32 PageForwardFlipCount);

	UFUNCTION()
		void ReceiveOnPageSelect();

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
	
	void PlayFlipAnim(bool bPlayForward);


	UPROPERTY(EditAnywhere)
		UAnimSequence *m_pAnimSequenceOpen;

	UPROPERTY(EditAnywhere)
		UAnimSequence *m_pAnimSequenceFlip;	

	UPROPERTY(VisibleDefaultsOnly)
		USkeletalMeshComponent *m_pSkelMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent *m_pTrigger;
	
	UPROPERTY(BlueprintReadOnly)
		bool m_bIsOpen;

	UPROPERTY(EditAnywhere)
		TArray<class UTexture2D *> m_aPageTextures;

	UPROPERTY(EditAnywhere)
		TArray<class UTexture2D *> m_aPageTexturesMineDone;

	UPROPERTY(EditAnywhere)
		TArray<class UTexture2D *> m_aPageTexturesOverride;

	UPROPERTY(EditAnywhere)
		FName m_SkelMaterialTargetParameterName;

	UPROPERTY(EditAnywhere)
		FName m_PageFrontMaterialTargetParameterName;

	UPROPERTY(EditAnywhere)
		FName m_PageBackMaterialTargetParameterName;
	
	UPROPERTY(EditAnywhere)
		float m_PageFlippedRoll;
	
	UPROPERTY(BlueprintReadOnly)
		bool m_bIsOverlapping;
	
	UPROPERTY()
		class UMaterialInstanceDynamic *m_pPageMat;
	
	UPROPERTY()
		class UMaterialInstanceDynamic *m_pPageBackMat;

	UPROPERTY()
		class UMaterialInstanceDynamic *m_pSkelFrontMat;

	UPROPERTY()
		class UMaterialInstanceDynamic *m_pSkelBackMat;

	FInteractCallbackSignature m_InteractDelegate;

	int32 m_PageForwardFlipCountCurrent;

	FTransform m_PageInitialTransform;

	float m_PageTargetRoll;

	const FName m_AnimSlotBody;

	const FName m_AnimSlotPage;
	
	int32 m_PageForwardFlipCountMax;

	const float ANIM_POSITION_END{ 1 };
	const float ANIM_POSITION_BEGIN{ 0 };
	const float ANIM_PLAYRATE_FORWARD{ 1 };
	const float ANIM_PLAYRATE_BACKWARD{ -1 };
	const float ANIM_BLEND_TIME{ 0 };



};
