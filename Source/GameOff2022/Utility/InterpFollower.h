// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameOff2022/CoreGame/HandManagerComponent.h"
#include "InterpFollower.generated.h"


// this class has been modified from the original InterpFollower to suit the current projects needs
// if you would like to reuse this class, please use the original unmodified version

/*
 * a utility component that can make the owning actor follow a scene component in the world
 * the following can be specified to have an offset or a specific bone to follow
 * it also features an initial interpolation to the followed component
 * all following is done by interpolating between the owning actor and the destination
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEOFF2022_API UInterpFollower : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInterpFollower();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartFollowing(USceneComponent* InOriginPoint, EHandSide HandSide, const UObject* InUser, FTransform PositionOverride = FTransform());

	UFUNCTION(BlueprintCallable)
	void StopFollowing();

	UPROPERTY(BlueprintReadOnly)
	const UObject* CurrentUser;
	
protected:

	virtual void BeginPlay() override;
	
	// is this component enabled at all
	UPROPERTY(EditAnywhere)
	bool bUseInterpFollower = true;

	UPROPERTY(EditAnywhere)
	FTransform RightHandSocketOffset = FTransform::Identity;

	UPROPERTY(EditAnywhere)
	FTransform LeftHandSocketOffset = FTransform::Identity;
	
	// SmoothStartInterpolation is the initial interpolation to the destination
	// this controls how long in seconds should the SmoothStartInterpolation take
	UPROPERTY(EditAnywhere)
	float SmoothStartInterpTime = 0.2f;

	// if the location is glued, we won't interpolate to that location, but instead just set the location straight
	// gluing can be thought as not even using the InterpFollower features for location
	UPROPERTY(EditAnywhere)
	bool bGlueLocation = false;
	
	// the speed of the interpolation of the location following
	UPROPERTY(EditAnywhere)
	float LocationInterpSpeed = 0.5f;

	// if the rotation is glued, we won't interpolate to that rotation, but instead just set the rotation straight
	// gluing can be thought as not even using the InterpFollower features for rotation
	UPROPERTY(EditAnywhere)
	bool bGlueRotation = false;
	
	// the speed of the interpolation of the angular following
	UPROPERTY(EditAnywhere)
	float RotationInterpSpeed = 0.4f;

private:

	FTransform InterpStartTransform;
	
	float InterpStartTime;
	
	UPROPERTY()
	USceneComponent* OriginPoint;
	
	FTransform SocketOffset;
	
	FName BoneName;
	
	UPROPERTY()
	TArray<UPrimitiveComponent*> OwnerPrimitiveComponents;

	TOptional<FVector> ScaleBackup;

	bool bIsStopping = false;
	
};
