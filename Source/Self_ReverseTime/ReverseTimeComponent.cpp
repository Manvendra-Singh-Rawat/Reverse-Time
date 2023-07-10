#include "ReverseTimeComponent.h"
#include "Self_ReverseTimeCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UReverseTimeComponent::UReverseTimeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxRecordedTime = 6.0f; // Max amount of time the player can revind to.
	bIsReversingTime = false;
}


// Called when the game starts
void UReverseTimeComponent::BeginPlay()
{
	Super::BeginPlay();
	RecordedTime = 0.0f;

	ASelf_ReverseTimeCharacter* ReverseTimeCharacter = Cast<ASelf_ReverseTimeCharacter>(UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0));
	ReverseTimeCharacter->ReverseTimeDelegate.AddDynamic(this, &UReverseTimeComponent::SetbIsReversingTime);
}


// Called every frame
void UReverseTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsReversingTime == false)
	{
		ReverseRunningTime = 0.0f;
		RightRunningTime = 0.0f;
		LeftRunningTime = 0.0f;

		AActor* Owner = GetOwner();
		auto Component = Owner->GetComponentByClass(UStaticMeshComponent::StaticClass());
		UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(Component);

		if (Component != nullptr)
		{
			if (SMC != nullptr)
			{
				FFramePackage TempData(Owner->GetActorLocation(), Owner->GetActorRotation(), SMC->GetPhysicsLinearVelocity(), SMC->GetPhysicsAngularVelocityInDegrees(), DeltaTime);
				if (RecordedTime < MaxRecordedTime)
				{
					StoredFrames.AddTail(TempData);
					RecordedTime += DeltaTime;
					bOutOfData = false;
				}
				else if (RecordedTime > MaxRecordedTime)
				{
					while (RecordedTime > MaxRecordedTime)
					{
						auto Head = StoredFrames.GetHead();
						float Head_DeltaTime = Head->GetValue().DeltaTime;

						RecordedTime -= Head_DeltaTime;
						StoredFrames.RemoveNode(Head);
					}

					StoredFrames.AddTail(TempData);
					RecordedTime += DeltaTime;
				}
			}
		}
	}
	else if(bIsReversingTime == true)
	{
		if (!bOutOfData)
		{
			ReverseRunningTime += DeltaTime * 0.5;

			auto Right = StoredFrames.GetTail();
			auto Left = Right->GetPrevNode();

			LeftRunningTime = RightRunningTime + Right->GetValue().DeltaTime;

			while (LeftRunningTime < ReverseRunningTime)
			{
				LeftRunningTime = LeftRunningTime + Left->GetValue().DeltaTime;
				RightRunningTime = RightRunningTime + Right->GetValue().DeltaTime;

				Right = Left;
				Left = Left->GetPrevNode();

				auto Tail = StoredFrames.GetTail();
				RecordedTime -= Tail->GetValue().DeltaTime;

				StoredFrames.RemoveNode(StoredFrames.GetTail());

				if (StoredFrames.GetHead() == Left)
				{
					bOutOfData = true;
					bIsReversingTime = false;
				}
			}

			if (ReverseRunningTime <= LeftRunningTime && ReverseRunningTime >= RightRunningTime)
			{
				float DT = ReverseRunningTime - RightRunningTime;
				float Interval = LeftRunningTime - RightRunningTime;
				float Fraction = DT / Interval;
				FVector TempLocation = FMath::VInterpTo(Right->GetValue().Location, Left->GetValue().Location, Fraction, 1.0f);
				FRotator TempRotation = FMath::RInterpTo(Right->GetValue().Rotation, Left->GetValue().Rotation, Fraction, 1.0f);
				FVector TempLinearVelocity = FMath::VInterpTo(Right->GetValue().LinearVelocity, Left->GetValue().LinearVelocity, Fraction, 1.0f);
				FVector TempAngularVelocity = FMath::VInterpTo(Right->GetValue().AngularVelocity, Left->GetValue().AngularVelocity, Fraction, 1.0f);
			
				SetObjectVariables(TempLocation, TempRotation, TempLinearVelocity, TempAngularVelocity);
			}
		}
	}
}

void UReverseTimeComponent::SetbIsReversingTime(bool bIsInReversingTime)
{
	bIsReversingTime = bIsInReversingTime;
	if (bIsReversingTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("in"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("out"));
	}
	
}

void UReverseTimeComponent::SetObjectVariables(FVector Location, FRotator Rotation, FVector LinearVelocity, FVector AngularVelocity)
{
	AActor* Owner = GetOwner();
	UActorComponent* Component = Owner->FindComponentByClass(UStaticMeshComponent::StaticClass());
	if (Component != nullptr)
	{
		UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(Component);
		if (SMC != nullptr)
		{
			Owner->SetActorLocationAndRotation(Location, Rotation);
			SMC->SetPhysicsLinearVelocity(LinearVelocity);
			SMC->SetPhysicsAngularVelocityInDegrees(AngularVelocity);
		}
	}
}

