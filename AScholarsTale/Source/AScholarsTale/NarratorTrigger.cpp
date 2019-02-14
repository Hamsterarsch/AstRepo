// Fill out your copyright notice in the Description page of Project Settings.

#include "NarratorTrigger.h"
#include "ASTGameInstance.h"

void ANarratorTrigger::BeginPlay()
{
	Super::BeginPlay();

	auto *pGI{ GetGameInstance() };
	if ( pGI && pGI->IsA<UASTGameInstance>() )
	{
		auto *pASTGI{ static_cast<UASTGameInstance *>(pGI) };
		
		SaveDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(ANarratorTrigger, ReceiveOnSaveGame));
		LoadDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(ANarratorTrigger, ReceiveOnLoadGame));

		pASTGI->m_OnSaveGame.Add(SaveDelegate);
		pASTGI->m_OnLoadGame.Add(LoadDelegate);
		
	}


}
