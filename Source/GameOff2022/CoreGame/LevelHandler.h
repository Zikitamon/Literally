// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelHandler.generated.h"

class ULevelProgressSaveGame;

/**
 * 
 */
UCLASS(BlueprintType)
class GAMEOFF2022_API ULevelHandler : public UObject
{
	GENERATED_BODY()

public:

	ULevelHandler();
	
	void SetGameInstance(UGameInstance* InGameInstance) {this->GameInstanceRef = InGameInstance;}
	
	UFUNCTION(BlueprintCallable)
	void PlayLevelByIndex(int32 InLevelIndex);

	UFUNCTION(BlueprintCallable)
	void ShufflePlayLevels();

	UFUNCTION(BlueprintCallable)
	void NextLevel();
	
	UFUNCTION(BlueprintCallable)
	void RestartLevel();

	UFUNCTION(BlueprintCallable)
	void ReturnToMainMenu();
	
	
	void LevelCompleted();

	UFUNCTION(BlueprintCallable)
	TSet<int32> GetCompletedLevels();
	

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentLevelIndex = -1;
	
protected:

	bool bShufflePlay = false;

	void InternalOpenLevelByIndex(int32 Index);

	UPROPERTY()
	ULevelProgressSaveGame* LevelProgressData;

	void UpdateSavedData(int32 NewCompletedLevel);

private:

	UPROPERTY()
	UGameInstance* GameInstanceRef;
	
};
