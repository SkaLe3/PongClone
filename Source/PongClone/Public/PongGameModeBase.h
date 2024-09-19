// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PongGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PONGCLONE_API APongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	/*========== PLAYERS ==========*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Players")
	TArray<FName> Players;

	/*========== CAMERAS ==========*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cameras")
	TArray<FName> PlayersCameras;

};
