// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerController.h"

#include "CustomGameInstance.h"
#include "LevelHandler.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"


AGamePlayerController::AGamePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SetInputMode(FInputModeGameOnly());
	this->PauseMenuWidgetRef = CreateWidget(this, this->PauseMenuWidgetClass);
}

void AGamePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!this->bLevelStopped)
	{
		if (IsInputKeyDown(EKeys::Escape) && !this->bEscapePressed)
		{
			this->PauseGame();
			this->bEscapePressed = true;
		}

		if (!IsInputKeyDown(EKeys::Escape))
		{
			this->bEscapePressed = false;
		}
	}

	if (this->bPlayerWin)
	{
		GetPawn()->FindComponentByClass<UCameraComponent>()->SetRelativeLocation(FVector(0.0,
																						FMath::Cos(GetWorld()->GetTimeSeconds() * 10.0) * 30.0,
																						FMath::Sin(GetWorld()->GetTimeSeconds() * 10.0) * 30.0
																						) +
																				FVector(0.0, 0.0, 70.0));
	}

	if (this->bPlayerLost)
	{
		GetPawn()->AddActorLocalRotation(FRotator(70.0 * DeltaSeconds, 0.0, 0.0));
		FRotator NewPawnRotation = GetPawn()->GetActorRotation();
		NewPawnRotation.Pitch = FMath::ClampAngle(NewPawnRotation.Pitch, 0.0, 85.0);
		GetPawn()->SetActorRotation(NewPawnRotation);

		if (NewPawnRotation.Pitch >= 85.0)
			this->bPlayerLost = false;
	}
}

void AGamePlayerController::WinLevel()
{
	if (this->bPlayerWin || this->bPlayerActuallyLost)
		return;

	this->StopLevel();
	this->bPlayerWin = true;

	this->WinWidgetRef = CreateWidget(this, this->WinWidgetClass, TEXT("PlayerWinWidget"));
	this->WinWidgetRef->AddToViewport(1);

	GetGameInstance<UCustomGameInstance>()->GetLevelHandler()->LevelCompleted();
}

void AGamePlayerController::LoseLevel()
{
	if (this->bPlayerActuallyLost || this->bPlayerWin)
		return;
	
	this->StopLevel();
	this->bPlayerLost = true;
	this->bPlayerActuallyLost = true;
	
	this->LoseWidgetRef = CreateWidget(this, this->LoseWidgetClass, TEXT("PlayerLoseWidget"));
	this->LoseWidgetRef->AddToViewport();
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	
}

void AGamePlayerController::PauseGame()
{
	bool bWasPaused = IsPaused();
	
	if (bWasPaused)
	{
		this->PauseMenuWidgetRef->RemoveFromParent();
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		this->PauseMenuWidgetRef->AddToViewport();
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(this->PauseMenuWidgetRef->TakeWidget());
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}
	SetShowMouseCursor(!bWasPaused);
	
	SetPause(!bWasPaused);
	this->OnGamePaused.Broadcast(!bWasPaused);
}

void AGamePlayerController::StopLevel()
{
	this->bLevelStopped = true;
	
	GetPawn()->FindComponentByClass<UCameraComponent>()->bUsePawnControlRotation = false;
	GetPawn()->DisableInput(this);

	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);
}

