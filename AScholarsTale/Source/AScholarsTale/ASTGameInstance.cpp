// Fill out your copyright notice in the Description page of Project Settings.

#include "ASTGameInstance.h"
#include "ASTSaveGame.h"
#include "AScholarsTale.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Level.h"



void UASTGameInstance::SaveGame()
{
	auto *pSavegame{ static_cast<UASTSaveGame *>(UGameplayStatics::CreateSaveGameObject( UASTSaveGame::StaticClass() )) };	
	if ( !pSavegame )
	{
		UE_LOG(AST_Error, Error, TEXT("Save failed: could not create save game object"));
		return;
	}

	auto *pPC{ GetWorld()->GetFirstPlayerController() };
	if(!pPC)
	{
		UE_LOG(AST_Error, Error, TEXT("Save failed: could not fetch player controller"));
		return;
	}

	auto *pPlayer{ pPC->GetPawn() };
	if (!pPlayer)
	{
		UE_LOG(AST_Error, Error, TEXT("Save failed: could not fetch player"));
		return;
	}
	
	pSavegame->m_bIsMineLevelCompleted = m_bIsMineLevelDone;
	pSavegame->m_bIsCanyonLevelCompleted = m_bIsCanyonLevelDone;
	pSavegame->m_CurrentPlayerTransform = pPlayer->GetActorTransform();
	pSavegame->m_CurrentLevelName = UGameplayStatics::GetCurrentLevelName(pPlayer->GetWorld(), true);//->GetLevel()->GetPathName(nullptr);
	
	m_OnSaveGame.Broadcast(pSavegame);

	if ( !UGameplayStatics::SaveGameToSlot(pSavegame, pSavegame->GetSlotName(), pSavegame->GetUserIndex()) )
	{
		UE_LOG(AST_Error, Error, TEXT("Save failed: could not save game to slot"));
		return;
	}


}

void UASTGameInstance::LoadGame()
{
	auto *pSavegame{ static_cast<UASTSaveGame *>(UGameplayStatics::CreateSaveGameObject(UASTSaveGame::StaticClass())) };
	
	pSavegame = static_cast<UASTSaveGame *>(UGameplayStatics::LoadGameFromSlot(pSavegame->GetSlotName(), pSavegame->GetUserIndex()));
	if( !pSavegame )
	{
		UE_LOG(AST_Error, Error, TEXT("Load save failed: could not load game from slot"));
		return;
	}

	m_bIsMineLevelDone = pSavegame->m_bIsMineLevelCompleted;
	m_bIsCanyonLevelDone = pSavegame->m_bIsCanyonLevelCompleted;
	
	
	UGameplayStatics::OpenLevel(GetWorld(), *(pSavegame->m_CurrentLevelName));
	auto *pPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	//auto Location = pSavegame->m_CurrentPlayerTransform.GetLocation();
	
	pPawn->SetActorTransform(pSavegame->m_CurrentPlayerTransform);
	//pPawn->TeleportTo(Location, pSavegame->m_CurrentPlayerTransform.GetRotation().Rotator());
	
	m_pLoadedSavegame = pSavegame;



}

bool UASTGameInstance::IsSavegameAvailable() const
{
	auto *pSavegame{ static_cast<UASTSaveGame *>(UGameplayStatics::CreateSaveGameObject(UASTSaveGame::StaticClass())) };

	pSavegame = static_cast<UASTSaveGame *>(UGameplayStatics::LoadGameFromSlot(pSavegame->GetSlotName(), pSavegame->GetUserIndex()));
	
	//todo: could lead do save overwrites on load fails not caused by not existent files
	return pSavegame ? true : false;
			
	
}

void UASTGameInstance::LoadComplete(float LoadTime, const FString &MapName)
{
	Super::LoadComplete(LoadTime, MapName);

	if (m_pLoadedSavegame)
	{		
		m_OnLoadGame.Broadcast(m_pLoadedSavegame);
		m_pLoadedSavegame = nullptr;

	}


}
