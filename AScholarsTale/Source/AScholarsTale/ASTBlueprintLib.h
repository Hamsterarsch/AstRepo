// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	
	UFUNCTION(BlueprintCallable)
		static void PrepareMapChange(const TArray<FSoftObjectPath> &aLevelPaths, UWorld *pWorld);

	UFUNCTION(BlueprintCallable)
		static void CommitMapChange(UWorld *pWorld);

};
