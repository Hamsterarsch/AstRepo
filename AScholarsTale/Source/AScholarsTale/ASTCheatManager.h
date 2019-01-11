// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "ASTCheatManager.generated.h"

/**
 * 
 */
UCLASS(Within = PlayerController)
class ASCHOLARSTALE_API UASTCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	virtual void InitCheatManager() override;

	UFUNCTION(exec)
		void SetMineLevelToDone();

	UFUNCTION(exec)
		void SetCanCharacterJump(bool bIsAllowed);

	UFUNCTION(exec)
		void SetCanCharacterGlide(bool bIsAllowed);
	

protected:
	UPROPERTY()
		class UASTGameInstance *pGameInstance;


	
	
};
