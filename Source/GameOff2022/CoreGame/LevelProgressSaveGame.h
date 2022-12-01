// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LevelProgressSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GAMEOFF2022_API ULevelProgressSaveGame : public USaveGame
{
	GENERATED_BODY()

public:	
	
	UPROPERTY()
	TSet<int32> CompletedLevels;
};
