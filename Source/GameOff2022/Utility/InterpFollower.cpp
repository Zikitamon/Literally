// Fill out your copyright notice in the Description page of Project Settings.

#include "InterpFollower.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UInterpFollower::UInterpFollower()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UInterpFollower::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()->HasAuthority())
		this->bUseInterpFollower = false;

	
	TArray<UActorComponent*> TempMeshComponents;
	GetOwner()->GetComponents(UPrimitiveComponent::StaticClass(), TempMeshComponents);
	
	for (UActorComponent* Current : TempMeshComponents)
		this->OwnerPrimitiveComponents.Add(Cast<UPrimitiveComponent>(Current));
}

void UInterpFollower::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!this->bUseInterpFollower)
		return;

	// if a transform to follow has been specified
	if (this->OriginPoint)
	{
		/*
		 *  at first, the ActualTransform will be the the transform SocketOffset is relative to
		 *  then, we check if a BoneName is specified, if yes, the SocketOffset will be relative to the bones transform
		 *  and finally, the ActualTransform will be the SocketOffset in world space
		 */
		FTransform RootTransform = this->OriginPoint->GetComponentTransform();
		
		if (this->BoneName != NAME_None)
			if (USkinnedMeshComponent* SkeletalOriginPoint = Cast<USkinnedMeshComponent>(this->OriginPoint))
				RootTransform = SkeletalOriginPoint->GetSocketTransform(this->BoneName);
		
		// getting the SocketOffset in world space so that we can use it to interpolate this component's transform
		FTransform ActualTransform = FTransform(RootTransform.TransformRotation(this->SocketOffset.GetRotation()),
												RootTransform.TransformPosition(this->SocketOffset.GetLocation()),
												/*RootTransform.GetScale3D() * */this->SocketOffset.GetScale3D());
		
		
		// if we are still in the time range of interpolating the SmoothStartInterp and SmoothStartInterp is enabled (is greater than 0)
		if (GetWorld()->GetTimeSeconds() - this->InterpStartTime <= this->SmoothStartInterpTime && this->SmoothStartInterpTime > 0.0f)
		{
			/*
			 * this is where the initial interpolation 'SmoothStartInterp' is calculated
			 */
			GetOwner()->SetActorLocation(FMath::LerpStable(this->InterpStartTransform.GetLocation(), ActualTransform.GetLocation(), FMath::Clamp((GetWorld()->GetTimeSeconds() - this->InterpStartTime) / this->SmoothStartInterpTime, 0.0f, 1.0f)));
			GetOwner()->SetActorRotation(FQuat::Slerp(this->InterpStartTransform.GetRotation(), ActualTransform.GetRotation(), FMath::Clamp((GetWorld()->GetTimeSeconds() - this->InterpStartTime) / this->SmoothStartInterpTime, 0.0f, 1.0f)));
			GetOwner()->SetActorScale3D(FMath::LerpStable(this->InterpStartTransform.GetScale3D(), ActualTransform.GetScale3D(), FMath::Clamp((GetWorld()->GetTimeSeconds() - this->InterpStartTime) / this->SmoothStartInterpTime, 0.0f, 1.0f)));
		}
		else
		{
			// if the location is glued, we just set it straight to the end location, the interpolation isn't skipped, since interpolating from spot A to A does nothing 
			if (this->bGlueLocation)
				GetOwner()->SetActorLocation(ActualTransform.GetLocation());
			// if the rotation is glued, we just set it straight to the end rotation, the interpolation isn't skipped, since interpolating from rotation A to A does nothing
			if (this->bGlueRotation)
				GetOwner()->SetActorRotation(ActualTransform.GetRotation());
			
			// DeltaDistance = OwningActorLocation + CurrentDistance * (1 - (1 - InterpSpeed)^DeltaTime);
			// we aren't using the formula above for performance reasons, it really doesn't matter that much since this calculation is only executed in the server and hence will be the same for all players
			GetOwner()->SetActorScale3D(FMath::VInterpTo(GetOwner()->GetActorScale3D(), ActualTransform.GetScale3D(), DeltaTime, LocationInterpSpeed));
			GetOwner()->SetActorRotation(FMath::QInterpTo(GetOwner()->GetActorRotation().Quaternion(), ActualTransform.GetRotation(), DeltaTime, RotationInterpSpeed));
			GetOwner()->SetActorLocation(FMath::VInterpTo(GetOwner()->GetActorLocation(), ActualTransform.GetLocation(), DeltaTime, LocationInterpSpeed));
		}
	}
	// we reuse the interpolation logic from earlier to interpolate the object scale to be tha same as before starting to follow it
	else if (GetWorld()->GetTimeSeconds() - this->InterpStartTime <= this->SmoothStartInterpTime && this->SmoothStartInterpTime > 0.0f && this->ScaleBackup.IsSet())
	{
		GetOwner()->SetActorScale3D(FMath::LerpStable(this->InterpStartTransform.GetScale3D(), this->ScaleBackup.GetValue(), FMath::Clamp((GetWorld()->GetTimeSeconds() - this->InterpStartTime) / this->SmoothStartInterpTime, 0.0f, 1.0f)));
	}
}

