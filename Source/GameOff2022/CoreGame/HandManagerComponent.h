// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HandManagerComponent.generated.h"

UENUM(BlueprintType)
enum EHandSide
{
	EHS_Right,
	EHS_Left,
	EHS_None
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEOFF2022_API UHandManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPunchSignature, TEnumAsByte<EHandSide>, HandSide);

public:	

	UHandManagerComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadOnly)
    bool bRightHandClosed;
    
    UPROPERTY(BlueprintReadOnly)
    bool bLeftHandClosed;
	
	void RightPunch();

	void LeftPunch();

	UPROPERTY(BlueprintAssignable)
	FPunchSignature OnPunch;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float PunchCooldownTime = 2.0f;

	UPROPERTY(EditAnywhere)
	USoundBase* PunchSound;
	
private:

	bool bRightPunchOnCooldown = false;

	bool bLeftPunchOnCooldown = false;

	UPROPERTY()
	USkeletalMeshComponent* OwnerHandsSkeletalMesh;
	
};
