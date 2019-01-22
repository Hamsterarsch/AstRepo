// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ASTGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameDelegate, class UASTSaveGame *, pSavegame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadGameDelegate, const UASTSaveGame *, pSavegame);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLoadGameBPEvent, const UASTSaveGame *, pSavegame);

/**
 * 
 */
UCLASS()
class ASCHOLARSTALE_API UASTGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void SetCanyonLevelDone() { m_bIsCanyonLevelDone = true; }

	UFUNCTION(BlueprintCallable)
		void SetMineLevelDone() { m_bIsMineLevelDone = true; }

	UFUNCTION(BlueprintCallable)
		bool IsCanyonLevelDone() const { return m_bIsCanyonLevelDone; }

	UFUNCTION(BlueprintCallable)
		bool IsMineLevelDone() const { return m_bIsMineLevelDone; }

	UFUNCTION(BlueprintCallable)
		void SaveGame();

	UFUNCTION(BlueprintCallable)
		void LoadGame();

	UFUNCTION(BlueprintCallable)
		void AddOnLoadEvent(FOnLoadGameBPEvent Event) { m_OnLoadGame.Add(Event); }

	FOnSaveGameDelegate m_OnSaveGame;

	FOnLoadGameDelegate m_OnLoadGame;


protected:
	virtual void LoadComplete(float LoadTime, const FString &MapName) override;


private:

	bool m_bIsCanyonLevelDone;
	bool m_bIsMineLevelDone;

	UPROPERTY()
		class UASTSaveGame *m_pLoadedSavegame;

	
};
