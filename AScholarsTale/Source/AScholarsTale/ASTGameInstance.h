// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ASTGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ASCHOLARSTALE_API UASTGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		void SetCanyonLevelDone() noexcept { m_bIsCanyonLevelDone = true; }

	UFUNCTION(BlueprintCallable)
		void SetMineLevelDone() noexcept { m_bIsMineLevelDone = true; }

	UFUNCTION(BlueprintCallable)
		bool IsCanyonLevelDone() const noexcept { return m_bIsCanyonLevelDone; }

	UFUNCTION(BlueprintCallable)
		bool IsMineLevelDone() const noexcept { return m_bIsMineLevelDone; }


private:
	bool m_bIsCanyonLevelDone;
	bool m_bIsMineLevelDone;
	   
	
};
