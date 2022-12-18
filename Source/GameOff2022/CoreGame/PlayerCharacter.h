// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionComponentBase.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UHandManagerComponent;


UCLASS()
class GAMEOFF2022_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
protected:

	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);
	
	void MoveRight(float AxisValue);

	void LookUp(float AxisValue);
	
	void LookRight(float AxisValue);

	void InputJump();
	
	UFUNCTION(BlueprintCallable)
	UActionComponentBase* AddActionComponent(TSubclassOf<UActionComponentBase> ActionComponentClass);
	
	UPROPERTY(VisibleAnywhere)
	UHandManagerComponent* HandManager;

	UPROPERTY(VisibleAnywhere)
	UActionComponentBase* ActionComponent;
	
};
