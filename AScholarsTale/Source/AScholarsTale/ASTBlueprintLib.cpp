// Fill out your copyright notice in the Description page of Project Settings.

#include "ASTBlueprintLib.h"
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



