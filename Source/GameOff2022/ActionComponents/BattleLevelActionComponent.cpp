// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleLevelActionComponent.h"

#include "GameOff2022/CoreGame/HandManagerComponent.h"


UBattleLevelActionComponent::UBattleLevelActionComponent()
{
	
}

void UBattleLevelActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerHandManager->bLeftHandClosed = true;
	OwnerHandManager->bRightHandClosed = true;
}

void UBattleLevelActionComponent::PrimaryAction(EInputEvent InputEvent)
{
	Super::PrimaryAction(InputEvent);

	OwnerHandManager->LeftPunch();
}

void UBattleLevelActionComponent::SecondaryAction(EInputEvent InputEvent)
{
	Super::SecondaryAction(InputEvent);

	OwnerHandManager->RightPunch();
}
