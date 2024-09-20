// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "PongPlayerController.generated.h"


class UUserWidget;
/**
 * 
 */
UCLASS()
class PONGCLONE_API APongPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupInputComponent() override;
	/*
	* Function that updates ControlledPaddle pointer, because it should be set after execution of HandleStartingNewPlayer_Implementation in APongGameModeBase
	* It is used to avoid casting to the APongPaddlePawn in the Move function which is called every frame while input is pressed.
	* Getting Pawn is necessary due to the implementation of movement inside the controller class as it is said in the task, instead of implementing it in the pawn class
	*/
	void UpdateControlledPaddle();

	UFUNCTION(Client, Reliable)
	void DisplayWaitingMessage();

	UFUNCTION(Client, Reliable)
	void RemoveWaitingMessage();

	UFUNCTION(Client, Reliable)
	void DisplayTimerWidget(float RemainingTime);
	void RemoveTimerWidget();

	UFUNCTION(Client, Reliable)
	void DisplayScoreWidget();
	UFUNCTION(Client, Reliable)
	void RemoveScoreWidget();

	UFUNCTION(Client, Reliable)
	void DisplayGameOverWidget(bool bHasWon);
	UFUNCTION(Client, Reliable)
	void RemoveGameOverWidget();

	UFUNCTION(Client, Reliable)
	void OnScoreChanged(int ScorePlayer0, int ScorePlayer1);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateScoreDisplay(int ScorePlayer0, int ScorePlayer1);

	UFUNCTION(BlueprintImplementableEvent)
	void SetGameOverMessage(bool bHasWon);

	UFUNCTION(Client, Reliable)
	void SetCamera(AActor* CameraActor);

	UFUNCTION(BlueprintCallable)
	float GetStartTimerRemainigTime();

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetScoreWidget();

	UFUNCTION(BlueprintCallable)
	UUserWidget* GetGameOverWidget();
	
protected:
	void MoveSideways(const FInputActionValue& Value);
	void Move(const float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMove(const float Value);




protected:
	/*========== INPUT ==========*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UPongInputConfigData* InputActions;

	/*========== Pawn ==========*/
	class APongPaddlePawn* ControlledPaddle;

	/*========== PROPERTIES ==========*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MinBoundary;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MaxBoundary;

	/*========== UI ==========*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> WaitingMessageWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> TimerWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ScoreWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UUserWidget* WaitingMessageWidgetInstance;
	UUserWidget* TimerWidgetInstance;
	UUserWidget* ScoreWidgetInstance;
	UUserWidget* GameOverWidgetInstance;


	FTimerHandle TimerHandle_GameStart;
};
