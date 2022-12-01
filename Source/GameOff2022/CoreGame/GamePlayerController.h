// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"


/**
 * 
 */
UCLASS()
class GAMEOFF2022_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGamePausedSignature, bool, bPaused);
	
public:

	AGamePlayerController();

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void WinLevel();
	
	UFUNCTION(BlueprintCallable)
    void LoseLevel();	

	UPROPERTY(BlueprintAssignable)
	FGamePausedSignature OnGamePaused;
	
protected:

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void PauseGame();

	void StopLevel();
	
	UPROPERTY()
	UUserWidget* PauseMenuWidgetRef;

	UPROPERTY()
	UUserWidget* WinWidgetRef;
	
	UPROPERTY()
	UUserWidget* LoseWidgetRef;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WinWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoseWidgetClass;

private:
	
	bool bEscapePressed = false;

	bool bPlayerWin = false;
	
	bool bPlayerLost = false;

	bool bPlayerActuallyLost = false;

	bool bLevelStopped = false;
};
