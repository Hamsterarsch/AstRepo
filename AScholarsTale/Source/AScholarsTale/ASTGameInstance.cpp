// Fill out your copyright notice in the Description page of Project Settings.

#include "ASTGameInstance.h"
#include "ASTSaveGame.h"
#include "AScholarsTale.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes//Camera/CameraComponent.h"
#include "ASTBlueprintLib.h"
#include "Engine/Level.h"
#include "ASTCharacter.h"



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

	auto *pPlayer{ Cast<AASTCharacter>(pPC->GetPawn()) };
	if (!pPlayer)
	{
		UE_LOG(AST_Error, Error, TEXT("Save failed: could not fetch player"));
		return;
	}
	
	pSavegame->m_bIsMineLevelCompleted = m_bIsMineLevelDone;
	pSavegame->m_bIsCanyonLevelCompleted = m_bIsCanyonLevelDone;
	pSavegame->m_CurrentPlayerTransform = pPlayer->GetActorTransform();
	pSavegame->m_CurrentLevelName = UGameplayStatics::GetCurrentLevelName(pPlayer->GetWorld(), true);//->GetLevel()->GetPathName(nullptr);
	pSavegame->m_bIsGlidingUnlocked = pPlayer->GetIsGlidingEnabled();

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

	/*
	auto *pPlayer = Cast<AASTCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//auto Location = pSavegame->m_CurrentPlayerTransform.GetLocation();
	//has to be done in levels when player is initialized ( actually only in player)
	pPlayer->SetActorTransform(pSavegame->m_CurrentPlayerTransform);
	if (pSavegame->m_bIsGlidingUnlocked)
	{
		pPlayer->EnableGliding();
	}
	else
	{
		pPlayer->DisableGliding();

	}
	//pPawn->TeleportTo(Location, pSavegame->m_CurrentPlayerTransform.GetRotation().Rotator());
	*/

	m_pLoadedSavegame = pSavegame;



}

bool UASTGameInstance::IsSavegameAvailable() const
{
	auto *pSavegame{ static_cast<UASTSaveGame *>(UGameplayStatics::CreateSaveGameObject(UASTSaveGame::StaticClass())) };

	pSavegame = static_cast<UASTSaveGame *>(UGameplayStatics::LoadGameFromSlot(pSavegame->GetSlotName(), pSavegame->GetUserIndex()));
	
	//todo: could lead do save overwrites on load fails not caused by not existent files
	return pSavegame ? true : false;
			
	
}

void UASTGameInstance::ApplyGameMetadata(class USoundClass *pMusicClass, USoundClass *pSfxClass, USoundClass *pDialogueClass, const FGameMetadata &Metadata, UCameraComponent *pCameraComp)
{
	auto BiasedGamma = Metadata.Gamma * 2;
	pCameraComp->PostProcessSettings.bOverride_ColorGamma = true;
	pCameraComp->PostProcessSettings.ColorGamma = FVector4(BiasedGamma, BiasedGamma, BiasedGamma);
	UASTBlueprintLib::SetVolumeMultiplier(pMusicClass, Metadata.VolumeMusicMult * 2 * m_MusicVolume);
	UASTBlueprintLib::SetVolumeMultiplier(pSfxClass, Metadata.VolumeSfxMult * 2 * m_SFXVolume);
	UASTBlueprintLib::SetVolumeMultiplier(pDialogueClass, Metadata.VolumeDialogueMult * 2 * m_DialogueVolume);
	

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
