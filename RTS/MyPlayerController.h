// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RTS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AMyPlayerController();

protected:
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	TWeakObjectPtr<class AMyHUD> HUD;

private:

	void SelectionPressed();

	void SelectionReleased();

	void MoveOrAttack();
	
	UPROPERTY()
    TArray<class ARTSCharacter*> SelectedActors;
	
};
