// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay() 
{
    Super::BeginPlay();

    HUD = CreateWidget(this, HUDClass);
    if(HUD) {
        HUD->AddToViewport();
    } else {
        UE_LOG(LogTemp, Error, TEXT("Failed to create HUD widget!"));
    }
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner) 
{
    Super::GameHasEnded(EndGameFocus, bIsWinner);

    if(HUD) {
        HUD->RemoveFromViewport();
    }

    if(bIsWinner) {
        UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
        if(WinScreen != nullptr) {
            WinScreen->AddToViewport();
        }
    } else {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        if(LoseScreen != nullptr) {
            LoseScreen->AddToViewport();
        }
    }
    
    
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

