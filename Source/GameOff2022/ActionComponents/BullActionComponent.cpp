// Fill out your copyright notice in the Description page of Project Settings.

#include "BullActionComponent.h"

#include "GameOff2022/CoreGame/GamePlayerController.h"


UBullActionComponent::UBullActionComponent()
{
	
}

void UBullActionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UBullActionComponent::PrimaryAction(EInputEvent InputEvent)
{
	Super::PrimaryAction(InputEvent);

	switch (InputEvent)
	{
	case IE_Pressed:
		OwnerHandManager->bLeftHandClosed = true;
		GrabNearestGrabbable(EHS_Left);
		break;

	case IE_Released:
		OwnerHandManager->bLeftHandClosed = false;
		ReleaseGrabbable(EHS_Left);
		break;
		
	default: ;
	}

	this->CheckWinCondition();
}

void UBullActionComponent::SecondaryAction(EInputEvent InputEvent)
{
	Super::SecondaryAction(InputEvent);

	switch (InputEvent)
	{
	case IE_Pressed:
		OwnerHandManager->bRightHandClosed = true;
		GrabNearestGrabbable(EHS_Right);
		break;

	case IE_Released:
		OwnerHandManager->bRightHandClosed = false;
		ReleaseGrabbable(EHS_Right);
		break;
		
	default: ;
	}

	this->CheckWinCondition();
}

void UBullActionComponent::CheckWinCondition()
{
	if ((RightHandGrabbed || LeftHandGrabbed) && OwnerHandManager->bRightHandClosed && OwnerHandManager->bLeftHandClosed)
	{
		FTimerHandle WinTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(WinTimerHandle, FTimerDelegate::CreateLambda([this]()
		{
			if (this->bAlreadyWinned)
				return;

			if ((RightHandGrabbed || LeftHandGrabbed) && OwnerHandManager->bRightHandClosed && OwnerHandManager->bLeftHandClosed)
			{
				GetOwner<APawn>()->GetController<AGamePlayerController>()->WinLevel();
			}
		}), 1.5f, false);
	}
}
