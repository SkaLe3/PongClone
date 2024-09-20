// Fill out your copyright notice in the Description page of Project Settings.


#include "PongPlayerController.h"
#include "PongInputConfigData.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PongPaddlePawn.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/GameModeBase.h"

void APongPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ControlledPaddle = Cast<APongPaddlePawn>(GetPawn());

}

void APongPlayerController::UpdateControlledPaddle()
{
	ControlledPaddle = Cast<APongPaddlePawn>(GetPawn());
}

void APongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(InputComponent);

	PEI->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &APongPlayerController::MoveSideways);
}


void APongPlayerController::MoveSideways(const FInputActionValue& Value)
{
	const float MoveValue = Value.Get<float>();

	Move(MoveValue);

	if (!HasAuthority())
	{
		ServerMove(MoveValue);
	}

}

void APongPlayerController::Move(const float MoveValue)
{
	ControlledPaddle = Cast<APongPaddlePawn>(GetPawn());
	if (ControlledPaddle)
	{
		FVector NewLocation = ControlledPaddle->GetActorLocation();
		FVector RightVector = ControlledPaddle->GetActorRightVector();
		NewLocation += RightVector * MoveValue * MovementSpeed * GetWorld()->GetDeltaSeconds();
		FVector BoxExtent = ControlledPaddle->GetCollisionComponent()->GetScaledBoxExtent();
		NewLocation.X = FMath::Clamp(NewLocation.X, MinBoundary + BoxExtent.Y, MaxBoundary - BoxExtent.Y);
		ControlledPaddle->SetActorLocation(NewLocation);
	}

}

void APongPlayerController::ServerMove_Implementation(const float MoveValue)
{
	Move(MoveValue);
}

bool APongPlayerController::ServerMove_Validate(const float MoveValue)
{
	return true;
}

void APongPlayerController::DisplayWaitingMessage_Implementation()
{
	if (WaitingMessageWidgetClass)
	{
		WaitingMessageWidgetInstance = CreateWidget<UUserWidget>(this, WaitingMessageWidgetClass);
		if (WaitingMessageWidgetInstance)
		{
			WaitingMessageWidgetInstance->AddToViewport();
		}
	}
}

void APongPlayerController::RemoveWaitingMessage_Implementation()
{
	if (WaitingMessageWidgetInstance)
	{
		WaitingMessageWidgetInstance->RemoveFromViewport();
		WaitingMessageWidgetInstance = nullptr;
	}
}

void APongPlayerController::DisplayTimerWidget_Implementation(float RemainingTime)
{
	if (TimerWidgetClass)
	{
		TimerWidgetInstance = CreateWidget<UUserWidget>(this, TimerWidgetClass);
		if (TimerWidgetInstance)
		{
			TimerWidgetInstance->AddToViewport();

			GetWorldTimerManager().SetTimer(TimerHandle_GameStart, this, &APongPlayerController::RemoveTimerWidget, RemainingTime, false);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Timer set on client"));
}

void APongPlayerController::RemoveTimerWidget()
{
	if (TimerWidgetInstance)
	{
		TimerWidgetInstance->RemoveFromViewport();
		TimerWidgetInstance = nullptr;
	}
}

void APongPlayerController::DisplayScoreWidget_Implementation()
{
	if (ScoreWidgetClass)
	{
		ScoreWidgetInstance = CreateWidget<UUserWidget>(this, ScoreWidgetClass);
		if (ScoreWidgetInstance)
		{
			ScoreWidgetInstance->AddToViewport();
		}
	}
}

void APongPlayerController::RemoveScoreWidget_Implementation()
{
	if (ScoreWidgetInstance)
	{
		ScoreWidgetInstance->RemoveFromViewport();
		ScoreWidgetInstance = nullptr;
	}
}


void APongPlayerController::DisplayGameOverWidget_Implementation(bool bHasWon)
{
	if (GameOverWidgetClass)
	{
		GameOverWidgetInstance = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
		if (GameOverWidgetInstance)
		{
			GameOverWidgetInstance->AddToViewport();
		}
	}
	SetGameOverMessage(bHasWon);
}



void APongPlayerController::RemoveGameOverWidget_Implementation()
{

	if (GameOverWidgetInstance)
	{
		GameOverWidgetInstance->RemoveFromViewport();
		GameOverWidgetInstance = nullptr;
	}

}

void APongPlayerController::OnScoreChanged_Implementation(int ScorePlayer0, int ScorePlayer1)
{
	UpdateScoreDisplay(ScorePlayer0, ScorePlayer1);
}

void APongPlayerController::SetCamera_Implementation(AActor* CameraActor)
{
	if (CameraActor)
	{
		CameraActor->SetOwner(this);
		SetViewTarget(CameraActor);
		UE_LOG(LogTemp, Warning, TEXT("SetCamera called, CameraActor: %s"), *CameraActor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid CameraActor passed to SetCamera"));
	}
}

float APongPlayerController::GetStartTimerRemainigTime()
{
	return GetWorldTimerManager().GetTimerRemaining(TimerHandle_GameStart);
}

UUserWidget* APongPlayerController::GetScoreWidget()
{
	return ScoreWidgetInstance;
}

UUserWidget* APongPlayerController::GetGameOverWidget()
{
	return GameOverWidgetInstance;
}
