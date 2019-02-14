// Fill out your copyright notice in the Description page of Project Settings.

#include "ASTCheatManager.h"
#include "ASTGameInstance.h"
#include "Engine/World.h"
#include "ASTPlayerController.h"
#include "ASTCharacter.h"

//Public----------------

void UASTCheatManager::InitCheatManager()
{
	Super::InitCheatManager();

	auto *pWorld{ GetWorld() };
	check(pWorld);

	pGameInstance = Cast<UASTGameInstance>(pWorld->GetGameInstance());

	checkf(pGameInstance, TEXT("ASTCheatManager:: could not get ast game instance for cheat manager init"));
	

}

void UASTCheatManager::SetMineLevelToDone()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Test"));
	pGameInstance->SetMineLevelDone();


}

void UASTCheatManager::SetCanCharacterJump(bool bIsAllowed)
{
	auto *pWorld{ GetWorld() };
	if (!pWorld)
	{
		return;
	}

	auto *PC{ pWorld->GetFirstPlayerController() };
	if (!PC)
	{
		return;
	}

	if (auto *pChar{ Cast<AASTCharacter>(PC->GetPawn()) } )
	{
		if (bIsAllowed)
		{
			pChar->EnableJumping();
		}
		else
		{
			pChar->DisableJumping();
		}

	}


}

void UASTCheatManager::SetCanCharacterGlide(bool bIsAllowed)
{
	auto *pWorld{ GetWorld() };
	if (!pWorld)
	{
		return;
	}

	auto *PC{ pWorld->GetFirstPlayerController() };
	if (!PC)
	{
		return;
	}

	if (auto *pChar{ Cast<AASTCharacter>(PC->GetPawn()) })
	{
		if (bIsAllowed)
		{
			pChar->EnableGliding();
		}
		else
		{
			pChar->DisableGliding();
		}

	}


}

void UASTCheatManager::DebugSaveGame()
{
	if (pGameInstance)
	{
		pGameInstance->SaveGame();

	}


}

void UASTCheatManager::DebugLoadGame()
{
	if (pGameInstance)
	{
		pGameInstance->LoadGame();

	}

}
