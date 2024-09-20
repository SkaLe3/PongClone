// Fill out your copyright notice in the Description page of Project Settings.


#include "PongBall.h"
#include "Components/StaticMeshComponent.h"
#include "PongPaddlePawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TriggerVolume.h"
#include "PongGameModeBase.h"

// Sets default values
APongBall::APongBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComp;

	//SphereComp->OnComponentHit.AddDynamic(this, &APongBall::OnHit);

	bReplicates = true;
	SetReplicateMovement(true);
	InitialDirection = FVector(0.0f, 1.0f, 0.0f);
	BounceFactor = 6.0f;


}

// Called when the game starts or when spawned
void APongBall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APongBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + (Velocity * DeltaTime);
	FHitResult HitResult;
	SetActorLocation(NewLocation, true, &HitResult);

	if (HitResult.IsValidBlockingHit())
	{
		// Handle the hit if needed
		OnHit(HitResult);
	}

}

void APongBall::Launch()
{
	Velocity = FVector(InitialDirection).GetSafeNormal() * Speed;
}

void APongBall::OnHit(const FHitResult& Hit)
{
	APongPaddlePawn* PP = Cast<APongPaddlePawn>(Hit.GetActor());
	ATriggerVolume* TV = Cast<ATriggerVolume>(Hit.GetActor());
	if (PP)
	{
		FVector Origin = PP->GetActorLocation();
		FVector HitPoint = Hit.ImpactPoint;


		FVector NewDirection = HitPoint - Origin;
		NewDirection *= FVector(1.0f, BounceFactor, 1.0f);
		NewDirection.Normalize();
		Velocity = NewDirection * Speed;
		Velocity.Z = 0;
	}
	else if (TV)
	{
		if (HasAuthority())
		{
			APongGameModeBase* GM = Cast<APongGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			GM->AddScore(Hit.GetActor());
		}
	}
	else
	{
		BounceOfWall(Hit.Normal);
	}
}

void APongBall::BounceOfWall(const FVector& HitNormal)
{
	Velocity = UKismetMathLibrary::GetReflectionVector(Velocity, HitNormal).GetSafeNormal() * Speed;
	Velocity.Z = 0;
}

void APongBall::BounceOfPaddle(const FVector& HitNormal, const float DistanceFromOrigin)
{
	Velocity = UKismetMathLibrary::GetReflectionVector(Velocity, HitNormal).GetSafeNormal() * Speed;
}

