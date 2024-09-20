// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PongBall.generated.h"

UCLASS()
class PONGCLONE_API APongBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APongBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Launch();

protected:
	void OnHit(const FHitResult& Hit);
	void BounceOfWall(const FVector& HitNormal);
	void BounceOfPaddle(const FVector& HitNormal, const float DistanceFromOrigin);

protected:
	/*========== COMPONENTS ==========*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* SphereComp;

	/*========== PROPERTIES ==========*/	  // TODO: Put in component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	FVector InitialDirection;

	UPROPERTY(VisibleAnywhere, Category = "Properties")
	FVector Velocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Properties")
	float BounceFactor;

};
