// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

class ULevelHandler;

/**
 * 
 */
UCLASS()
class GAMEOFF2022_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	ULevelHandler* GetLevelHandler() const {return this->GlobalLevelHandler;}

	UPROPERTY(BlueprintReadWrite)
	bool bMenuInitialized = false;
	
protected:

	UPROPERTY()
	ULevelHandler* GlobalLevelHandler;
	
	
};
