// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameOff2022/CoreGame/ActionComponentBase.h"
#include "CardActionComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAMEOFF2022_API UCardActionComponent : public UActionComponentBase
{
	GENERATED_BODY()

public:

	UCardActionComponent();

protected:

	virtual void BeginPlay() override;
	
	virtual void PrimaryAction(EInputEvent InputEvent) override;

	virtual void SecondaryAction(EInputEvent InputEvent) override;
	
};
