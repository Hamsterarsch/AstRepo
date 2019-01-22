// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NarratorTrigger.generated.h"

UCLASS()
class ASCHOLARSTALE_API ANarratorTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	UFUNCTION()
		void ReceiveOnSaveGame(class UASTSaveGame *pSavegame) { OnSaveGame(pSavegame); }
	UFUNCTION()
		void ReceiveOnLoadGame(const class UASTSaveGame *pSavegame) { OnLoadGame(pSavegame); }


protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
		void OnSaveGame(class UASTSaveGame *pSavegame);

	UFUNCTION(BlueprintImplementableEvent)
		void OnLoadGame(const class UASTSaveGame *pSavegame);
	
	TScriptDelegate<> LoadDelegate, SaveDelegate;

};
