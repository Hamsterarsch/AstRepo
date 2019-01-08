// Fill out your copyright notice in the Description page of Project Settings.

#include "ASTBlueprintLib.h"
#include "Engine.h"
#include "Runtime/Engine/Classes/Engine/ObjectLibrary.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"


UObject *UASTBlueprintLib::LoadObjectLibraryAssetAt(const int Index, const FString &Path)
{
	auto *ObjectLibrary = LoadObject<UObjectLibrary>(nullptr, *Path);
	if (ObjectLibrary)
	{
		TArray<UObject *> LibraryAssets{};
		ObjectLibrary->GetObjects(LibraryAssets);

		if (Index < LibraryAssets.Num())
		{
			return LibraryAssets[Index];

		}

	}
	return nullptr;


}

TArray<UObject *> UASTBlueprintLib::LoadObjectLibrary(const FString &Path)
{
	TArray<UObject *> LibraryObjects{};

	auto *ObjectLibrary = LoadObject<UObjectLibrary>(nullptr, *Path);
	if (ObjectLibrary)
	{
		TArray<UObject *> LibraryAssets{};
		ObjectLibrary->GetObjects(LibraryAssets);

		return LibraryAssets;

	}
	return {};


}

void UASTBlueprintLib::PrepareMapChange(const TArray<FSoftObjectPath> &aLevelPaths, UObject *pContext)
{
	TArray<FName> aPackageNames{};
	auto pWorld{ pContext->GetWorld() };

	for (auto Itr = aLevelPaths.CreateConstIterator(); Itr; ++Itr)
	{		
		aPackageNames.Add(*Itr->GetLongPackageName());

	}
	
	if (pWorld)
	{
		pWorld->PrepareMapChange(aPackageNames);

	}
	

}

void UASTBlueprintLib::CommitMapChange(UObject *pContext)
{
	auto pWorld{ pContext->GetWorld() };

	if ( pWorld && pWorld->IsMapChangeReady() && !IsAsyncLoading() )
	{
		pWorld->CommitMapChange();
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CommitWorldNull"));
	}


}

bool UASTBlueprintLib::IsMapChangeReady(UObject *pContext)
{
	auto pWorld{ pContext->GetWorld() };

	ensureMsgf(pWorld, TEXT("Could not get world"));

	return pWorld->IsMapChangeReady() && !IsAsyncLoading() ;
	

}


