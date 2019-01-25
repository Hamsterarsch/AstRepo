// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "SoftObjectPtr.h"

//Containslogs for errors resulting from client 'misuse'.
DECLARE_LOG_CATEGORY_EXTERN(AST_LogLogicError, All, All);

//General errors
DECLARE_LOG_CATEGORY_EXTERN(AST_Error, All, All);

//Contains general verbose informative logs.
DECLARE_LOG_CATEGORY_EXTERN(AST_LogInfo, Log, All);

//Contains logs for gliding and swining debugging. Defaults to only show log, but most logs are VeryVerbose.
DECLARE_LOG_CATEGORY_EXTERN(AST_Movement, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(AST_Teleport, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(AST_Interact, Log, All);


template<class T>
T *LoadClassPtr(const TSoftClassPtr<T> &Ptr)
{	
	T *pClass{ nullptr };
	if ((pClass = Ptr.Get()) == nullptr)
	{
		pClass = Ptr.LoadSynchronous();
		

	}
	return pClass;


}

template<class T>
T *LoadObjectPtr(const TSoftObjectPtr<T> &Ptr)
{
	T *pObject{ nullptr };
	if ((pObject = Ptr.Get()) == nullptr)
	{
		pObject = Ptr.LoadSynchronous();


	}
	return pObject;


}

