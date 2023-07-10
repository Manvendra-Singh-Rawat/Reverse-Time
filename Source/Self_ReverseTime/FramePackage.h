#pragma once

#include "Math/Vector.h"
#include "Math/Rotator.h"

struct FFramePackage
{
	FORCEINLINE FFramePackage();
	FORCEINLINE FFramePackage(FVector InLocation, FRotator InRotation, FVector InLinearVelocity, FVector InAngluarVelocity, float InDeltaTime);

	FVector Location;
	FRotator Rotation;
	FVector LinearVelocity;
	FVector AngularVelocity;
	float DeltaTime;
};

FFramePackage::FFramePackage()
{}

FFramePackage::FFramePackage(FVector InLocation, FRotator InRotation, FVector InLinearVelocity, FVector InAngluarVelocity, float InDeltaTime)
{
	this->Location = InLocation;
	this->Rotation = InRotation;
	this->LinearVelocity = InLinearVelocity;
	this->AngularVelocity = InAngluarVelocity;
	this->DeltaTime = InDeltaTime;
}