void UInterpFollower::StartFollowing(USceneComponent* InOriginPoint, EHandSide HandSide, const UObject* InUser, FTransform PositionOverride)
{
	if (this->bIsStopping)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([=](){this->StartFollowing(InOriginPoint, HandSide, InUser, PositionOverride);}));
		return;
	}
	
	this->CurrentUser = InUser;

	this->OriginPoint = InOriginPoint;

	switch (HandSide)
	{
	case EHS_Right:
		this->SocketOffset = this->RightHandSocketOffset;
		this->BoneName = TEXT("Hand_R");
		break;

	case EHS_Left:
		this->SocketOffset = this->LeftHandSocketOffset;
		this->BoneName = TEXT("Hand_L");
		break;

	case EHS_None:
		this->SocketOffset = PositionOverride;
		this->BoneName = NAME_None;
		break;

	default:
		break;
	}

	
	this->InterpStartTransform = GetOwner()->GetActorTransform();
	this->InterpStartTime = GetWorld()->GetTimeSeconds();
	
	if (!this->ScaleBackup.IsSet())
		this->ScaleBackup = GetOwner()->GetActorScale3D();
	
	for (UPrimitiveComponent* Current : this->OwnerPrimitiveComponents)
	{
		Current->SetSimulatePhysics(false);
		Current->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UInterpFollower::StopFollowing()
{
	this->bIsStopping = true;

	this->CurrentUser = nullptr;
	
	this->InterpStartTransform = GetOwner()->GetActorTransform();
	this->InterpStartTime = GetWorld()->GetTimeSeconds();
	
	
	for (UPrimitiveComponent* Current : this->OwnerPrimitiveComponents)
	{
		Current->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		if (!Cast<ACharacter>(GetOwner()))
		{
			Current->SetSimulatePhysics(true);

			if (this->OriginPoint)
				Current->AddImpulse((Current->GetComponentLocation() - this->OriginPoint->GetComponentLocation()).GetSafeNormal() * 300.0, NAME_None, true);
		}

	}

	if (Cast<ACharacter>(GetOwner()))
	{
		FRotator OwnerRestoredRotation = GetOwner()->GetActorRotation();
		OwnerRestoredRotation = FRotator(0.0, OwnerRestoredRotation.Yaw, 0.0);
		GetOwner()->SetActorRotation(OwnerRestoredRotation);
		Cast<ACharacter>(GetOwner())->GetCharacterMovement()->AddImpulse((GetOwner()->GetActorLocation() - this->OriginPoint->GetComponentLocation()).GetSafeNormal() * 800.0, true);
	}
	
	this->OriginPoint = nullptr;

	this->bIsStopping = false;
}

