// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Containslogs for errors resulting from client 'misuse'.
DECLARE_LOG_CATEGORY_EXTERN(AST_LogLogicError, All, All);

//Contains general verbose informative logs.
DECLARE_LOG_CATEGORY_EXTERN(AST_LogInfo, Log, All);

//Contains logs for gliding and swining debugging. Defaults to only show log, but most logs are VeryVerbose.
DECLARE_LOG_CATEGORY_EXTERN(AST_Movement, Log, All);

