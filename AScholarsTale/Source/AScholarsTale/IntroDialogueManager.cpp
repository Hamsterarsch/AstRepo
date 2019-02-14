// Fill out your copyright notice in the Description page of Project Settings.

#include "IntroDialogueManager.h"
#include "AScholarsTale.h"
#include "Engine/Classes/Sound/DialogueWave.h"
#include "Engine/Classes/Sound/DialogueVoice.h"
#include "Components/AudioComponent.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "ASTBlueprintLib.h"


//Public------------------------

AIntroDialogueManager::AIntroDialogueManager() :
	m_NumPlayedDialogues{ 0 }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AIntroDialogueManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//Protected----------------------

void AIntroDialogueManager::BeginPlay()
{
	Super::BeginPlay();

	if (!ShouldPlay())
	{
		return;

	}

	if (m_apDialogueWaves.Num() <= 0)
	{
		return;
	}

	FDialogueContext Context{};
	Context.Speaker = LoadObjectPtr(m_SpeakerVoice);
	Context.Targets = TArray<UDialogueVoice *>{ LoadObjectPtr(m_TargetVoice) };

	//Spawn all dialogues that should play and link their starts/ends
	m_apSpawnedDialogues.Add( UGameplayStatics::CreateSound2D(GetWorld(), m_apDialogueWaves[0]->GetWaveFromContext(Context), 1, 1, 0, nullptr, false, false) );
	m_apSpawnedDialogues[0]->bSuppressSubtitles = true;
	{
		//Play next dialogue delegate creation
		TScriptDelegate<> PlayNextDialogueDelegate{};
		PlayNextDialogueDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AIntroDialogueManager, PlayNextDialogue));
		
		for (size_t Index{ 1 }; Index < m_apDialogueWaves.Num(); ++Index)
		{
			m_apSpawnedDialogues.Add( UGameplayStatics::CreateSound2D(GetWorld(), m_apDialogueWaves[Index]->GetWaveFromContext(Context), 1, 1, 0, nullptr, false, false) );
			m_apSpawnedDialogues[Index]->bSuppressSubtitles = true;
			m_apSpawnedDialogues[Index-1]->OnAudioFinished.Add(PlayNextDialogueDelegate);

		}
		//Link the OnEnd function for blueprint access
		TScriptDelegate<> Delegate{};
		Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(AIntroDialogueManager, ReceiveOnDialoguesFinished));

		m_apSpawnedDialogues.Last()->OnAudioFinished.Add(Delegate);
		
	}


}

void AIntroDialogueManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	m_apDialogueWaves = UASTBlueprintLib::LoadObjectLibraryTyped<UDialogueWave *>(m_DialogueLibrary.ToSoftObjectPath().GetAssetPathString());
	m_apSpawnedDialogues.Reserve(m_apDialogueWaves.Num());


}

void AIntroDialogueManager::PlayNextDialogue()
{
	++m_NumPlayedDialogues;
	m_apSpawnedDialogues[m_NumPlayedDialogues]->Play();

	m_OnDialoguePartStarted.Broadcast(m_apDialogueWaves[m_NumPlayedDialogues]->SpokenText);


}

void AIntroDialogueManager::ReceiveOnDialoguesFinished()
{	
	OnDialoguesFinished();
	m_OnDialoguePartStarted.Broadcast("");
	m_OnDialoguesFinished.Broadcast();

	
}

void AIntroDialogueManager::StartPlayDialogues()
{
	if (!ShouldPlay())
	{
		return;

	}

	if (m_apSpawnedDialogues.Num() > 0)
	{
		if (auto *pFirstDialogue{ m_apSpawnedDialogues[0] })
		{
			pFirstDialogue->Play();
		
			m_OnDialoguePartStarted.Broadcast(m_apDialogueWaves[0]->SpokenText);
		}
	}


}

