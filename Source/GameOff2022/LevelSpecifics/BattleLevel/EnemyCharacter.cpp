// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOff2022/LevelSpecifics/BattleLevel/EnemyCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameOff2022/CoreGame/GamePlayerController.h"


AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->PlayerPawnRef = GetGameInstance()->GetPrimaryPlayerController()->GetPawn();

	this->bCurrentlyMoving = true;
	this->ChangeMovementStatus();
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->bDisabled)
		return;

	if (this->bCurrentlyAttacking)
	{
		FQuat DesiredRotation = (this->PlayerPawnRef->GetActorLocation() - GetActorLocation()).ToOrientationQuat();
		GetController()->SetControlRotation(FQuat::Slerp(GetActorQuat(), DesiredRotation, 5.0 * DeltaTime).Rotator());
		
		return;
	}
	
	if (FVector::Distance(GetActorLocation(), this->PlayerPawnRef->GetActorLocation()) < 100.0)
	{
		this->bCurrentlyAttacking = true;
		FTimerHandle AttackingTimerHandle;
		GetWorldTimerManager().SetTimer(AttackingTimerHandle, FTimerDelegate::CreateUObject(this, &AEnemyCharacter::Attack), 1.0f, false);
		
		return;
	}	
	
	if (this->bCurrentlyMoving)
	{
		AddMovementInput((this->PlayerPawnRef->GetActorLocation() - GetActorLocation()).GetSafeNormal());

		if (GetCharacterMovement()->Velocity.Length() < 5.0 && this->bWasMovingInLastTick)
		{
			Jump();
		}

		this->bWasMovingInLastTick = true;
	}
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	this->Health -= DamageAmount;

	if (this->Health <= 0.0f)
	{
		this->bDisabled = true;
		this->OnDeath();
		
		FTimerHandle WinGameTimerHandle;
		GetWorldTimerManager().SetTimer(WinGameTimerHandle, FTimerDelegate::CreateLambda([this, EventInstigator]()
			{
				if (AGamePlayerController* WinnerPlayer = Cast<AGamePlayerController>(EventInstigator); WinnerPlayer)
					WinnerPlayer->WinLevel();
			}), 1.0f, false);
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemyCharacter::ChangeMovementStatus()
{
	if (this->bDisabled)
		return;
	
	FTimerHandle MovementStatusTimerHandle;
	float NextChangeDelay = 3.0f;
	if (this->bCurrentlyMoving)
	{
		this->bCurrentlyMoving = false;
		this->bWasMovingInLastTick = false;
		
		NextChangeDelay = 3.0f;
	}
	else
	{
		this->bCurrentlyMoving = true;

		NextChangeDelay = 15.0f;
	}

	GetWorldTimerManager().SetTimer(MovementStatusTimerHandle, FTimerDelegate::CreateUObject(this, &AEnemyCharacter::ChangeMovementStatus), NextChangeDelay, false);
}

void AEnemyCharacter::Attack()
{
	if (this->bDisabled)
		return;
	
	this->bCurrentlyAttacking = false;
	this->bWasMovingInLastTick = false;
	
	FHitResult AttackHitResult;
	GetWorld()->LineTraceSingleByChannel(AttackHitResult, GetActorLocation(), GetActorRotation().Vector() * 200.0 + GetActorLocation(), ECollisionChannel::ECC_Visibility, FCollisionQueryParams("AttackTrace", false, this));

	APawn* HitPawn = Cast<APawn>(AttackHitResult.GetActor());

	if (!HitPawn)
		return;

	AGamePlayerController* HitPlayerController = HitPawn->GetController<AGamePlayerController>();

	if (!HitPlayerController)
		return;

	HitPlayerController->LoseLevel();
	this->bDisabled = true;
}
