// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandManagerComponent.h"
#include "Components/ActorComponent.h"
#include "ActionComponentBase.generated.h"

class UHandManagerComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Abstract )
class GAMEOFF2022_API UActionComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	

	UActionComponentBase();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GrabDistance = 300.0f;
	
protected:

	virtual void BeginPlay() override;

	
	virtual void PrimaryAction(EInputEvent InputEvent) {}

	virtual void SecondaryAction(EInputEvent InputEvent) {}


	AActor* GrabNearestGrabbable(EHandSide HandSide);

	void ReleaseGrabbable(EHandSide HandSide);

	UPROPERTY()
    AActor* RightHandGrabbed;
    
    UPROPERTY()
    AActor* LeftHandGrabbed;
	
	UPROPERTY()
	UInputComponent* InputComponentRef;

	UPROPERTY()
	UHandManagerComponent* OwnerHandManager;

private:
	
	void InputPrimaryActionPressed() {this->PrimaryAction(EInputEvent::IE_Pressed); }
	
	void InputPrimaryActionReleased() {this->PrimaryAction(EInputEvent::IE_Released); }

	void InputSecondaryActionPressed() {this->SecondaryAction(EInputEvent::IE_Pressed); }
	
	void InputSecondaryActionReleased() {this->SecondaryAction(EInputEvent::IE_Released); }
	
};
