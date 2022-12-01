// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomGameInstance.h"

#include "LevelHandler.h"


void UCustomGameInstance::Init()
{
	Super::Init();

	this->GlobalLevelHandler = NewObject<ULevelHandler>();
	this->GlobalLevelHandler->SetGameInstance(this);

#if WITH_EDITOR
	this->GlobalLevelHandler->CurrentLevelIndex = 5;
#endif
	
}
