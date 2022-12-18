// Fill out your copyright notice in the Description page of Project Settings.

#include "HandManagerComponent.h"

#include "Engine/DamageEvents.h"


#define PRINTSTRING(InString) GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, TEXT(InString)); UE_LOG(LogTemp, Warning, TEXT(InString));
#undef PRINTSTRING

UHandManagerComponent::UHandManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UHandManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	this->OwnerHandsSkeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

void UHandManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

}


void UHandManagerComponent::RightPunch()
{
	if (this->bRightPunchOnCooldown)
		return;
	
	FTimerHandle PunchDamageTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(PunchDamageTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		FHitResult PunchHitResult;
		GetWorld()->LineTraceSingleByChannel(PunchHitResult,
											 this->OwnerHandsSkeletalMesh->GetSocketLocation("Hand_R"),
											 this->OwnerHandsSkeletalMesh->GetSocketRotation("Hand_R").Vector() * 150.0 + this->OwnerHandsSkeletalMesh->GetSocketLocation("Hand_R"),
											 ECC_Visibility,
											 FCollisionQueryParams(NAME_None, false, GetOwner()));
		
		if (PunchHitResult.GetActor())
		{
			PunchHitResult.GetActor()->TakeDamage(10.0f, FDamageEvent(), GetOwner<APawn>()->GetController(), GetOwner());
		}
		
	}), 0.05f, false);
	
	this->bRightPunchOnCooldown = true;
	FTimerHandle PunchCooldownTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(PunchCooldownTimerHandle, FTimerDelegate::CreateLambda([this](){this->bRightPunchOnCooldown = false;}), this->PunchCooldownTime, false);
	
	this->OnPunch.Broadcast(EHS_Right);
	
	if (this->PunchSound)
		GetOwner()->GetGameInstance()->GetPrimaryPlayerController()->ClientPlaySound(this->PunchSound);
}

void UHandManagerComponent::LeftPunch()
{
	if (this->bLeftPunchOnCooldown)
		return;

	FTimerHandle PunchDamageTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(PunchDamageTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		FHitResult PunchHitResult;
		GetWorld()->LineTraceSingleByChannel(PunchHitResult,
											 this->OwnerHandsSkeletalMesh->GetSocketLocation("Hand_L"),
											 this->OwnerHandsSkeletalMesh->GetSocketRotation("Hand_L").Vector() * 150.0 + this->OwnerHandsSkeletalMesh->GetSocketLocation("Hand_L"),
											 ECC_Visibility,
											 FCollisionQueryParams(NAME_None, false, GetOwner()));
		if (PunchHitResult.GetActor())
		{
			PunchHitResult.GetActor()->TakeDamage(10.0f, FDamageEvent(), GetOwner<APawn>()->GetController(), GetOwner());
		}
	}), 0.05f, false);
	
	this->bLeftPunchOnCooldown = true;
	FTimerHandle PunchCooldownTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(PunchCooldownTimerHandle, FTimerDelegate::CreateLambda([this](){this->bLeftPunchOnCooldown = false;}), this->PunchCooldownTime, false);
	
	this->OnPunch.Broadcast(EHS_Left);
	
	if (this->PunchSound)
		GetOwner()->GetGameInstance()->GetPrimaryPlayerController()->ClientPlaySound(this->PunchSound);
}
