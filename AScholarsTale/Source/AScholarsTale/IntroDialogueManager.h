// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IntroDialogueManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialoguePartStartedDelegate, const FString &, NewText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVoidDelegateSignature);

UCLASS()
class ASCHOLARSTALE_API AIntroDialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AIntroDialogueManager();

	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintAssignable)
		FOnDialoguePartStartedDelegate m_OnDialoguePartStarted;

	UPROPERTY(BlueprintAssignable)
		FVoidDelegateSignature m_OnDialoguesFinished;


protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
		void PlayNextDialogue();

	UFUNCTION()
		void ReceiveOnDialoguesFinished();
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnDialoguesFinished();

	UFUNCTION(BlueprintCallable)
		void StartPlayDialogues();

	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<class UObjectLibrary> m_DialogueLibrary;

	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<class UDialogueVoice> m_SpeakerVoice; 
	
	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<class UDialogueVoice> m_TargetVoice;
	
	UPROPERTY()
		TArray<class UDialogueWave *> m_apDialogueWaves;

	UPROPERTY()
		TArray<class UAudioComponent *> m_apSpawnedDialogues;

	uint32 m_NumPlayedDialogues;

	
	
};
