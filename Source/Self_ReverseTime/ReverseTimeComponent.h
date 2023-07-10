// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FramePackage.h"
#include "ReverseTimeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class SELF_REVERSETIME_API UReverseTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReverseTimeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
protected:
	bool bIsReversingTime; // Variable to check if the player is reversing time or not
	bool bOutOfData; // Variable to denote that the double linked list is empty or not
	float ReverseRunningTime;
	float RightRunningTime;
	float LeftRunningTime;
	float RecordedTime;
	float MaxRecordedTime;
	TDoubleLinkedList<FFramePackage> StoredFrames;

	UFUNCTION()
		void SetbIsReversingTime(bool bIsInReversingTime);
	void SetObjectVariables(FVector Location, FRotator Rotation, FVector LinearVelocity, FVector AngularVelocity);
};
