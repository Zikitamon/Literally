// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionComponentBase.h"
#include "HandManagerComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "GameOff2022/Utility/InterpFollower.h"
#include "Kismet/GameplayStatics.h"


UActionComponentBase::UActionComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;


}

void UActionComponentBase::BeginPlay()
{
	Super::BeginPlay();

	this->InputComponentRef = GetOwner()->FindComponentByClass<UInputComponent>();

	if (!this->InputComponentRef)
		return;
	
	this->InputComponentRef->BindAction("PrimaryAction", EInputEvent::IE_Pressed, this, &UActionComponentBase::InputPrimaryActionPressed);
	this->InputComponentRef->BindAction("PrimaryAction", EInputEvent::IE_Released, this, &UActionComponentBase::InputPrimaryActionReleased);
	this->InputComponentRef->BindAction("SecondaryAction", EInputEvent::IE_Pressed, this, &UActionComponentBase::InputSecondaryActionPressed);
	this->InputComponentRef->BindAction("SecondaryAction", EInputEvent::IE_Released, this, &UActionComponentBase::InputSecondaryActionReleased);

	this->OwnerHandManager = GetOwner()->FindComponentByClass<UHandManagerComponent>();

	if (!this->OwnerHandManager)
		return;
}

AActor* UActionComponentBase::GrabNearestGrabbable(EHandSide HandSide)
{
	TArray<AActor*> Grabbables;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Grabbable", Grabbables);

	FVector GrabLocation = GetOwner()->FindComponentByClass<USkeletalMeshComponent>()->GetSocketLocation(HandSide == EHS_Right ? TEXT("Hand_R") : TEXT("Hand_L"));

	bool bValidGrabbableFound = false;
	AActor* ClosestGrabbable = nullptr;
	
	for (AActor* Current : Grabbables)
	{
		if (!Current->FindComponentByClass<UInterpFollower>())
			continue;
			
		if (Current->FindComponentByClass<UInterpFollower>()->CurrentUser == this)
			continue;
		
		if (!bValidGrabbableFound ||
			FVector::Distance(GrabLocation, Current->GetActorLocation()) <=
			FVector::Distance(GrabLocation, ClosestGrabbable->GetActorLocation()))
		{
			if (FVector::Distance(GrabLocation, Current->GetActorLocation()) < this->GrabDistance)
			{
				bValidGrabbableFound = true;
				ClosestGrabbable = Current;
			}
		}
	}
	
	if (!bValidGrabbableFound)
	{
		return nullptr;
	}
	
	ClosestGrabbable->FindComponentByClass<UInterpFollower>()->StartFollowing(GetOwner()->FindComponentByClass<USkeletalMeshComponent>(), HandSide, this);

	if (this->PickUpSound)
	{
		TArray<UActorComponent*> ClosestGrabbableAudioComponents = ClosestGrabbable->GetComponentsByTag(UActorComponent::StaticClass(), TEXT("TEMP_PickUpSoundComponent"));
		for (UActorComponent* Current : ClosestGrabbableAudioComponents)
		{
			Current->DestroyComponent();
		}
		
		UAudioComponent* NewlySpawnedSound = UGameplayStatics::SpawnSoundAttached(this->PickUpSound, ClosestGrabbable->GetRootComponent());
		NewlySpawnedSound->ComponentTags.Add(TEXT("TEMP_PickUpSoundComponent"));
		
	}

	if (HandSide == EHS_Right)
	{
		this->RightHandGrabbed = ClosestGrabbable;
	}
	if (HandSide == EHS_Left)
	{
		this->LeftHandGrabbed = ClosestGrabbable;
	}
	
	return ClosestGrabbable;
}

void UActionComponentBase::ReleaseGrabbable(EHandSide HandSide)
{
	if (HandSide == EHS_Right)
	{
		if (!this->RightHandGrabbed)
			return;

		if (this->RightHandGrabbed->FindComponentByClass<UInterpFollower>()->CurrentUser != this)
		{
			this->RightHandGrabbed = nullptr;
			return;
		}
		
		this->RightHandGrabbed->FindComponentByClass<UInterpFollower>()->StopFollowing();
		this->RightHandGrabbed = nullptr;
	}
	if (HandSide == EHS_Left)
	{
		if (!this->LeftHandGrabbed)
			return;

		if (this->LeftHandGrabbed->FindComponentByClass<UInterpFollower>()->CurrentUser != this)
		{
			this->LeftHandGrabbed = nullptr;
			return;
		}
		
		this->LeftHandGrabbed->FindComponentByClass<UInterpFollower>()->StopFollowing();
		this->LeftHandGrabbed = nullptr;
	}
}

void UActionComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	
	
}
