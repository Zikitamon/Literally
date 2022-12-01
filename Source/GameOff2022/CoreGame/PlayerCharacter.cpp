// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

#include "HandManagerComponent.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	this->HandManager = CreateDefaultSubobject<UHandManagerComponent>(TEXT("HandManager"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APlayerCharacter::LookRight);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::InputJump);

}


///
/// input functions
///
void APlayerCharacter::MoveForward(float AxisValue)
{
	if (AxisValue == 0.0f)
		return;

	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if (AxisValue == 0.0f)
		return;

	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::LookUp(float AxisValue)
{
	if (AxisValue == 0.0f)
		return;

	AddControllerPitchInput(AxisValue);
}

void APlayerCharacter::LookRight(float AxisValue)
{
	if (AxisValue == 0.0f)
		return;

	AddControllerYawInput(AxisValue);
}

void APlayerCharacter::InputJump()
{
	Jump();
}

///
///
///



void APlayerCharacter::AddActionComponent(TSubclassOf<UActionComponentBase> ActionComponentClass)
{
	if (!IsValid(ActionComponentClass))
		return;
	
	this->ActionComponent = NewObject<UActionComponentBase>(this, ActionComponentClass);
	this->ActionComponent->CreationMethod = EComponentCreationMethod::Instance;
	this->ActionComponent->RegisterComponent();
	AddInstanceComponent(this->ActionComponent);
}
