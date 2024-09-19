// Fill out your copyright notice in the Description page of Project Settings.


#include "PongPlayerController.h"
#include "PongInputConfigData.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PongPaddlePawn.h"

void APongPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ControlledPaddle = Cast<APongPaddlePawn>(GetPawn());

}

void APongPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(InputComponent);

	PEI->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &APongPlayerController::Move);
}


void APongPlayerController::Move(const FInputActionValue& Value)
{
	if (ControlledPaddle)
	{
	   const float MoveValue = Value.Get<float>();
	   FVector NewLocation = ControlledPaddle->GetActorLocation();
	   NewLocation.X += MoveValue * MovementSpeed * GetWorld()->GetDeltaSeconds();
	   FVector BoxExtent = ControlledPaddle->GetCollisionComponent()->GetScaledBoxExtent();
	   NewLocation.X = FMath::Clamp(NewLocation.X, MinBoundary.X + BoxExtent.X, MaxBoundary.X - BoxExtent.X);
	   ControlledPaddle->SetActorLocation(NewLocation);
	}
}
