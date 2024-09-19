// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameModeBase.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void APongGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> PlayerActor0;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Players[0], PlayerActor0);

	TArray<AActor*> PlayerActor1;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Players[1], PlayerActor1);

	TArray<AActor*> PlayerCamera0;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), PlayersCameras[0], PlayerCamera0);

	TArray<AActor*> PlayerCamera1;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), PlayersCameras[1], PlayerCamera1);

	if (!PlayerActor0.IsEmpty() && !PlayerCamera0.IsEmpty())
	{
		APlayerController* PC = Cast<APlayerController>(Cast<APawn>(PlayerActor0[0])->GetController());
		if (PC)
		{
			PC->SetViewTarget(PlayerCamera0[0]);
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to set view target for first player"));
	}
	if (!PlayerActor1.IsEmpty() && !PlayerCamera1.IsEmpty())
	{
		APlayerController* PC = Cast<APlayerController>(Cast<APawn>(PlayerActor1[0])->GetController());
		if (PC)
		{
			PC->SetViewTarget(PlayerCamera1[0]);
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to set view target for second player"));
	}
}
