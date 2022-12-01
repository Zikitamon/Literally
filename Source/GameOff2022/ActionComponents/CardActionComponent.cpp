// Fill out your copyright notice in the Description page of Project Settings.

#include "CardActionComponent.h"
#include "GameOff2022/CoreGame/HandManagerComponent.h"


UCardActionComponent::UCardActionComponent()
{
	
}

void UCardActionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCardActionComponent::PrimaryAction(EInputEvent InputEvent)
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
}

void UCardActionComponent::SecondaryAction(EInputEvent InputEvent)
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

	
}
