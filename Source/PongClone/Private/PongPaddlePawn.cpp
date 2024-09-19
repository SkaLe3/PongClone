// Fill out your copyright notice in the Description page of Project Settings.


#include "PongPaddlePawn.h"
#include "Components/BoxComponent.h"

// Sets default values
APongPaddlePawn::APongPaddlePawn()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = BoxComp;
	MeshComp->SetupAttachment(BoxComp);

	bReplicates = true;
}

// Called when the game starts or when spawned
void APongPaddlePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APongPaddlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UBoxComponent* APongPaddlePawn::GetCollisionComponent()
{
	 return BoxComp;
}
