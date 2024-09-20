// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PongGameModeBase.generated.h"


class APongPlayerController;

/**
 *
 */
UCLASS()
class PONGCLONE_API APongGameModeBase : public AGameModeBase
{
	GENERATED_BODY()



protected:
	virtual void BeginPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void RestartGame();
	void StartGame();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateTimer(float RemainingTime);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartMatch();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastScoreUpdate();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastGameOver();



public:
	void AddScore(AActor* Gates);
	void SpawnBall();
	void LaunchBall();
	void ResetScore();


protected:
	/*========== CAMERAS TAGS ==========*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cameras")
	TArray<FName> PlayersCameras;

	/*========== GATES TAGS==========*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cameras")
	TArray<FName> GatesTags;

	/*========== GAMEPLAY ==========*/
	FTimerHandle TimerHandle_GameStart;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	float GameStartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int ScorePlayer0 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int ScorePlayer1 = 0;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	int ScoreToWin = 5;

	/*========== Ball ==========*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AActor> BallClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning")
	FVector BallSpawnLocation;

	TArray<APongPlayerController*> ConnectedControllers;

	class APongBall* Ball;



};
