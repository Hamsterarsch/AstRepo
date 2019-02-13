// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ASTSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ASCHOLARSTALE_API UASTSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	FString GetSlotName() const { return TEXT("ASTSavegame"); }
	uint32 GetUserIndex() const { return 0; }

	UPROPERTY(BlueprintReadWrite)
		bool m_bIsMineLevelCompleted;

	UPROPERTY(BlueprintReadWrite)
		bool m_bIsCanyonLevelCompleted;

	UPROPERTY(BlueprintReadWrite)
		FString m_CurrentLevelName;

	UPROPERTY(BlueprintReadWrite)
		FTransform m_CurrentPlayerTransform;
	
	UPROPERTY(BlueprintReadWrite)
		TMap<FString, bool> m_NarratorMap;

	UPROPERTY(BlueprintReadWrite)
		bool m_bIsGlidingUnlocked{ false };
	



};
