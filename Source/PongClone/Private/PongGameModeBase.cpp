// Fill out your copyright notice in the Description page of Project Settings.


#include "PongGameModeBase.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "PongPlayerController.h"
#include "TimerManager.h"
#include "PongBall.h"

void APongGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}


void APongGameModeBase::RestartGame()
{
	GetWorldTimerManager().SetTimer(TimerHandle_GameStart, this, &APongGameModeBase::StartGame, GameStartTime, false);
	MulticastUpdateTimer(GameStartTime);
}

void APongGameModeBase::StartGame()
{
	for (APongPlayerController* PC : ConnectedControllers)
	{
		PC->RemoveGameOverWidget();
		PC->RemoveScoreWidget();
	}
	ResetScore();
	SpawnBall();
	MulticastStartMatch();
}

void APongGameModeBase::MulticastUpdateTimer_Implementation(float RemainingTime)
{
	for (APongPlayerController* PC : ConnectedControllers)
	{
		PC->DisplayTimerWidget(RemainingTime);
	}
}

void APongGameModeBase::MulticastStartMatch_Implementation()
{
	for (APongPlayerController* PC : ConnectedControllers)
	{
		PC->DisplayScoreWidget();
	}
}

void APongGameModeBase::MulticastScoreUpdate_Implementation()
{
	for (APongPlayerController* PC : ConnectedControllers)
	{
		PC->OnScoreChanged(ScorePlayer0, ScorePlayer1);
	}
}

void APongGameModeBase::MulticastGameOver_Implementation()
{
	for (APongPlayerController* PC : ConnectedControllers)
	{
		PC->DisplayGameOverWidget(true);
	}
}

void APongGameModeBase::SpawnBall()
{
	if (BallClass)
	{
		FRotator SpawnRotation = FRotator::ZeroRotator;
		AActor* SpawnedBall = GetWorld()->SpawnActor<AActor>(BallClass, BallSpawnLocation, SpawnRotation);
		Ball = Cast<APongBall>(SpawnedBall);
		FTimerHandle TimerHandle_LaunchBall;
		GetWorldTimerManager().SetTimer(TimerHandle_LaunchBall, this, &APongGameModeBase::LaunchBall, 3.0f, false);
	}
}

void APongGameModeBase::LaunchBall()
{
	Ball->Launch();
}

void APongGameModeBase::ResetScore()
{
	  ScorePlayer0 = 0;
	  ScorePlayer1 = 0;
}

void APongGameModeBase::AddScore(AActor* Gates)
{
	Ball->Destroy();
	if (GatesTags.Num() > 1)
	{
		if (Gates->ActorHasTag(GatesTags[1]))
		{
			ScorePlayer0++;
		}
		if (Gates->ActorHasTag(GatesTags[0]))
		{
			ScorePlayer1++;
		}
	}
	MulticastScoreUpdate();
	if (ScorePlayer0 >= ScoreToWin)
	{
		ConnectedControllers[0]->DisplayGameOverWidget(true);
		ConnectedControllers[1]->DisplayGameOverWidget(false);
		RestartGame();
	}
	else
	if (ScorePlayer1 >= ScoreToWin)
	{
		ConnectedControllers[0]->DisplayGameOverWidget(false);
		ConnectedControllers[1]->DisplayGameOverWidget(true);
		RestartGame();
	}
	else
	{
		SpawnBall();
	}




}

void APongGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	ConnectedControllers.Add(Cast<APongPlayerController>(NewPlayer));



	if (ConnectedControllers.Num() == 2)
	{
		TArray<AActor*> Camera0;
		TArray<AActor*> Camera1;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), PlayersCameras[0], Camera0);
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), PlayersCameras[1], Camera1);

		TArray<AActor*> Cameras;
		if (Camera0.Num() > 0 && Camera1.Num() > 0)
		{
			Cameras.Add(Camera0[0]);
			Cameras.Add(Camera1[0]);
		}

		int Idx = 0;
		for (APongPlayerController* PC : ConnectedControllers)
		{
			PC->RemoveWaitingMessage();
			RestartPlayerAtPlayerStart(PC, FindPlayerStart(PC, FString::FromInt(Idx)));
			PC->UpdateControlledPaddle();  // TODO: Make replicated
			if (Cameras.Num() > Idx)
			{
				PC->SetCamera(Cameras[Idx]);
			}
			Idx++;

		}
		RestartGame();
		return;
	}
	if (ConnectedControllers.Num() == 1)
	{
		APongPlayerController* YourPC = ConnectedControllers[0];
		if (YourPC)
		{
			YourPC->DisplayWaitingMessage();
		}
	}

}
