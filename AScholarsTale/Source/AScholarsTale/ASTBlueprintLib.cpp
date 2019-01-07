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

void UASTBlueprintLib::PrepareMapChange(const TArray<FSoftObjectPath> &aLevelPaths, UWorld *pWorld)
{
	//Context.World()->PrepareMapChange(aLevelNames);
	TArray<FName> aPackageNames{};

	for (auto Itr = aLevelPaths.CreateConstIterator(); *Itr != aLevelPaths.Last(); ++Itr)
	{		
		aPackageNames.Add(*Itr->GetLongPackageName());

	}
	
	if (pWorld)
	{
		pWorld->PrepareMapChange(aPackageNames);

	}
	

}

void UASTBlueprintLib::CommitMapChange(UWorld *pWorld)
{
	if (pWorld)
	{
		pWorld->CommitMapChange();
	}

}


