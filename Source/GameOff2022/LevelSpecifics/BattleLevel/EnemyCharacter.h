// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"


UCLASS()
class GAMEOFF2022_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	AEnemyCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BlueprintReadOnly)
	float Health = 100.0f;
	
protected:
	
	virtual void BeginPlay() override;

	void ChangeMovementStatus();

	void Attack();

	UPROPERTY(BlueprintReadOnly)
	bool bCurrentlyAttacking = false;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();

	UPROPERTY(EditAnywhere)
	USoundBase* TakeDamageSound;

	UPROPERTY(EditAnywhere)
	USoundBase* PunchSound;
	
private:

	UPROPERTY()
	APawn* PlayerPawnRef;

	bool bCurrentlyMoving = false;

	bool bWasMovingInLastTick = false;
	
	bool bDisabled = false;
	
};
