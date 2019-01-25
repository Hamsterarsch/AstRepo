// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Engine/ObjectLibrary.h"
#include "ASTBlueprintLib.generated.h"

/**
 * 
 */
UCLASS()
class ASCHOLARSTALE_API UASTBlueprintLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ObjectLibrary")
		static UObject *LoadObjectLibraryAssetAt(int Index, const FString &Path);

	UFUNCTION(BlueprintCallable, Category = "ObjectLibrary")
		static TArray<UObject *> LoadObjectLibrary(const FString &Path);
	
	template<class AssetType>
		static TArray<AssetType> LoadObjectLibraryTyped(const FString &Path);

	UFUNCTION(BlueprintCallable)
		static void PrepareMapChange(const TArray<FSoftObjectPath> &aLevelPaths, UObject *pContext);

	UFUNCTION(BlueprintCallable)
		static void CommitMapChange(UObject *pContext);

	UFUNCTION(BlueprintCallable)
		static bool IsMapChangeReady(UObject *pContext);

};

template<class AssetType>
TArray<AssetType> UASTBlueprintLib::LoadObjectLibraryTyped(const FString &Path)
{	
	auto *ObjectLibrary = LoadObject<UObjectLibrary>(nullptr, *Path);
	if (ObjectLibrary)
	{
		TArray<AssetType> LibraryAssets{};
		ObjectLibrary->GetObjects(LibraryAssets);

		return LibraryAssets;

	}
	return {};

	

}