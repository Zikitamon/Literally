// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelHandler.h"

#include "LevelProgressSaveGame.h"
#include "Kismet/GameplayStatics.h"


ULevelHandler::ULevelHandler()
{
	this->LevelProgressData = Cast<ULevelProgressSaveGame>(UGameplayStatics::LoadGameFromSlot("LevelProgress", 0));

	if (!this->LevelProgressData)
		this->LevelProgressData = Cast<ULevelProgressSaveGame>(UGameplayStatics::CreateSaveGameObject(ULevelProgressSaveGame::StaticClass()));

	this->CurrentLevelIndex = -1;
}

void ULevelHandler::PlayLevelByIndex(int32 InLevelIndex)
{
	this->InternalOpenLevelByIndex(InLevelIndex);
}

void ULevelHandler::ShufflePlayLevels()
{
	this->bShufflePlay = true;
	
	this->NextLevel();
}

void ULevelHandler::NextLevel()
{
	if (this->bShufflePlay)
	{
		int32 RandomLevelIndex = FMath::RandRange(0, 6);

		while (RandomLevelIndex == this->CurrentLevelIndex)
			RandomLevelIndex = FMath::RandRange(0, 6);
	
		this->InternalOpenLevelByIndex(RandomLevelIndex);
	}
	else
	{
		this->InternalOpenLevelByIndex(this->CurrentLevelIndex + 1);
	}
}

void ULevelHandler::RestartLevel()
{
	this->InternalOpenLevelByIndex(this->CurrentLevelIndex);
}

void ULevelHandler::ReturnToMainMenu()
{
	this->CurrentLevelIndex = -1;
	this->bShufflePlay = false;

	UGameplayStatics::OpenLevel(this->GameInstanceRef->GetWorld(), TEXT("/Game/Menu/Menu"));
}

void ULevelHandler::LevelCompleted()
{
	this->UpdateSavedData(CurrentLevelIndex);
	
}

TSet<int32> ULevelHandler::GetCompletedLevels()
{
	return this->LevelProgressData->CompletedLevels;
}

void ULevelHandler::InternalOpenLevelByIndex(int32 Index)
{
	if (Index > 6)
	{
		ReturnToMainMenu();
		return;
	}

	this->CurrentLevelIndex = Index;
	// open a level with the specified LevelIndex
	// all the game levels will be located in /Game/Gamelevels

	FName LevelPath = *(TEXT("/Game/GameLevels/") + FString::FromInt(Index));
	UGameplayStatics::OpenLevel(this->GameInstanceRef->GetWorld(), LevelPath);
}

void ULevelHandler::UpdateSavedData(int32 NewCompletedLevel)
{
	if (NewCompletedLevel == -1)
		return;
	
	this->LevelProgressData->CompletedLevels.Add(NewCompletedLevel);
	
	UGameplayStatics::AsyncSaveGameToSlot(this->LevelProgressData, "LevelProgress", 0);
}
